# Linux Kernel Module (LKM) Sandbox
## Overview
The Linux Kernel Module Sandbox is a little module to learn, test and experiment with
the development of Kernel Modules for the Linux System. It won't be anything very usable
and there won't be any new driver emerging from this sandbox (at least it is not intented).
That module is only for learning and study purpose and will contain also heavy comments
explaning things in the source code.

## Building
```
make clean && make
```

## Testing
Testing by loading, unloading and outputing Kernel Ring Buffer (sudo will ask for root permissions).
```
make test module=lkm_device
make test module=lkm_sandbox
make test module=lkm_skeleton
```
Additional tests for sandbox device including loading, creating device and getting messages.
For creating character device the <major> is the major device number created when registering device in the init function. This number is written to the Kernel Ring Buffer.
```
sudo insmod lkm_device.ko
dmesg | grep "Registered sandbox device"
sudo mknod /dev/lkm_device c <major> 0
cat /dev/lkm_device
sudo rmmod lkm_device 
```

## Modules
#|Module|Source|Description
---|---|---|---
1|LKM Sandbox|[lkm_sandbox.c](blob/master/lkm_sandbox.c)|A sandbox module for different experiments
2|LKM Device|[lkm_device.c](blob/master/lkm_device.c)|A module showing how to operate with character devices
3|LKM Skeleton|[lkm_skeleton.c](blob/master/lkm_skeleton.c)|A skeleton module for faster scaffolding little modules

## Notes
"A Linux kernel module is a piece of compiled binary code that is inserted directly into 
the Linux kernel, running at ring 0, the lowest and least protected ring of execution in 
the x86–64 processor."

"Traditional application development paradigms can be largely discarded. Other than loading 
and unloading of your module, you’ll be writing code that responds to system events rather 
than operates in a sequential pattern."

*"With kernel development, you’re writing APIs, not applications themselves."*


## Links
- [Writing simple Linux Kernel Module](https://blog.sourcerer.io/writing-a-simple-linux-kernel-module-d9dc3762c234)
 by [Robert W. Oliver II](https://blog.sourcerer.io/@rwoliver2)




