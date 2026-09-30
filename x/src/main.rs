use anyhow::{Context, Result};
use image::codecs::jpeg::JpegEncoder;
use image::imageops::FilterType;
use image::{ColorType, ImageReader};
use log::debug;
use parley::{
    Alignment, AlignmentOptions, FontContext, Layout, LayoutContext, PositionedLayoutItem,
    StyleProperty,
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
    retweet: Option<Box<Tweet>>,
}

fn text_place(text: &str, x: f32, y: f32, max_width: f32, font_size: f32) -> (String, f32) {
    let mut out = String::new();
    // 排版字体生成 layout
    let mut font_cx = FontContext::new();
    let mut layout_cx = LayoutContext::new();
    let mut builder = layout_cx.ranged_builder(&mut font_cx, text, 1.0, true);
    builder.push_default(StyleProperty::FontSize(font_size));
    let mut layout: Layout<()> = builder.build(text);
    layout.break_all_lines(Some(max_width));
    layout.align(Alignment::Start, AlignmentOptions::default());
    let mut last_y: f32 = 0.0;
    for line in layout.lines() {
        let range = line.text_range();
        let line_text = &text[range];
        let baseline = line
            .items()
            .find_map(|item| {
                if let PositionedLayoutItem::GlyphRun(run) = item {
                    Some(run.baseline())
                } else {
                    None
                }
            })
            .unwrap_or(0.0);
        out.push_str(&format!(
            r#"  <text x="{:.2}" y="{:.2}">{}</text>"#,
            x,
            y + baseline,
            line_text.trim()
        ));
        out.push('\n');
        last_y = last_y.max(y + baseline);
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
    let mut options = usvg::Options::default();
    options.fontdb = FONTDB.clone();
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
    // tiny-skia 是 RGBA，需要转换成 RGB
    let rgba = pixmap.data();
    let mut rgb = vec![0u8; (width * height * 3) as usize];
    for (dst, src) in rgb.chunks_exact_mut(3).zip(rgba.chunks_exact(4)) {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
    }
    // JPEG 编码
    let file = File::create(output_path)?;
    let writer = BufWriter::new(file);
    let mut encoder = JpegEncoder::new_with_quality(writer, quality);
    encoder.encode(&rgb, width, height, ColorType::Rgb8.into())?;
    Ok(())
}

fn build_twitter_card(tweet: &Tweet, retweet: Option<(String, f32)>) -> Result<(String, f32)> {
    let full_text = if tweet.full_text.starts_with("RT @") {
        "↩ Retweeted"
    } else {
        &tweet.full_text
    };
    // 渲染文本部分
    let mut text = String::new();
    let (text_svg, last_y) = text_place(full_text, PADDING, 64.0, CONTENT_WIDTH, 16.0);
    // 如果有翻译，就渲染翻译
    text.push_str(&text_svg);
    let last_y = if !tweet.translated_text.is_empty() {
        let (split, last_y) = split_place(PADDING, last_y + PADDING, CONTENT_WIDTH);
        text.push_str(&split);
        text.push('\n');
        let (translated_svg, last_y) = text_place(
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
    let (mut image_svg, last_y) =
        image_place(&tweet.media_urls, PADDING, last_y + PADDING, CONTENT_WIDTH)?;
    // 如果有转发，就把转发卡片的 SVG 直接嵌套进来
    let last_y = if let Some((retweet_svg, retweet_height)) = retweet {
        let scale = CONTENT_WIDTH / SVG_WIDTH;
        let y = last_y + PADDING;
        image_svg.push_str(&format!(
            r#"<g transform="translate({PADDING} {y}) scale({scale})">{retweet_svg}</g>"#
        ));
        image_svg.push('\n');
        y + retweet_height * scale
    } else {
        last_y
    };
    let height = last_y + PADDING;
    let output_svg = read_to_string("tweet.tmpl")
        .context("读取 SVG 模板失败")?
        .replace("{{ HEIGHT }}", &height.to_string())
        .replace("{{ BORDER_HEIGHT }}", &(height - 1.0).to_string())
        .replace("{{ AVATAR }}", &tweet.avatar_url)
        .replace("{{ USERNAME }}", &tweet.user_name)
        .replace("{{ ID }}", &tweet.screen_name)
        .replace("{{ TEXT }}", &text)
        .replace("{{ IMAGE }}", &image_svg);
    Ok((output_svg, height))
}

fn main() -> Result<()> {
    env_logger::init();
    let mut args = args().skip(1);
    if args.len() != 2 {
        anyhow::bail!("Usage: ./prog <tweet.json> <output.jpg>");
    }
    let tweet = args.next().expect("unreachable");
    let output = args.next().expect("unreachable");
    let tweet = read_to_string(&tweet).with_context(|| format!("读取推文 JSON 失败: {tweet}"))?;
    let tweet: Tweet = serde_json::from_str(&tweet).context("解析推文 JSON 失败")?;
    let retweet = if let Some(retweet) = &tweet.retweet {
        // 如果有转发推特，则先渲染
        let (svg, height) = build_twitter_card(retweet, None)?;
        Some((svg, height))
    } else {
        None
    };
    let (output_svg, _) = build_twitter_card(&tweet, retweet)?;
    debug!("{}", output_svg);
    render_svg(output_svg.as_bytes(), &output, 95)
}
