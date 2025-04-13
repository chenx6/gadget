from pyinfra.operations import apt, files

apt.packages(
    name="Install wireguard-tools",
    packages=["wireguard"],
)
files.template(
    name="Copy server config",
    src="",
    dest="/etc/wireguard/server.conf",
)
