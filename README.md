![Build Status](https://travis-ci.com/dkolmakov/hm.svg?branch=master)

# hm
Command line history manager for bash. 

## About
History manager provides an alternative command line history storage with the ability to select a subset of commands and load it into the current Bash session so the <kbd>Ctrl</kbd><kbd>r</kbd> search is performed on the selected subset. It has an [SQLite](https://www.sqlite.org) database under the hood and, besides the executed command line, stores time, working directory, returned code and a unique terminal session identifier. All additional information may be used as a selection criterion.

## Motivation

There are two main usage scenarios are kept in mind during the development:

### Recover history

It is a quite common issue when you return to some project development after a long break and don't remember exact commands you have used to build it, run tests or tune an environment for it. History manager provides a way to recover command history related to the specific directory, particular session or both.

### Synchronize history

Beside the recovery of old histories it can be useful to synchronize histoties of several opened terminal sessions so the recently executed commands are quickly accessible everywhere. History manager completely satisfies this requirement by using the same command selection mechanism as for the history recovery case. 

## Installation

Install release:
- [Ubuntu/Debian](docs/installation.md#ubuntudebian)
- [Nix/NixOS](docs/installation.md#nixnixos)
- [From sources](docs/installation.md#from-sources)

Install latest version in development: 
- [From dev sources](docs/installation.md#from-dev-sources)

## Configuration

Configuration of the history manager is performed by:

```Shell
hm-db configure ~/.bashrc
```
To enable `hm` in the current session source the `.bashrc`:

```Shell
. ~/.bashrc
```

More details are [here](docs/configuration.md).

## Usage

### Per-directory history recovery

Recovering command history related to the specific directory isdone with:

```Shell
hm -d /path/of/interest
```
or for the current working directory:

```Shell
hm -d
```

### Terminal session command history recovery

To set a terminal session name and to recover the command history related to this name run the following:
```Shell
hm -s "Session Name"
```

To synchronize history with the database using the previously given name:
```Shell
hm -s
```

More usage examples are [here](docs/usage.md).

## Acknowledgements

History manager utilizes the following thirdparty projects:
- [SQLite](https://www.sqlite.org) - SQL database
- [apathy](https://github.com/dlecocq/apathy) - header-only path manipulation library
- [clipp](https://github.com/muellan/clipp) - header-only arguments parsing library


