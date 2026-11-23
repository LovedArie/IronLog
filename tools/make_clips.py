"""Builds one short clip per exercise from its two pictures (ADR-025).

Frames (720x960): start pose, end pose, start, end, muscle diagram.
The how-to picture is split in half; the purple arrow between the poses is
painted out. ffmpeg fades between the still frames.
Usage: python3 tools/make_clips.py assets/exercises assets/exercises/clips [id ...]
Needs: Python 3 + Pillow, ffmpeg, the Inter font (paths in FONT / FONT_REG).
"""
import json, subprocess, sys
from pathlib import Path
from PIL import Image, ImageDraw, ImageFont

W, H = 720, 960
FONT = "/usr/share/fonts/opentype/inter/Inter-Bold.otf"
FONT_REG = "/usr/share/fonts/opentype/inter/Inter-Regular.otf"
INK, MUTED, RED, ORANGE = (27, 29, 33), (74, 78, 87), (47, 91, 208), (143, 170, 240)   # main / helper: design-system blue (dark / light)

# id: (start label, end label, caption under the end frame)
STEPS = {
    "boxsquat": ("Stand tall", "Sit back, touch the box", "Down 2 s · Up 1 s"),
    "goblet":   ("Stand tall", "Squat down, chest up", "Down 2 s · Up 1 s"),
    "wallsit":  ("Back on the wall", "Hold: thighs level", "Hold and breathe"),
    "dbrow":    ("Arm straight", "Pull to your hip", "Pull 1 s · Lower 2 s"),
    "ringrow":  ("Arms straight", "Chest to your hands", "Pull 1 s · Lower 2 s"),
    "bandrow":  ("Arms forward", "Pull to your ribs", "Pull 1 s · Return 2 s"),
    "cablerow": ("Reach forward", "Pull to your belly", "Pull 1 s · Return 2 s"),
    "dbbench":  ("Arms up", "Lower to your chest", "Down 2 s · Press 1 s"),
    "pushup":   ("Top: body straight", "Chest near the floor", "Down 1.5 s · Up 1 s"),
    "revlunge": ("Stand tall", "Step back, knee down", "Down 1.5 s · Up 1 s"),
    "stepup":   ("One foot on the box", "Stand on top", "Up 1 s · Down 1.5 s"),
    "plank":    ("Set up on your knees", "Hold: straight line", "Hold and breathe"),
    "deadbug":  ("Arms up, knees up", "Reach opposite arm + leg", "Reach 2 s · Back 1.5 s"),
    "catcow":   ("Cat: round up", "Cow: let it sag", "Move with your breath"),
    "hipflex":  ("Start", "Shift forward, feel the stretch", "Hold 30 s each side"),
    "walk":     ("Step", "Step", "Easy pace: you can still talk"),
    "legpress": ("Legs long, knees soft", "Lower to about 90°", "Down 2 s · Press 1 s"),
}


def font(size, bold=True):
    return ImageFont.truetype(FONT if bold else FONT_REG, size)


def remove_arrow(im):
    """Paint the purple double arrow in the middle band with the background colour,
    including its soft (anti-aliased) edges: mask bluish pixels, then grow the mask."""
    from PIL import ImageFilter
    bg = im.getpixel((8, 8))
    w, h = im.size
    x0, x1, y0, y1 = int(w * 0.34), int(w * 0.66), int(h * 0.10), int(h * 0.62)
    mask = Image.new("L", im.size, 0)
    px, mp = im.load(), mask.load()
    for y in range(y0, y1):
        for x in range(x0, x1):
            r, g, b = px[x, y][:3]
            if b > 150 and b - r > 20 and b - g > 15:      # bluish: arrow or its edge; navy shorts are darker
                mp[x, y] = 255
    mask = mask.filter(ImageFilter.MaxFilter(7))
    im.paste(Image.new("RGB", im.size, bg), (0, 0), mask)
    return im


