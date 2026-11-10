#!/bin/bash

mkdir -p ~/.winc
cp -v -r ./* ~/.winc/
rm ~/.winc/install.sh
rm -rf ~/.winc/obj_std