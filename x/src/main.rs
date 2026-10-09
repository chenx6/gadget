use anyhow::{Context, Result};
use chrono::{DateTime, FixedOffset};
use image::codecs::jpeg::JpegEncoder;
use image::imageops::FilterType;
use image::{ImageBuffer, ImageReader, RgbaImage};
use log::debug;
use parley::{
    Alignment, AlignmentOptions, FontContext, GenericFamily, Layout, LayoutContext,
    PositionedLayoutItem, StyleProperty,
};
use resvg::{tiny_skia, usvg};
use serde::Deserialize;
use std::env::args;
use std::fs::{File, read_to_string};
use std::io::BufWriter;
use std::sync::{Arc, LazyLock};
use tiny_skia::{Pixmap, Transform};
use usvg::fontdb::Database;

const PADDING: f32 = 16.0;
const SVG_WIDTH: f32 = 600.0;
const CONTENT_WIDTH: f32 = SVG_WIDTH - PADDING * 2.0;
const DEFAULT_TEMPLATE: &str = include_str!("../tweet.tmpl");
const TEXT_FAMILY: &str = "Source Han Sans CN";
const EMOJI_FAMILY: &str = "Noto Color Emoji";
static FONTDB: LazyLock<Arc<Database>> = LazyLock::new(|| {
    let mut db = Database::new();
    db.load_fonts_dir("font");
    Arc::new(db)
});

#[derive(Deserialize)]
struct Tweet {
    avatar_url: String,
    user_name: String,
    screen_name: String,
    full_text: String,
    #[serde(default)]
    translated_text: String,
    media_urls: Vec<String>,
    created_at: String,
    retweet: Option<Box<Tweet>>,
}

fn escape_xml(text: &str) -> String {
    text.replace('&', "&amp;")
        .replace('<', "&lt;")
        .replace('>', "&gt;")
}

/// 返回 emoji 字体的 (blob id, index)，用于判断某个 run 是否使用了 emoji 字体。
fn emoji_font_key(font_cx: &mut FontContext) -> Option<(u64, u32)> {
    let family_id = font_cx.collection.family_id(EMOJI_FAMILY)?;
    let family = font_cx.collection.family(family_id)?;
    let font = family.default_font()?;
    let index = font.index();
    let blob = font.load(Some(&mut font_cx.source_cache))?;
    Some((blob.id(), index))
}

fn text_place(
    font_cx: &mut FontContext,
    layout_cx: &mut LayoutContext<()>,
    text: &str,
    x: f32,
    y: f32,
    max_width: f32,
    font_size: f32,
) -> (String, f32) {
    let mut out = String::new();
    let emoji_key = emoji_font_key(font_cx);
    // 排版字体生成 layout
    let mut builder = layout_cx.ranged_builder(font_cx, text, 1.0, true);
    builder.push_default(StyleProperty::FontSize(font_size));
    let mut layout: Layout<()> = builder.build(text);
    layout.break_all_lines(Some(max_width));
    layout.align(Alignment::Start, AlignmentOptions::default());
    let mut last_y: f32 = 0.0;
    for line in layout.lines() {
        for item in line.items() {
            // 按 parley 解析出的字体拆分 run，并用其算出的横向位置精确定位。
            let PositionedLayoutItem::GlyphRun(glyph_run) = item else {
                continue;
            };
            let baseline = glyph_run.baseline();
            last_y = last_y.max(y + baseline);
            let run = glyph_run.run();
            let piece = text[run.text_range()].trim_end();
            if piece.is_empty() {
                continue;
            }
            // 通过当前 glyph_run 的 font id 判断是否为 emoji
            let font = run.font();
            let family = if emoji_key == Some((font.data.id(), font.index)) {
                EMOJI_FAMILY
            } else {
                TEXT_FAMILY
            };
            out.push_str(&format!(
                r#"  <text x="{:.2}" y="{:.2}" font-family="{}">{}</text>"#,
                x + glyph_run.offset(),
                y + baseline,
                family,
                escape_xml(piece)
            ));
            out.push('\n');
        }
    }
    (out, last_y)
}

