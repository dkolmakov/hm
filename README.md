![Build Status](https://travis-ci.com/dkolmakov/hm.svg?branch=master)

# hm
Command line history manager for bash. 

History manager has an (SQLite)[https://www.sqlite.org] database under the hood to store executed commands. 
Beside the executed command line it stores time, working directory, returned code and a unique terminal session identifier.
History manager provides a simple mechanism for commands selection using this additional information.

Selection result may be loaded into the current bash session history, so it utilizes the built-in bash history mechanism.

Currently implemented functionality allows to select commands by the directory they were executed in. It allows to recover the history

