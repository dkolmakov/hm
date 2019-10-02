![Build Status](https://travis-ci.com/dkolmakov/hm.svg?branch=master)

# hm
Command line history manager for bash. 

## About
History manager provides an alternative command line history storage with the ability to select a subset of commands and load it into the current Bash session history so the <kbd>Ctrl</kbd><kbd>r</kbd> search is performed on the selected subset. It has an [SQLite](https://www.sqlite.org) database under the hood and, besides the executed command line, stores time, working directory, returned code and a unique terminal session identifier. All additional information may be used as a selection criterion.

### Current status

Currently implemented functionality allows to search for commands by the directory they were executed in and load them to the bash history.

### Planned features

- Selection by terminal session identifier which will, in other words, enable restoring of closed terminal session.
- Creation of labeled sessions to store specific command history, for example, a separate session for administrating the computer or special sessions to manage different subsystems.
- Default commands set loaded at terminal session creation.
- UI to simplify and accommodate commands selection and history management. 

## Installation

Installation consists of two steps: building the `hm-db` binary and `.bashrc` modification.

1. Use `git` to clone the repository:

```Shell
git clone https://github.com/dkolmakov/hm.git
cd hm
```

2. Build the project with `cmake`:

```Shell
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/path/to/installation/directory ..
make install
```

Be sure that the installation directory is in your `PATH`.

3. Add `hm` wrapper scripts to the `.bashrc`:

```Shell
./install.sh
```

4. To enable `hm` in the current session source the `.bashrc`:

```Shell
. ~/.bashrc
```

5. Run system tests to verify the installation (tests are built with [pytest](https://docs.pytest.org)):

```Shell
py.test
```

## Usage

There are several main usage scenarios are kept in mind during the development.

### Per directory history recovery

It is a quite common issue when you return to some project development after a long break and don't remember exact commands you have used to build it, run tests or tune an environment for it. If these commands are still in the history file it is possible to find them, but it is not very convenient to check all the neighboring commands you have used because of the following `bash` history drawbacks: 
- it may require several <kbd>Ctrl</kbd><kbd>r</kbd> consequent searches to get to the right place in the history,
- commands related to the project may be interleaved by commands from other terminal sessions,
- some commands may be lost if terminal session was crushed.

History manager provides a way to recover command history related to the specified directory:

```Shell
hm -d /path/of/interest
```
or for current working directory:

```Shell
hm -d $(pwd)
```
After this all commands executed in the specified directory are combined together and placed at the beginning of the bash built-in history, so it is possible to walk through them using <kbd>&#8593;</kbd> button.

To include commands executed in nested folders recursively add `-R` flag: 
```Shell
hm -d /path/of/interest -R
```

**TODO:** It is possible to filter out commnads containing specified words:
```Shell
hm --exclude "cd ls git"
```

### Terminal session command history recovery

Sometimes it is convenient to group commands used for a specific purpose, for example, commands related to the administration of a particular subsystem like slurm, docker, jenkins, .etc. History manager allows to name the terminal sessions and to perform commands selection with this name.

**TODO:** To set a terminal session name if it doesn't exist or recover command history otherwise:
```Shell
hm -s "Session Name"
```
The same name can be used by several terminal sessions opened at the same time to share the common command history.
