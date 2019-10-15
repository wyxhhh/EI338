obj-m:=jiffies.o
simplemodule-objs:=module
KDIR:=/lib/modules/$(shell uname -r)/build
MAKE:=make
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

