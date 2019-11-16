## Usage

### Per-directory history recovery

Recovering command history related to the specific directory is done with:

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

To set a terminal session name and to recover the command history related to this name run the following:
```Shell
hm -s "Session Name"
```

To synchronize history with the database using the previously given name:
```Shell
hm -s
```

**TODO:** It is possible to filter out commnads containing specified words the same way as in per-directory history recovery.


### Using both criteria

Both path and session name can be combined together in a single recovery request:
```Shell
hm -s "Session Name" -d .
```
