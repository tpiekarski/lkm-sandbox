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

SHELL:=/bin/bash
SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
ccflags-y := -Wall
obj-m += lkm_device.o lkm_mem.o lkm_parameters.o lkm_proc.o lkm_process.o lkm_sandbox.o lkm_skeleton.o

include $(SELF_DIR)/tests.mk

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test:
	$(info Running all available tests)
	@$(MAKE) test-tests
	@$(MAKE) test-module name=lkm_device
	@$(MAKE) test-module name=lkm_mem
	@$(MAKE) test-module name=lkm_parameters
	@$(MAKE) test-module name=lkm_proc
	@$(MAKE) test-module name=lkm_process
	@$(MAKE) test-module name=lkm_sandbox
	@$(MAKE) test-module name=lkm_skeleton
	@$(MAKE) test-device
	@$(MAKE) test-memory
	@$(MAKE) test-parameters
	@$(MAKE) test-proc

	@echo "All test targets have run successfully."

#
# Generic targets for load/dmesg/unload module tests
#

test-module:
	$(info Testing module '$(name)' by loading and displaying Kernel Message Ring Buffer...)
	$(info Root permissions are needed for clearing buffer with dmesg and loading/unloading with insmod/rmmod)
	$(call test_module_exists,$(name))
	$(call test_module,$(name))

#
# Targets for additional concept-based module tests
#

test-device:
	$(info Running additional tests for module 'lkm_device' by loading, accessing major device number in /proc and creating device)
	$(info Root permissions are needed for loading/unloading module with insmod/rmmod and creating device with mknod)

	$(eval module = lkm_device)
	$(eval proc_filename = /proc/lkm_device_major)
	$(eval device_filename = /dev/lkm_device)
	$(eval message = Hello, Linux!)

	$(call test_module_exists,$(module))
	@sudo insmod $(module_filename)
	$(call test_file_exists,$(proc_filename),"-f")
	@sudo mknod $(device_filename) c `cat $(proc_filename)` 0
	$(call test_file_exists,$(device_filename), "-c")
	$(call test_value_is_equal,"\"`head -n 1 $(device_filename)`\"","\"$(message)\"")
	@sudo rm $(device_filename)
	@sudo rmmod $(module_filename)

test-memory:
	$(info Running additional tests for module 'lkm_mem' by loading and accessing exposed memory and swap information in /proc)
	$(info Root permissions are needed for clearing buffer with dmesg and loading/unloading with insmod/rmmod)

	$(eval module = lkm_mem)
	
	$(call test_module_exists,$(module))
	@sudo insmod $(module_filename)
	$(call test_module_loaded,$(module))
	$(call test_proc_file_content_greater_than_zero,"/proc/lkm/mem/buffer")
	$(call test_proc_file_content_greater_than_zero,"/proc/lkm/mem/free")
	$(call test_proc_file_content_greater_than_zero,"/proc/lkm/mem/shared")
	$(call test_proc_file_content_greater_than_zero,"/proc/lkm/mem/total")
	$(call test_proc_file_readable,"/proc/lkm/swap/free")
	$(call test_proc_file_readable,"/proc/lkm/swap/total")

	@sudo rmmod $(module_filename)

test-parameters:
	$(eval module = lkm_parameters)
	$(eval number = 22)
	$(eval message = I am a Makefile)

	$(info Running additional tests for 'lkm_parameters' by loading and checking parameters in /sys filesystem)
	$(info Root permissions are needed for clearing buffer with dmesg and loading/unloading with insmod/rmmod)

	$(eval filename = ${module}.ko)
	@test -f ${filename} || (echo "!! The module ${filename} could not be found. Did you forgot to run make?"; exit 2)
	@sudo insmod $(filename) number=$(number) message=\"$(message)\"
	@test "`cat /sys/module/$(module)/parameters/number`" -eq $(number) \
		|| (echo "!! The parameter 'number' is unequal $(number)."; exit 3) \
		&& echo "Passed parameter 'number' is equal to $(number)."
	@test "\"`cat /sys/module/$(module)/parameters/message`\"" = "\"$(message)\"" \
		|| (echo "!! The parameter 'message' is unequal $(message)."; exit 3) \
		&& echo "Passed parameter 'message' is equal to $(message)."
	@sudo rmmod $(filename)

test-proc:
	$(eval proc_module = lkm_proc)
	$(eval proc_file = /proc/$(proc_module))

	$(info Running additional tests for 'lkm_proc' to access /proc filesystem by loading and cating '$(proc_file)'...)
	$(info Root permissions are needed for loading/unloading with insmod/rmmod)
	
	$(eval filename = ${proc_module}.ko)
	@test -f ${filename} || (echo "!! The module ${filename} could not be found. Did you forgot to run make?"; exit 2)
	
	@sudo insmod ${filename}
	@test -f $(proc_file) && echo ">> The file $(proc_file) exists." || (echo "!! The file $(proc_file) does not exists."; exit 3)
	@cat $(proc_file)
	@sudo rmmod ${filename}

#
# Miscellaneous targets
# 

test-tests:
	$(call test_file_exists,Makefile)
	$(call test_file_readable,Makefile)
	$(call test_module_exists,lkm_sandbox)
	@sudo insmod lkm_sandbox.ko
	$(call test_module_loaded,lkm_sandbox)
	@sudo rmmod lkm_sandbox
	$(call test_value_is_equal,42,42)

license:
	@echo -e " LKM Sandbox::Make\n\n \
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
