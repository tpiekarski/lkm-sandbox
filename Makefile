#
# LKM Sandbox::Makefile
# <https://github.com/tpiekarski/lkm-sandbox>
# ---
# Copyright 2020 Thomas Piekarski <t.piekarski@deloquencia.de>
#
# This file is part of LKM Sandbox.
# 
# LKM Sandbox is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
# 
# LKM Sandbox is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with LKM Sandbox. If not, see <https://www.gnu.org/licenses/>.
# 
#

obj-m += lkm_device.o lkm_proc.o lkm_sandbox.o lkm_skeleton.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test:
	$(info Running all available tests...)
	@make test-module name=lkm_device
	@make test-module name=lkm_proc
	@make test-module name=lkm_sandbox
	@make test-module name=lkm_skeleton
	@make test-proc

test-module:
	$(info >> Testing Module '$(name)' by loading and displaying Kernel Message Ring Buffer...)
	$(info >> Root permissions are needed for clearing buffer with dmesg and loading/unloading with insmod/rmmod)
	
	@test ${name} || (echo "!! Please provide a valid module name to test, like 'make test name=lkm_sandbox'."; exit 1)
	$(eval filename = ${name}.ko)
	@test -f ${filename} || (echo "!! The module ${filename} could not be found. Did you forgot to run make?"; exit 2)
	
	@sudo dmesg --clear
	@sudo insmod ${filename}
	@sudo rmmod ${filename}
	@dmesg

test-proc:
	$(eval proc_module = lkm_proc)
	$(eval proc_file = /proc/$(proc_module))

	$(info >> Testing access to /proc filesystem with the module '$(proc_module)' by loading and cating '$(proc_file)'...)
	$(info >> Root permissions are needed for loading and unloading with insmod/rmmod)
	
	$(eval filename = ${proc_module}.ko)
	@test -f ${filename} || (echo "!! The module ${filename} could not be found. Did you forgot to run make?"; exit 2)
	
	@sudo insmod ${filename}
	@test -f $(proc_file) && echo ">> The file $(proc_file) exists." || (echo "!! The file $(proc_file) does not exists."; exit 3)
	@cat $(proc_file)
	@sudo rmmod ${filename}

license:
	@echo " LKM Sandbox::Make\n\n \
	LKM Sandbox is free software: you can redistribute it and/or modify\n \
	it under the terms of the GNU General Public License as published by\n \
	the Free Software Foundation, either version 3 of the License, or\n \
	(at your option) any later version.\n\n \
	LKM Sandbox is distributed in the hope that it will be useful,\n \
	but WITHOUT ANY WARRANTY; without even the implied warranty of\n \
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n \
	GNU General Public License for more details.\n\n \
	You should have received a copy of the GNU General Public License\n \
	along with LKM Sandbox.  If not, see <https://www.gnu.org/licenses/>.\n"
