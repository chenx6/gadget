from pyinfra.operations import apt, files, systemd, server
from pyinfra.context import config, host

config.SUDO = True
config.SUDO_PASSWORD = host.data.get("ssh_password")

aria2_config = host.data.get("aria2_config")

apt.packages(
    name="Install aria2 program",
    packages=["aria2"],
    update=True,
)
files.template(
    name="Add config",
    src="config/aria2/aria2.conf.j2",
    dest="/etc/aria2/aria2.conf",
    config=aria2_config,
)
server.group(
    name="Create group",
    group="aria2",
)
server.user(
    name="Create user",
    user="aria2",
    group="aria2",
)
files.directory(
    name="Create download dir",
    path=aria2_config["download_dir"],  # type: ignore
)
files.file(
    name="Create aria2 session",
    path=aria2_config["download_dir"] + "/aria2.session",  # type: ignore
    user="aria2",
    group="aria2",
    touch=True,
)
files.put(
    name="Add systemd config",
    src="config/aria2/aria2.service",
    dest="/etc/systemd/system/aria2.service",
)
systemd.daemon_reload()
systemd.service(
    name="Start service",
    service="aria2.service",
    running=True,
    enabled=True,
)
