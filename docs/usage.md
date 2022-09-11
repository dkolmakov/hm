# Usage

History recovery/sychronization:
- [By path](#by-path)
- [By session name](#by-session-name)
- [Using both criteria](#using-both-criteria)

Usage with other tools: 
- [hstr](#hstr)

## History recovery/synchronization 
### By path

Recovering command history related to the specific directory is done with:

```Shell
hm /path/of/interest
```
or for the current working directory:

```Shell
hm .
```

All commands executed in the specified directory are combined together and placed at the beginning of the bash built-in history, so it is possible to walk through them using <kbd>&#8593;</kbd> button.

To include commands executed in the nested folders add `-R` flag: 

```Shell
hm . -R
```

**TODO:** It is possible to filter out commnads containing specified words:
```Shell
hm . --exclude "cd ls git"
```

### By session name 

To set a terminal session name and to recover the command history related to this name run the following:
```Shell
hm -s "Session Name"
```

To synchronize history with the database using the previously given name:
```Shell
hm -s
```

To query the previously given name:
```Shell
hm -i
```

**TODO:** It is possible to filter out commnads containing specified words the same way as in per-directory history recovery.


### Using both criteria

Both path and session name can be combined together in a single recovery request:
```Shell
hm /some/path -s "Session Name"
```
## Usage with other tools

### hstr

History manager is completely compatible with [**hstr**](https://github.com/dvorka/hstr) - a history suggest box which allows to easily view, navigate and search your command history. 
