#!/usr/bin/env bash
CC="gcc"

$CC shell.c util.c -o shell -Wall
$CC handler.c util.c -o handler -Wall