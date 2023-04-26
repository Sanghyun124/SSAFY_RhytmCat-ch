obj-m := mpu6050.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	gcc -o user_app user_app.c

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -f user_app
