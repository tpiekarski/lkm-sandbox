# Linux Kernel Module (LKM) Sandbox

## Overview
[Building](#building) / [Testing](#testing) / [Modules](#modules) / [License](#license) / [Notes](#notes) / [Links](#links)

The Linux Kernel Module Sandbox is a little module to learn, test and experiment with
the development of Kernel Modules for the Linux System. It won't be anything very usable
and there won't be any new driver emerging from this sandbox (at least it is not intented).
That module is only for learning and study purpose and will contain also heavy comments
explaning things in the source code.

## [Building](#building)
```
make clean && make
```

## [Testing](#testing)
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

## [Modules](#modules)
#|Module|Source|Description
---|---|---|---
1|LKM Sandbox|[lkm_sandbox.c](lkm_sandbox.c)|A sandbox module for different experiments
2|LKM Device|[lkm_device.c](lkm_device.c)|A module showing how to operate with character devices
3|LKM Skeleton|[lkm_skeleton.c](lkm_skeleton.c)|A skeleton module for faster scaffolding little modules

## [License](#license)
LKM Sandbox is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

LKM Sandbox is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LKM Sandbox. If not, see [<https://www.gnu.org/licenses/>](https://www.gnu.org/licenses/). 

## [Notes](#notes)
"A Linux kernel module is a piece of compiled binary code that is inserted directly into 
the Linux kernel, running at ring 0, the lowest and least protected ring of execution in 
the x86–64 processor."

"Traditional application development paradigms can be largely discarded. Other than loading 
and unloading of your module, you’ll be writing code that responds to system events rather 
than operates in a sequential pattern."

*"With kernel development, you’re writing APIs, not applications themselves."*

## [Links](#links)
- [Writing simple Linux Kernel Module](https://blog.sourcerer.io/writing-a-simple-linux-kernel-module-d9dc3762c234)
 by [Robert W. Oliver II](https://blog.sourcerer.io/@rwoliver2)
- [GNU Licenses HowTo](https://www.gnu.org/licenses/gpl-howto.en.html)
