OUTPUT = userspace

obj-m := main.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
	gcc -o $(OUTPUT) $(OUTPUT).c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
	rm userspace

