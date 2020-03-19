obj-m += lkm_sandbox.o lkm_device.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test:
	$(info Testing Module '$(module)' by loading and displaying Kernel Message Ring Buffer...)
	$(info Root permissions are needed for clearing buffer with dmesg and loading/unloading with insmod/rmmod)
	@sudo dmesg --clear
	@sudo insmod $(module).ko
	@sudo rmmod $(module).ko
	@dmesg
