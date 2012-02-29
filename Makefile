
obj-m := showPCB.o
#KERNELDIR =/usr/src/linux-headers-$(shell uname -r)
KERNELDIR = /lib/modules/$(shell uname -r)/build 

PWD :=$(shell pwd)
default:
		$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm *.mod.c *.o *.order *.symvers *.ko