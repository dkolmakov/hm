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

All commands executed in the specified directory are combined together and placed at the beginning of the bash built-in history, so it is possible to walk through them using <kbd>&#8593;</kbd> button.

To include commands executed in the nested folders add `-R` flag: 

```Shell
hm -d . -R
```

**TODO:** It is possible to filter out commnads containing specified words:
```Shell
hm -d . --exclude "cd ls git"
```

### Terminal session command history recovery

**TODO:** To set a terminal session name and to recover the command history related to this name run the following:
```Shell
hm -s "Session Name"
```

**TODO:** To synchronize history with the database:
```Shell
hm -s
```

**TODO:** It is possible to filter out commnads containing specified words the same way as in per-directory history recovery.


### Using both criteria

**TODO:** All criteria described above can be combined together:
```Shell
hm -s "Session Name" -d . --exclude "cd ls git"
```
