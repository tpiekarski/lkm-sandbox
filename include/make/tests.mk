#
# LKM Sandbox::Makefile Test Functions
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

INVALID_ARGUMENTS = 1
BUILD_ERROR = 2
COMPARISON_ERROR = 3

# Don't break line @lsmod... with backslashes, grep will fail for unknown reason.
define load_module
	@echo "Loading module $(1)"
	@lsmod | awk '{print $$1}' | grep -qE "^$(1)$$" && (sudo rmmod $(1) && sudo insmod $(1).ko) || sudo insmod $(1).ko
endef

define test_file_exists
	@echo "Testing if file $(1) exists."
	@$(3) test $(2) $(1) \
		|| (echo "  !! The file $(1) could not be found."; exit $(BUILD_ERROR))
endef

define test_file_readable
	@echo "Testing if file $(1) is readable."
	@test -r $(1) \
		|| (echo "  !! The file $(1) is not readable"; exit $(BUILD_ERROR))
endef

define test_module_exists
	@echo "Testing if the module $(1) exists."
	@test ${1} \
		|| (echo "  !! Please provide a valid module name to test, like 'make test name=lkm_sandbox'."; exit $(INVALID_ARGUMENTS))

	$(eval module_filename = $(1).ko)
	@test -f $(module_filename) \
		|| (echo "  !! The module $(1) could not be found. Did you forgot to run make?"; exit $(BUILD_ERROR))
endef

define test_value_is_greater_zero
	@echo "Testing if value $(1) is greater than zero."
	@test $(1) -gt 0 \
		|| (echo "  !! Provided value, $(1) is not greater than zero."; exit $(COMPARISON_ERROR))
endef

define test_compare_values
	@echo "Comparing '$(1)' $(2) ' $(3)'."
	@test $(1) $(2) $(3) \
		|| (echo "  !! Comparison failed values have been '$(1)' and '$(3)'."; exit $(COMPARISON_ERROR))
endef

define test_module_loaded
	@echo "Testing if module $(1) was loaded."
	@lsmod | awk '{print $$1}' | grep -qE "^$(1)$$"  \
		|| (echo "  !! Module $(1) is not loaded."; exit $(BUILD_ERROR))
endef

define test_module
	@sudo dmesg --clear
	$(call load_module,$(1))
	$(call test_module_loaded,$(1))
	@sudo rmmod $(1)
	@dmesg
endef

define test_proc_file_content_greater_than_zero
	$(call test_file_exists,$(1),"-f")
	$(call test_file_readable,$(1))
	$(call test_value_is_greater_zero,`cat $(1)`)
endef

define test_proc_file_readable
	$(call test_file_exists,$(1), "-f")
	$(call test_file_readable,$(1))
endef
