from time import sleep
from subprocess import Popen
from tomllib import load
from dataclasses import dataclass
from pathlib import Path

from httpx import HTTPTransport, Client


@dataclass
class Config:
    SOCKET: str
    KERNEL: str
    ROOTFS: str
    INITRD: str | None
    VCPU: int
    MEM: int


def load_config(config_path: str) -> Config:
    with open(config_path, "rb") as f:
        data = load(f)
    required = ("KERNEL", "ROOTFS")
    missing = [key for key in required if not data.get(key)]
    if missing:
        raise ValueError(f"Missing required config item(s): {', '.join(missing)}")
    if config_initrd := data.get("INITRD"):
        initrd = str(Path(config_initrd).resolve())
    else:
        initrd = None
    return Config(
        SOCKET=data.get("SOCKET", "/tmp/firecracker.socket"),
        KERNEL=str(Path(data["KERNEL"]).resolve()),
        ROOTFS=str(Path(data["ROOTFS"]).resolve()),
        INITRD=initrd,
        VCPU=int(data.get("VCPU", 2)),
        MEM=int(data.get("MEM", 1024)),
    )


def run_firecracker():
    global firecracker_pid
    Path(config.SOCKET).unlink(missing_ok=True)
    proc = Popen(["./firecracker", "--api-sock", config.SOCKET, "--enable-pci"])
    firecracker_pid = proc.pid
    while not Path(config.SOCKET).exists():
        sleep(1)


def create_vm(tap: bool = False):
    client.put(
        "http://localhost/machine-config",
        json={"vcpu_count": config.VCPU, "mem_size_mib": config.MEM, "smt": False},
    )
    boot_source = {
        "kernel_image_path": config.KERNEL,
        "boot_args": "console=ttyS0 reboot=k panic=1",
    }
    if config.INITRD:
        boot_source["initrd_path"] = config.INITRD
    client.put("http://localhost/boot-source", json=boot_source)
    client.put(
        "http://localhost/drives/rootfs",
        json={
            "drive_id": "rootfs",
            "path_on_host": config.ROOTFS,
            "is_root_device": True,
            "is_read_only": False,
        },
    )
    if tap:
        client.put(
            "http://localhost/network-interfaces/eth0",
            json={
                "iface_id": "eth0",
                "host_dev_name": "tap0",
                "guest_mac": "AA:FC:00:00:00:01",
            },
        )


def start_vm():
    client.put("http://localhost/actions", json={"action_type": "InstanceStart"})


def main():
    # run_firecracker()
    create_vm(True)
    start_vm()


config = load_config("config.toml")
client = Client(transport=HTTPTransport(uds=config.SOCKET))
firecracker_pid = -1
main()
