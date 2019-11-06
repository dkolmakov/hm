#!/usr/bin/env bash

version=$1

if ! command -v dpkg >/dev/null
then
    echo "Debian package manager is required for this test!"
    exit 1
fi

wget https://github.com/dkolmakov/hm/releases/download/v${version}/hm-${version}-Linux-x86_64.deb
sudo dpkg -i hm-${version}-Linux-x86_64.deb
