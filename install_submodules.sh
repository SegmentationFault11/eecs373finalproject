#! /bin/bash

printf "Making submodules ...\n"
cd submodules/cppsockets
make install

printf "\nMoving submodules into their appropriate location ...\n"
mv ./cppsockets.a ../../serialreader/
