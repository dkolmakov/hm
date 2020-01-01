
## Configuration

### Adding `hm` scripts to the Bash configuration

Configuration of the history manager is performed by the following command:

```Shell
hm-db configure ~/.bashrc
```

To check what will be added to the `~/.bashrc` run:

```Shell
hm-db configure
```

If it's not convenient to use `~/.bashrc` directly the configuration scripts can be placed to a separate file `~/.hm/configuration`:

```Shell
hm-db configure ~/.hm/configuration
```
and add the following line to the `~/.bashrc`:

```Shell
[[ -f ~/.hm/configuration ]] && . ~/.hm/configuration
```

### Directory to store `hm` database

History manager home directory is `~/.hm` by default. Other home path can be specified at configuration step:

```Shell
hm-db configure ~/.bashrc --home /custom/hm/home/path
```
### Applying changes to the current session

Simply source the `.bashrc`:

```Shell
. ~/.bashrc
```

