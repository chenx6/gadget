from pyinfra.operations import apt, files, systemd
from pyinfra.context import config, host

config.SUDO = True
config.SUDO_PASSWORD = host.data.get("ssh_password")

apt.packages(
    name="Install caddy",
    packages=["caddy"],
)
files.template(
    name="Add config",
    src="config/caddy/Caddyfile.j2",
    dest="/etc/caddy/Caddyfile",
    config=host.data.get("caddy_config")
)
systemd.service(
    name="Restart caddy",
    service="caddy.service",
    running=True,
    restarted=True,
)
