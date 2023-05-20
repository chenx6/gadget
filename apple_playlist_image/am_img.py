from PIL import Image, ImageDraw


def draw_strip_square(
    size: tuple[int, int],
    fill: tuple[int, int, int],
) -> Image.Image:
    img = Image.new("RGBA", size, (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    sx, sy, dx, dy = 0, 0, size[0], size[1]
    delta = 10
    for i in range(-size[0], size[0] + 1, 20):
        draw.line(
            (sx + i - delta, sy - delta, dx + i + delta, dy + delta), fill=fill, width=8
        )
    return img


def draw_ring(diameter: int, inner_diameter: int, color: tuple[int, int, int]):
    img = Image.new("RGBA", (diameter, diameter), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    draw.ellipse((0, 0, diameter, diameter), outline=color, width=int((diameter - inner_diameter) / 2))
    return img


def draw_house_cover() -> Image.Image:
    img = Image.new("RGBA", (500, 500), (0, 0, 0, 255))
    red_part = draw_strip_square((100, 100), (255, 0, 0))
    white_part = draw_strip_square((100, 100), (255, 255, 255))
    for ix, iy, ty in [
        (200, 0, 0),
        (400, 0, 0),
        (100, 100, 1),
        (0, 200, 1),
        (200, 200, 0),
        (400, 200, 0),
        (300, 300, 1),
    ]:
        if ty == 0:
            img.alpha_composite(white_part, (ix, iy))
        elif ty == 1:
            img.alpha_composite(red_part, (ix, iy))
    return img


def draw_deep_house_cover():
    img = Image.new("RGBA", (500, 500), (0, 0, 0))
    red_part = draw_strip_square((300, 200), (255, 0, 0))
    white_part = draw_strip_square((300, 200), (255, 255, 255))
    img.alpha_composite(red_part, (40, 100))
    img.alpha_composite(white_part, (150, 140))
    return img


def draw_uk_bass_cover():
    img = Image.new("RGBA", (500, 500), (0, 0, 0))
    ring = draw_ring(400, 150, (255, 0, 0))
    img.alpha_composite(ring)
    return img


draw_house_cover().show()
draw_deep_house_cover().show()
draw_uk_bass_cover().show()
