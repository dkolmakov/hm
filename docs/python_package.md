# hm
Command line history manager for bash (distributed as a python package). 

## About
History manager provides an alternative command line history storage with the ability to select a subset of commands and load it into the current Bash session so the <kbd>Ctrl</kbd><kbd>r</kbd> search is performed on the selected subset. It has an [SQLite](https://www.sqlite.org) database under the hood and, besides the executed command line, stores time, working directory, returned code and a unique terminal session identifier. All additional information may be used as a selection criterion.

## Motivation

There are two main usage scenarios:

### Recover history

It is a quite common issue when you need to repeat some actions with a previously developed project but don't remember exact commands you have used for it. History manager provides a way to recover command history related to the specific directory, particular session or both.

### Synchronize history

The recovery of a history can be assumed as synchronization between previously existed session and the current one. But it may be also useful to synchronize histories of several simultaneously opened terminal sessions so any executed command can be easily shared. History manager solves this by using the same command selection mechanism as for the history recovery case.

