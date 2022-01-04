#!/bin/bash

if [ ! -f ./obj/ ];then
	mkdir obj
fi

make
sudo cp main /usr/bin/projetc
