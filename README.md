# Linux Kernel Module (LKM) Sandbox

## Overview
[Building](#building) / [Testing](#testing) / [Make](#make) / [Modules](#modules) / [License](#license) / [Notes](#notes) / [Links](#links)

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
To run all tests, including loading/unloading and proc test.
```
make test
```

Testing by loading, unloading and outputing Kernel Ring Buffer (sudo will ask for root permissions).
```
make test-module name=lkm_device
make test-module name=lkm_proc
make test-module name=lkm_sandbox
make test-module name=lkm_skeleton
```
Additional tests for sandbox device including loading, creating device and getting messages.
For creating character device the major number is needed and can be obtained by cating the file /proc/lkm_device_major.
This major number is written to the Kernel Ring Buffer as well.
```
sudo insmod lkm_device.ko
dmesg | grep "Registered sandbox device"
sudo mknod /dev/lkm_device c $(cat /proc/lkm_device_major) 0
test -c /dev/lkm_device && cat /dev/lkm_device || echo "Device /dev/lkm_device" not found."
sudo rmmod lkm_device 
```

Tests for sandbox proc including loading module, testing if file inside /proc exists and outputing that file. 
Either run the Makefile target *proctest* with ```make test-proc``` or the following few commands:
```
sudo insmod lkm_proc.ko
test -f /proc/lkm_proc && cat /proc/lkm_proc || echo "File /proc/lkm_proc not found."
sudo rmmod lkm_proc
```

## [Make](#make)
![Screenshots of make](images/screenshots.gif?raw=true "Screenshots of make")


## [Modules](#modules)
#|Module|Source|Description
---|---|---|---
1|LKM Device|[lkm_device.c](lkm_device.c)|Module showing how to operate with character devices and storing device information in /proc filesystem
2|LKM Proc|[lkm_proc.c](lkm_proc.c)|Module accessing /proc filesystem using sequential I/O
3|LKM Sandbox|[lkm_sandbox.c](lkm_sandbox.c)|Sandbox module for different experiments
4|LKM Skeleton|[lkm_skeleton.c](lkm_skeleton.c)|Skeleton module for faster scaffolding of new modules

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
- GNU, [Licenses HowTo](https://www.gnu.org/licenses/gpl-howto.en.html)
- Kernelnewbies, [Sequential Files HowTo](https://kernelnewbies.org/Documents/SeqFileHowTo)
- Medium, [Writing simple Linux Kernel Module](https://blog.sourcerer.io/writing-a-simple-linux-kernel-module-d9dc3762c234) by [Robert W. Oliver II](https://blog.sourcerer.io/@rwoliver2)
- Pointer-Overloading, [Creating entry in proc...](http://pointer-overloading.blogspot.com/2013/09/linux-creating-entry-in-proc-file.html) by eniac
- Superuser, [Variables in GNU Make...](https://superuser.com/questions/790560/variables-in-gnu-make-recipes-is-that-possible)
