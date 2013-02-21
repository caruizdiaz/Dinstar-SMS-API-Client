# Makefile to compile dwgclient
#
# Author: Carlos Ruiz Diaz
#         caruizdiaz.com
#

all: 	
	gcc main.c -ldwgsms.pub -lpthread -o sendussd -g
	gcc sendsms.c -ldwgsms.pub -lpthread -o sendsms -g
clean:
	rm -rf sendsms sendussd
