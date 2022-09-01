![Build Status](https://github.com/dkolmakov/hm/actions/workflows/c-cpp.yml/badge.svg)

# hm
Command line history manager for bash. 

## About
History manager provides an alternative command line history storage with the ability to select a subset of commands and load it into the current Bash session so the <kbd>Ctrl</kbd><kbd>r</kbd> search is performed on the selected subset. It has an [SQLite](https://www.sqlite.org) database under the hood and, besides the executed command line, stores time, working directory, returned code and a unique terminal session identifier. All additional information may be used as a selection criterion.

## Motivation

There are two main usage scenarios:

### Recover history

It is a quite common issue when you need to repeat some actions with a previously developed project but don't remember exact commands you have used for it. History manager provides a way to recover command history related to the specific directory, particular session or both.

### Synchronize history

The recovery of a history can be assumed as synchronization between previously existed session and the current one. But it may be also useful to synchronize histories of several simultaneously opened terminal sessions so any executed command can be easily shared. History manager solves this by using the same command selection mechanism as for the history recovery case.

## Installation

History manager is available as a python package:

```Shell
pip install history-manager
```

For other installation options please follow the [link](docs/installation.md)

## Configuration

At the configuration step the history manager adds some wrapper scripts to the `.bashrc`:

```Shell
hm-init ~/.bashrc
```
To enable `hm` in the current session source the `.bashrc`:

```Shell
. ~/.bashrc
```

More details are [here](docs/configuration.md).

## Usage

### Per-directory history recovery

Recovering command history related to the specific directory is performed with:

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


