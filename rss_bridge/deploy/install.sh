#!/usr/bin/env bash
set -x

echo "Install Python"
sudo apt install -y python3 python3-venv python3-pip
python3 -m venv venv
source venv/bin/activate
python3 -m pip install -r requirements.txt -i https://pypi.tuna.tsinghua.edu.cn/simple

echo "Install systemd service"
SED_EXPR="s./path/to.${PWD}.g"
sed "$SED_EXPR" deploy/aria2.service | sudo tee /etc/systemd/system/aria2.service
sed "$SED_EXPR" deploy/rss_bridge.service | sudo tee /etc/systemd/system/rss_bridge.service
sudo systemctl daemon-reload

echo "You should edit aria2.conf and start service manually"
