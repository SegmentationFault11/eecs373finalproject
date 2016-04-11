#! /bin/bash

printf "Making submodules ...\n"
cd cppsockets
make install

printf "\nMoving submodules into their appropriate location ...\n"
mv ./cppsockets.a ../SerialReader/