def split_column(im):
    """The column near the middle with the least drawing in it, so neither pose is cut."""
    bg = im.getpixel((8, 8)); px = im.load(); w, h = im.size
    best, best_x = None, w // 2
    for x in range(int(w * 0.38), int(w * 0.62)):
        ink = sum(1 for y in range(int(h * 0.08), int(h * 0.96), 2) if sum(abs(a - b) for a, b in zip(px[x, y][:3], bg)) > 60)
        score = ink * 1000 + abs(x - w // 2)
        if best is None or score < best:
            best, best_x = score, x
    return best_x


def text_center(d, y, s, f, fill):
    w = d.textlength(s, font=f)
    d.text(((W - w) / 2, y), s, font=f, fill=fill)


def frame(title, img, label, sub, step=None):
    c = Image.new("RGB", (W, H), (250, 250, 247))
    d = ImageDraw.Draw(c)
    text_center(d, 28, title, font(44), INK)
    if img is not None:
        box_w, box_h = W - 40, 680
        s = min(box_w / img.width, box_h / img.height)
        im = img.resize((round(img.width * s), round(img.height * s)), Image.LANCZOS)
        c.paste(im, ((W - im.width) // 2, 100 + (box_h - im.height) // 2))
    if step:
        d.rounded_rectangle((W / 2 - 30, 792, W / 2 + 30, 836), radius=22, fill=INK)
        text_center(d, 796, step, font(30), (255, 255, 255))
    text_center(d, 848, label, font(40), INK)
    text_center(d, 902, sub, font(28, False), MUTED)
    return c


def muscles_frame(title, img, main, helps):
    c = Image.new("RGB", (W, H), (250, 250, 247))
    d = ImageDraw.Draw(c)
    text_center(d, 28, title, font(44), INK)
    if img is not None:
        im = img.resize((W, round(img.height * W / img.width)), Image.LANCZOS)
        c.paste(im, (0, 120))
    else:
        text_center(d, 380, "Muscle diagram coming soon", font(32, False), MUTED)
    y = 640
    for dot, head, names in ((RED, "Main", main), (ORANGE, "Helps", helps)):
        d.ellipse((60, y + 10, 92, y + 42), fill=dot)
        d.text((110, y), f"{head}:", font=font(40), fill=INK)
        d.text((110, y + 52), ", ".join(n.capitalize() for n in names), font=font(32, False), fill=MUTED)
        y += 130
    return c


def build(ex, src, out, tmp):
    eid, name = ex["id"], ex["name"]
    start_lbl, end_lbl, tempo = STEPS[eid]
    how = remove_arrow(Image.open(src / ex["how"]).convert("RGB"))
    half = split_column(how)
    left, right = how.crop((0, 0, half, how.height)), how.crop((half, 0, how.width, how.height))
    mus = Image.open(src / ex["muscles"]).convert("RGB") if ex.get("muscles") else None
    frames = [frame(name, left, start_lbl, tempo, "1"), frame(name, right, end_lbl, tempo, "2"), muscles_frame(name, mus, ex["main"], ex["helpers"])]
    paths = []
    for i, f in enumerate(frames):
        p = tmp / f"{eid}-{i}.png"; f.save(p); paths.append(p)
    # sequence: start, end, start, end, muscles (walk flips faster)
    fast = eid == "walk"
    seq = [(0, 0.9 if fast else 1.3), (1, 0.9 if fast else 1.6), (0, 0.9 if fast else 1.3), (1, 0.9 if fast else 1.6)]
    if fast:
        seq += [(0, 0.9), (1, 0.9)]
    seq += [(2, 3.2)]
    fade = 0.25 if fast else 0.4
    args = ["ffmpeg", "-y", "-loglevel", "error"]
    for idx, dur in seq:
        args += ["-loop", "1", "-framerate", "30", "-t", f"{dur}", "-i", str(paths[idx])]
    chain, prev, offset = [], "[0:v]", 0.0
    for k in range(1, len(seq)):
        offset += seq[k - 1][1] - fade
        tag = f"[x{k}]"
        chain.append(f"{prev}[{k}:v]xfade=transition=fade:duration={fade}:offset={offset:.2f}{tag}")
        prev = tag
    chain.append(f"{prev}format=yuv420p[out]")
    tmpout = out / f"tmp_ironlog-{eid}-clip.mp4"
    args += ["-filter_complex", ";".join(chain), "-map", "[out]", "-r", "30", "-c:v", "libx264", "-crf", "24",
             "-preset", "veryfast", "-pix_fmt", "yuv420p", "-movflags", "+faststart", str(tmpout)]
    subprocess.run(args, check=True)
    final = out / f"ironlog-{eid}-clip.mp4"
    tmpout.rename(final)
    return final


if __name__ == "__main__":
    src, out = Path(sys.argv[1]), Path(sys.argv[2])
    out.mkdir(parents=True, exist_ok=True)
    tmp = Path("/tmp/clips/frames"); tmp.mkdir(parents=True, exist_ok=True)
    data = json.loads((src / "exercises.json").read_text())["exercises"]
    only = set(sys.argv[3:])
    for ex in data:
        if only and ex["id"] not in only:
            continue
        f = build(ex, src, out, tmp)
        print(f.name, f"{f.stat().st_size // 1024} KB")
