obj-m += mpu.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	make -C $(KERNELDIR) M=$(PWD) modules
	gcc mpu6050_read.c -o mpu6050_read -lwiringPi
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
	rm -f mpu6050_read

