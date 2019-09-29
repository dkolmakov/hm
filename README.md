![Build Status](https://travis-ci.com/dkolmakov/hm.svg?branch=master)

# hm
Command line history manager for bash. 

## About
History manager provides an alternative command line history storage with the ability to select a subset of commands and load it into the current Bash session history so the <kbd>Ctrl</kbd><kbd>r</kbd> search is performed on the selected subset. It has an [SQLite](https://www.sqlite.org) database under the hood and, besides the executed command line, stores time, working directory, returned code and a unique terminal session identifier. All additional information may be used as a selection criterion.

### Current status

Currently implemented functionality allows to load commands by the directory they were executed in. By default, it selects commands recursively down by the directory hierarchy. 

### Planned features

- Selection by terminal session identifier which will, in other words, enable restoring of closed terminal session.
- Creation of labeled sessions to store specific command history, for example, a separate session for administrating the computer or special sessions to manage different subsystems.
- Default commands set loaded at terminal session creation.
- UI to simplify and accommodate commands selection and history management. 

## Installation

Installation consists of two steps: building the `hm-db` binary and `.bashrc` modification.

1. Use `git` to clone the repository:

```Shell
git clone https://github.com/dkolmakov/hm.gitthe ability
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

### By directory history recovery

It is quite common when bash commands to run  rebuilding working command line from scratch may take a lot of time. 
It is a common issue when you return to some project development after a long break and don't remember how did you run it.  
To recover command history for the current directory:

```Shell
hm
```

