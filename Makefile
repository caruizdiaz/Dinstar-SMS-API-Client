# Makefile to compile dwgclient
#
# Author: Carlos Ruiz Diaz
#         caruizdiaz.com
#

all: 	
	gcc main.c -ldwgsms.pub -lpthread -o dwgclient
clean:
	rm -rf dwgclient