fn image_place(images: &[String], x: f32, y: f32, width: f32) -> Result<(String, f32)> {
    let mut last_y = y;
    let mut res = String::new();
    if images.len() == 1 {
        let (w, h) = image::image_dimensions(&images[0])?;
        let new_height = width / w as f32 * h as f32;
        res.push_str(&format!(
            r#"<image image-rendering="optimizeSpeed" href="{}" x="{}" y="{}" width="{}" height="{}" clip-path="url(#rounded)" />"#,
            images[0], x, y, width, new_height
        ));
        res.push('\n');
        last_y = last_y.max(y + new_height);
    } else {
        const NEW_WIDTH: u32 = 180;
        const IMAGE_GAP: f32 = 8.0;
        let step = NEW_WIDTH as f32 + IMAGE_GAP;
        for (yidx, row) in images.chunks(3).enumerate() {
            for (xidx, image) in row.iter().enumerate() {
                let image_x = x + xidx as f32 * step;
                let image_y = y + yidx as f32 * step;
                let img = ImageReader::open(image)?.decode()?;
                // 等比例缩放并居中裁剪，横图和竖图都填满正方形。
                let img = img.resize_to_fill(NEW_WIDTH, NEW_WIDTH, FilterType::Triangle);
                let img_name = format!("data/{}_{}.jpg", xidx, yidx);
                img.save(&img_name)?;
                res.push_str(&format!(
                    r#"<image image-rendering="optimizeSpeed" href="{}" x="{}" y="{}" width="{}" height="{}" clip-path="url(#rounded)" />"#,
                    img_name,
                    image_x,
                    image_y,
                    NEW_WIDTH,
                    NEW_WIDTH
                ));
                res.push('\n');
                last_y = last_y.max(image_y + NEW_WIDTH as f32);
            }
        }
    }
    Ok((res, last_y))
}

fn split_place(x: f32, y: f32, width: f32) -> (String, f32) {
    (
        format!(
            r#"<line x1="{}" y1="{}" x2="{}" y2="{}" stroke="{}"/>"#,
            x,
            y,
            x + width,
            y,
            "#cfd9e2"
        ),
        y,
    )
}

fn render_svg(svg_data: &[u8], output_path: &str, quality: u8) -> Result<()> {
    // 解析 SVG
    const SCALE: f32 = 2.0;
    let options = usvg::Options {
        fontdb: FONTDB.clone(),
        ..Default::default()
    };
    let tree = usvg::Tree::from_data(svg_data, &options).context("解析 SVG 失败")?;
    let size = tree.size();
    let width = (size.width().ceil() * SCALE) as u32;
    let height = (size.height().ceil() * SCALE) as u32;
    // 使用 resvg 渲染
    let mut pixmap = Pixmap::new(width, height).context("创建 Pixmap 失败")?;
    pixmap.fill(tiny_skia::Color::WHITE);
    resvg::render(
        &tree,
        Transform::from_scale(SCALE, SCALE),
        &mut pixmap.as_mut(),
    );
    let img: RgbaImage =
        ImageBuffer::from_raw(width, height, pixmap.take()).context("构造 RGBA 图像失败")?;
    // JPEG 编码
    let file = File::create(output_path)?;
    let writer = BufWriter::new(file);
    let mut encoder = JpegEncoder::new_with_quality(writer, quality);
    encoder.encode_image(&img)?;
    Ok(())
}

fn format_time(input: &str) -> Result<String> {
    let dt = DateTime::parse_from_str(input, "%a %b %d %H:%M:%S %z %Y")?;
    let offset = FixedOffset::east_opt(8 * 3600).context("解析 FixedOffset 失败")?;
    let dt_local = dt.with_timezone(&offset);
    let output = dt_local.format("%-I:%M %p · %b %-d, %Y").to_string();
    Ok(output)
}

fn load_template(path: Option<&str>) -> Result<String> {
    match path {
        Some(p) => read_to_string(p).with_context(|| format!("读取 SVG 模板失败: {p}")),
        None => Ok(DEFAULT_TEMPLATE.to_string()),
    }
}

