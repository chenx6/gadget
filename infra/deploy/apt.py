from pyinfra.operations import files
from pyinfra.context import config, host

config.SUDO = True
config.SUDO_PASSWORD = host.data.get("ssh_password")

files.replace(
    path="/etc/apt/sources.list",
    text="deb.debian.org",
    replace="mirrors.ustc.edu.cn",
)
