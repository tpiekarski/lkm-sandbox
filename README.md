# Linux Kernel Module (LKM) Sandbox

## Overview
[Building](#building) / [Testing](#testing) / [Make](#make) / [Modules](#modules) / [License](#license) / [Notes](#notes) / [Links](#links)

The Linux Kernel Module Sandbox is a little module to learn, test and experiment with the development of Kernel Modules 
for the Linux System. It won't be anything very usable and there won't be any new driver emerging from this sandbox 
(at least it is not intended). That module is only for learning and study purpose and will contain also a few comments
explaining things in the source code.

## [Building](#building)
```
make clean && make
```

## [Testing](#testing)
To run all available tests, including basic loading/unloading and both additional tests for device and /proc.
```
make test
```

Testing by loading, unloading and outputting Kernel Ring Buffer (sudo will ask for root permissions).
```
make test-module name=lkm_device
make test-module name=lkm_parameters
make test-module name=lkm_proc
make test-module name=lkm_sandbox
make test-module name=lkm_skeleton
```

Additional tests for the sandbox device including loading module, gathering major device number from /proc, creating device and comparing 
the final message either run the Makefile target with ```make test-device``` or run the following commands. 
For creating character device the major number is needed and can be obtained by catting the file /proc/lkm_device_major. 
This major number is written to the Kernel Ring Buffer as well. It is possible to provide this major number using the module parameter
param_major_num and load this module like ```sudo insmod lkm_device.ko param_major_num=241``` (At this moment this static allocation seems 
not to work reliable. The registration of the sandbox device fails sometimes. It was tried with a major of -16 when registration fails and the operation is not permitted like that - the reason is unknown.)
```
sudo insmod lkm_device.ko
dmesg | grep "Registered sandbox device"
sudo mknod /dev/lkm_device c $(cat /proc/lkm_device_major) 0
test -c /dev/lkm_device && cat /dev/lkm_device || echo "Device /dev/lkm_device" not found."
sudo rmmod lkm_device 
```

Additional tests for sandbox access to /proc including loading module, testing if file inside /proc exists and outputting that file. 
Either run the Makefile target *test-proc* with ```make test-proc``` or the following few commands:
```
sudo insmod lkm_proc.ko
test -f /proc/lkm_proc && cat /proc/lkm_proc || echo "File /proc/lkm_proc not found."
sudo rmmod lkm_proc
```

For additional tests for passing parameters to module lkm_parameters run the Makefile target
test-parameter with ```make test-parameters```. This will load/unload the module and compare
the parameters number and message passed when loading the module with the values read in the
/sys filesystem (/sys/module/lkm_parameters/parameters/*). Or run the following commands.
```
sudo insmod lkm_parameters.ko number=33 message=\"Some message...\"
cat /sys/module/lkm_parameters/parameters/number
cat /sys/module/lkm_parameters/parameters/message
sudo rmmod lkm_parameters
```

## [Make](#make)
![Screenshots of make](images/screenshots.gif?raw=true "Screenshots of make")


## [Modules](#modules)
#|Module|Source|Description
---|---|---|---
1|LKM Device|[lkm_device.c](lkm_device.c)|Module showing how to operate with character devices and storing device information in /proc filesystem
2|LKM Parameters|[lkm_parameters.c](lkm_parameters.c)|Module for passing parameters from user- to kernelspace
3|LKM Proc|[lkm_proc.c](lkm_proc.c)|Module accessing /proc filesystem using sequential I/O
4|LKM Sandbox|[lkm_sandbox.c](lkm_sandbox.c)|Sandbox module for different experiments
5|LKM Skeleton|[lkm_skeleton.c](lkm_skeleton.c)|Skeleton module for faster scaffolding of new modules

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
- Developers Area, [Kernel Module Parameters](https://devarea.com/linux-kernel-development-kernel-module-parameters/) by [Liran B.H](https://devarea.com/author/liran/)
- GNU, [Licenses HowTo](https://www.gnu.org/licenses/gpl-howto.en.html)
- Kernelnewbies, [Sequential Files HowTo](https://kernelnewbies.org/Documents/SeqFileHowTo)
- Medium, [Writing simple Linux Kernel Module](https://blog.sourcerer.io/writing-a-simple-linux-kernel-module-d9dc3762c234) by [Robert W. Oliver II](https://blog.sourcerer.io/@rwoliver2)
- Pointer-Overloading, [Creating entry in proc...](http://pointer-overloading.blogspot.com/2013/09/linux-creating-entry-in-proc-file.html) by eniac
- Superuser, [Variables in GNU Make...](https://superuser.com/questions/790560/variables-in-gnu-make-recipes-is-that-possible)
