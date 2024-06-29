from subprocess import run
from pathlib import Path


def smartctl_temperature(disk: str):
    proc = run(["smartctl", "-A", disk], capture_output=True)
    for line in proc.stdout.decode().split("\n"):
        if not line.startswith("194"):
            continue
        toks = [ch for ch in line.split(" ") if ch]
        return toks[9]


def sys_temperature(hw_name: str):
    hwmon = Path("/sys/class/hwmon/")
    for name_path in hwmon.glob("*/name"):
        name = name_path.read_text().strip()
        if name != hw_name:
            continue
        total_temp = 0
        core_count = 0
        for input_path in name_path.parent.glob("temp*_input"):
            total_temp += int(input_path.read_text())
            core_count += 1
        return total_temp / core_count / 1000


def main():
    for dev in Path("/dev/").glob("*"):
        dev_s = str(dev)
        dev_name = dev.name
        if dev_name.startswith("sd"):
            if dev_name[-1].isdigit():
                continue
            if temp := smartctl_temperature(dev_s):
                print(f"Disk {dev_name}:", temp)
        if dev_name.startswith("nvme") and len(dev_name) == 5:
            if temp := sys_temperature("nvme"):
                print("NVME:", temp)
    for cpu_snsr in ["k10temp", "pvt"]:
        if temp := sys_temperature(cpu_snsr):
            print(f"CPU sensor {cpu_snsr}:", temp)


if __name__ == "__main__":
    main()