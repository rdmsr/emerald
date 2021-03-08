#!/bin/sh

compiler=${1:-'clang'}
reconfigure=${2:-'setup'}

echo -e 'Installing limine & echfs-utils...'
make -C ../../thirdparty/limine limine-install
make -C ../../thirdparty/echfs echfs-utils
echo -e '\033[1;32mDone! \033[1;0m' 

if [ $reconfigure == 'setup' ] 
then
	CC=$compiler meson setup ../../build ../../kernel
else
	rm -rf ../../build && CC=$compiler meson setup ../../build ../../kernel
fi

echo -e '\033[1;32mFinished configuring ninja for your system! \033[1;0m' 
