#!/usr/bin/env bash

device_name=tap0

if ip link show $device_name; then
    exit 0
fi
sudo ip tuntap add $device_name mode tap
sudo ip addr add 172.16.0.1/30 dev $device_name
sudo ip link set $device_name up
