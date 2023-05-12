obj-m += smartglove.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)



all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	gcc mpu6050_read.c -o mpu6050_read -lwiringPi
	gcc hh.c -o hh 

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -f mpu6050_read
	rm -f hh
