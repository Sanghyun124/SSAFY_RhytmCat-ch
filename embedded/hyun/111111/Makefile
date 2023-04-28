obj-m += mpu.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	make -C $(KERNELDIR) M=$(PWD) modules
	gcc -o mpu6050_read mpu6050_read.c
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
	rm -f mpu6050_read

