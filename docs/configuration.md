
## Configuration

### Adding `hm` scripts to the Bash configuration (python package)

Configuration of the history manager is generated with python script when installed with `pip`:

```Shell
hm-init
```
To enable `hm` in the current session execute:

```Shell
. ~/.hm/configuration
```

And modify `~/.bashrc` to enable `hm` by default in future sessions:

```Shell
echo ". ~/.hm/configuration" >> ~/.bashrc
```

### Adding `hm` scripts to the Bash configuration (installed from sources)

Call `hm-db` directly:

```Shell
hm-db configure
```
which will output everything to the terminal or to a specified file, for example:

```Shell
hm-db configure ~/.bashrc
```

If it's not convenient to use `~/.bashrc` directly the configuration scripts can be placed to an arbitrary file:

```Shell
hm-db configure <filename>
```
and add the following line to the `~/.bashrc`:

```Shell
. <filename>
```

### Home directory for history manager

By default it is `~/.hm`. Other home path can be specified at configuration step:

```Shell
hm-init --home /custom/hm/home/path
```
