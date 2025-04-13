from pyinfra.operations import files, server, systemd
from pyinfra.context import config, host

config.SUDO = True
config.SUDO_PASSWORD = host.data.get("sudo_pwd")

files.download(
    name="Download release",
    src="https://github.com/ekzhang/bore/releases/download/v0.5.2/bore-v0.5.2-x86_64-unknown-linux-musl.tar.gz",
    dest="/tmp/bore.tar.gz",
)
files.directory(
    name="Create dir",
    path="/opt/bore"
)
server.shell(
    name="Extract file",
    commands=["tar xf /tmp/bore.tar.gz -C /opt/bore"],
)
files.put(
    name="Add systemd config",
    src="config/bore/bore.service",
    dest="/etc/systemd/system/bore.service",
)
systemd.daemon_reload()
systemd.service(
    name="Start service",
    service="bore.service",
    running=True,
    enabled=True,
)