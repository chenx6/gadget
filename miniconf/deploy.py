from tomllib import load
from subprocess import run
from argparse import ArgumentParser
from pathlib import Path


def install_package(packages: list[str]):
    run(["sudo", "apt", "install", "-y", *packages])


def replace_config_path(config_content: str, kv: dict[str, str]) -> str:
    new_config = config_content
    for k, v in kv.items():
        new_config = new_config.replace(f"{{{{{k}}}}}", v)
    return new_config


def apply_config(config: dict, additional: dict[str, str]):
    for prog, conf in config.items():
        conf_path = Path(conf["path"])
        if not conf_path.parent.exists():
            conf_path.parent.mkdir(parents=True)
        with open(conf_path, "w") as f:
            content = conf["content"]
            content = replace_config_path(content, additional)
            f.write(content)


def apply_systemd_config(config: dict):
    for unit_name, unit in config.items():
        with open(f"/etc/systemd/system/{unit_name}.service", "w") as f:
            f.write(unit["content"])


def main():
    parser = ArgumentParser()
    parser.add_argument("-a", "--additional", action="append")
    parser.add_argument("-d", "--dry-run", action="store_true")
    args = parser.parse_args()
    additional = {}
    if addis := args.additional:
        for addi in addis:
            k, v = addi.split("=")
            additional[k] = v
    with open("./config.toml", "rb") as f:
        config = load(f)
        install_package(config["packages"])
        apply_config(config["config"], additional)
