# History manager installation

Install release:
- [Ubuntu/Debian](#ubuntudebian)
- [Nix/NixOS](#nixnixos)
- [From sources](#from-sources)

Install latest version in development: 
- [From dev sources](#from-dev-sources)


## Ubuntu/Debian

1. Download the latest .deb package from [release page](https://github.com/dkolmakov/hm/releases) or with command line:

```Shell
wget https://github.com/dkolmakov/hm/releases/download/v1.0.0/hm-1.0.0-Linux-x86_64.deb
```

2. Install it:

```Shell
sudo dpkg -i hm-${version}-Linux-x86_64.deb
```

## Nix/NixOS

1. Download the latest .nix expression from [release page](https://github.com/dkolmakov/hm/releases) or with command line:

```Shell
wget https://github.com/dkolmakov/hm/releases/download/v1.0.0/hm-1.0.0.nix
```

2. Install it:

```Shell
nix-env -i -f hm-${version}.nix
```

## From sources

1. Download the latest .tar.gz expression from [release page](https://github.com/dkolmakov/hm/releases) or with command line:

```Shell
wget https://github.com/dkolmakov/hm/archive/v1.0.0.tar.gz
```

2. Unpack it:

```Shell
tar xvf v${version}.tar.gz
```

3. Build and install it:

```Shell
cd hm-${version}
mkdir build
cd build
cmake ..
make install
```

4. Run system tests to verify the installation (tests are built with [pytest](https://docs.pytest.org)):

```Shell
py.test
```

## From dev sources

1. Use `git` to clone the repository:

```Shell
git clone https://github.com/dkolmakov/hm.git
```

2. Build and install the project:

```Shell
cd hm
mkdir build
cd build
cmake ..
make install
```

3. Run system tests to verify the installation (tests are built with [pytest](https://docs.pytest.org)):

```Shell
py.test
```