fn build_twitter_card(
    font_cx: &mut FontContext,
    layout_cx: &mut LayoutContext<()>,
    tweet: &Tweet,
    retweet: Option<(String, f32)>,
    template: &str,
) -> Result<(String, f32)> {
    let only_retweet = tweet.full_text.starts_with("RT @");
    let full_text = if only_retweet {
        "↩ Retweeted"
    } else {
        &tweet.full_text
    };
    // 渲染文本部分
    let mut text = String::new();
    let (text_svg, last_y) = text_place(
        font_cx,
        layout_cx,
        full_text,
        PADDING,
        64.0,
        CONTENT_WIDTH,
        16.0,
    );
    // 如果有翻译，就渲染翻译
    text.push_str(&text_svg);
    let last_y = if !tweet.translated_text.is_empty() {
        let (split, last_y) = split_place(PADDING, last_y + PADDING, CONTENT_WIDTH);
        text.push_str(&split);
        text.push('\n');
        let (translated_svg, last_y) = text_place(
            font_cx,
            layout_cx,
            &tweet.translated_text,
            PADDING,
            last_y + PADDING,
            CONTENT_WIDTH,
            16.0,
        );
        text.push_str(&translated_svg);
        last_y
    } else {
        last_y
    };
    // 渲染图片
    let (mut image_svg, last_y) = if only_retweet || tweet.media_urls.is_empty() {
        (String::new(), last_y)
    } else {
        image_place(&tweet.media_urls, PADDING, last_y + PADDING, CONTENT_WIDTH)?
    };
    // 如果有转发，就把转发卡片的 SVG 直接嵌套进来
    let last_y = if let Some((retweet_svg, retweet_height)) = retweet {
        let scale = CONTENT_WIDTH / SVG_WIDTH;
        let retweet_y = last_y + PADDING;
        image_svg.push_str(&format!(
            r#"<g transform="translate({PADDING} {retweet_y}) scale({scale})">{retweet_svg}</g>"#
        ));
        image_svg.push('\n');
        retweet_y + retweet_height * scale
    } else {
        last_y
    };
    let tweeter_time = format_time(&tweet.created_at)?;
    let (created, last_y) = text_place(
        font_cx,
        layout_cx,
        &tweeter_time,
        PADDING,
        last_y + PADDING / 2.0, // 边框占据了部分 Padding, 所以手动去掉一些
        CONTENT_WIDTH,
        16.0,
    );
    let height = last_y + PADDING;
    let output_svg = template
        .replace("{{ HEIGHT }}", &height.to_string())
        .replace("{{ BORDER_HEIGHT }}", &(height - 1.0).to_string())
        .replace("{{ AVATAR }}", &tweet.avatar_url)
        .replace("{{ USERNAME }}", &tweet.user_name)
        .replace("{{ ID }}", &tweet.screen_name)
        .replace("{{ TEXT }}", &text)
        .replace("{{ IMAGE }}", &image_svg)
        .replace("{{ CREATED_AT }}", &created);
    Ok((output_svg, height))
}

fn main() -> Result<()> {
    env_logger::init();
    let mut args = args().skip(1);
    if args.len() < 2 || args.len() > 3 {
        anyhow::bail!("Usage: ./prog <tweet.json> <output.jpg> [template.tmpl]");
    }
    let tweet = args.next().expect("unreachable");
    let output = args.next().expect("unreachable");
    let template_path = args.next();
    let template = load_template(template_path.as_deref())?;
    let tweet = read_to_string(&tweet).with_context(|| format!("读取推文 JSON 失败: {tweet}"))?;
    let tweet: Tweet = serde_json::from_str(&tweet).context("解析推文 JSON 失败")?;
    let mut font_cx = FontContext::new();
    font_cx.collection.load_fonts_from_paths(["font"]);
    // 给 parley 注册字体
    for (generic, family) in [
        (GenericFamily::SansSerif, TEXT_FAMILY),
        (GenericFamily::Emoji, EMOJI_FAMILY),
    ] {
        if let Some(id) = font_cx.collection.family_id(family) {
            font_cx
                .collection
                .set_generic_families(generic, [id].into_iter());
        }
    }
    let mut layout_cx = LayoutContext::new();
    let retweet = if let Some(retweet) = &tweet.retweet {
        // 如果有转发推特，则先渲染
        let (svg, height) =
            build_twitter_card(&mut font_cx, &mut layout_cx, retweet, None, &template)?;
        Some((svg, height))
    } else {
        None
    };
    let (output_svg, _) =
        build_twitter_card(&mut font_cx, &mut layout_cx, &tweet, retweet, &template)?;
    debug!("{}", output_svg);
    render_svg(output_svg.as_bytes(), &output, 95)
}
