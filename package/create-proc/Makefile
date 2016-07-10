ifeq ($(KERNELRELEASE),)

KERNELDIR ?= /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) #modules

#modules_install:
#	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c *.order *.symvers .*.swp .tmp_versions 

.PHONY: modules modules_install clean

else
	obj-m := hello_proc.o world_proc.o read_write_proc.o 
endif
