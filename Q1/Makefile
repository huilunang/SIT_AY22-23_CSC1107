# compile and insert loadable kernel module into Linux kernel
obj-m += CSC1107_22_kernel.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean