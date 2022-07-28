
## Configuration

### Adding `hm` scripts to the Bash configuration

Configuration of the history manager is performed by the following command:

```Shell
hm-init
```
or for the installation from sources:

```Shell
hm-db configure
```
This command will output everything to the terminal. To add the content to `~/.bashrc` run:

```Shell
hm-init ~/.bashrc
```
or

```Shell
hm-db configure ~/.bashrc
```

If it's not convenient to use `~/.bashrc` directly the configuration scripts can be placed to a separate file `~/.hm/configuration`:

```Shell
hm-init ~/.hm/configuration
```
and add the following line to the `~/.bashrc`:

```Shell
[[ -f ~/.hm/configuration ]] && . ~/.hm/configuration
```

### Directory to store `hm` database

History manager home directory is `~/.hm` by default. Other home path can be specified at configuration step:

```Shell
hm-init ~/.bashrc --home /custom/hm/home/path
```

### Applying changes to the current session

Simply source the `.bashrc`:

```Shell
. ~/.bashrc
```

