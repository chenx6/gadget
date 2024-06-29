from subprocess import run, DEVNULL
from argparse import ArgumentParser


def gen_thumb(file: str, second: int | float = 30, duration: str = "05:00"):
    """
    https://superuser.com/questions/135117/how-to-extract-one-frame-of-a-video-every-n-seconds-to-an-image
    https://www.cnblogs.com/architectforest/p/17060422.html
    """
    name = file.split("/")[-1]
    p = run(
        [
            "ffmpeg",
            "-ss",
            "00:00",
            "-i",
            file,
            "-r",
            f"{1/second:.02f}",
            "-t",
            duration,
            "-f",
            "image2",
            "output%d.jpg",
        ],
        stdout=DEVNULL,
        stderr=DEVNULL,
    )
    retcode = p.returncode
    if retcode != 0:
        print(f"ffmpeg convert failed, {retcode}")
        return retcode
    run(f"convert -append output*.jpg '{name}'.jpg", shell=True)
    run("rm output*.jpg", shell=True)
    print(f"{name} => {name}.jpg")


def main():
    parser = ArgumentParser()
    parser.add_argument("-f", "--file")
    parser.add_argument("-b", "--batch")
    args = parser.parse_args()
    if args.file:
        files = [args.file]
    elif args.batch:
        with open(args.batch, "r") as f:
            files = [i.strip() for i in f.readlines()]
    else:
        parser.print_usage()
        exit(-1)
    for file in files:
        gen_thumb(file)


main()
