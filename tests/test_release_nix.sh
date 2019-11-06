#!/usr/bin/env bash

version=$1

if ! command -v nix-build >/dev/null
then
    echo "Nix package manager is required for this test!"
    exit 1
fi

wget https://github.com/dkolmakov/hm/releases/download/v${version}/hm-${version}.nix
nix-env -i -f hm-${version}.nix
