#! /bin/bash

if [[ "$OSTYPE" == "linux-gnu" ]]; then
	sudo apt-get -y install x11-xserver-utils
	sudo apt-get -y install libxrandr-dev
	sudo apt-get -y install libxinerama-dev
	sudo apt-get -y install libxcursor-dev
	sudo apt-get -y install libxi-dev
	sudo apt-get -y install libxkbcommon-dev
	sudo apt-get -y install libgl-dev
fi
