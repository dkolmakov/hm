![Build Status](https://travis-ci.com/dkolmakov/hm.svg?branch=master)

# hm
Command line history manager for bash. 

## About
History manager provides an alternative command line history storage with ability to select a subset of commands and load it into the current Bash session history so the <ctrl-r> search is performed on the selcted subset. It has an [SQLite](https://www.sqlite.org) database under the hood and, beside the executed command line, stores time, working directory, returned code and a unique terminal session identifier. All additional information may be used as a selection criteria.

### Current status

Currently implemented functionality allows to load commands by the directory they were executed in. By default it selects commnads recursively down by the directory hierarchy. 

### Planned features

- Selection by terminal session identifier which will in other words enable restoring of closed terminal session.
- Creation of labeled sessions to store spesific command history, for example, a seaprate session for administrating the computer or special sessions to manage different subsystems.
- Default commnads set loaded at terminal session creation.
- UI to simplify and accomodate commands selection and history management. 

## Installation

Requires cmake 

## Usage


