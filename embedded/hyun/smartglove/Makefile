obj-m += smartglove.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)



all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	gcc smartglove_read.c -o smartglove_read -lwiringPi

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -f smartglove_read
