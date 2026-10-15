from argparse import ArgumentParser
from fcntl import ioctl
from os import O_RDONLY, read
from os import open as os_open
from pathlib import Path

HIDIOCSFEATURE = lambda n: (3 << 30) | (n << 16) | (0x48 << 8) | 0x06


def find_dev(vid: int, pid: int):
    for folder in Path("/sys/class/hidraw/").iterdir():
        uevent = folder / "device/uevent"
        if f"HID_ID=0003:{vid:08X}:{pid:08X}" in uevent.read_text():
            return "/dev/" + folder.name


def write_report5(fd, buf: bytearray):
    assert len(buf) == 13 and buf[0] == 5
    n = ioctl(fd, HIDIOCSFEATURE(13), buf, True)
    return n


def monitor_battery(p: str):
    f = os_open(p, O_RDONLY)
    data = read(f, 64)
    if len(data) < 5:
        print(f"Data length {len(data)} < 5")
        return
    if data[0] != 0x03:
        print("Report ID not matched", data)
        return
    sub = data[1]
    group = data[2]
    charging = bool(data[3])
    level = int(data[4])
    if group != 0x40:
        print("Group not matched", data)
        return
    print(f"Battery status: {level * 10}%, Charging: {charging}")


def main():
    path = find_dev(0x1D57, 0xFA60)
    if not path:
        print("Device not found")
        return
    parser = ArgumentParser()
    parser.add_argument("-p", "--profile", type=int, default=1)
    parser.add_argument("-m", "--mode", type=int, default=1)
    parser.add_argument("-c", "--color", default="FF 48 B0")
    parser.add_argument("-d", "--duration", type=int, default=4)
    parser.add_argument("-b", "--brightness", type=int, default=8)
    parser.add_argument("--battery", action="store_true")
    args = parser.parse_args()
    if args.battery:
        monitor_battery(path)
        return
    profile = int(args.profile)
    mode = int(args.mode)
    color = bytes.fromhex(args.color)
    duration = int(args.duration)
    brightness = int(args.brightness)
    tmpl = bytearray.fromhex("05 0D 01 02 04 08 FF 48 B0 06 01 00 00")
    tmpl[2] = profile
    tmpl[3] = mode
    tmpl[4] = duration
    tmpl[5] = brightness
    tmpl[6:9] = color
    print("Writing", tmpl, "to", path)
    with open(path, "rb+", buffering=0) as fd:
        n = write_report5(fd, tmpl)
        print(f"Write {n} bytes")


if __name__ == "__main__":
    main()
