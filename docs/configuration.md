
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

History manager home directory is `~/.hm` by default. Other home path can be specified at configuration step:

```Shell
hm-db configure ~/.bashrc --home /custom/hm/home/path
```

### Applying changes to the current session

Simply source the `.bashrc`:

```Shell
. ~/.bashrc
```

