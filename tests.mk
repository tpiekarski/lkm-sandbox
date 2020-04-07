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

define test_file_exists
	@echo "function: test_file_exists"
endef

define test_module_exists
	@echo "function: test_module_exists"
endef

define test_value_is_greater_zero
	@echo "function: test_value_is_greater_zero"
endef

define test_value_is_equal
	@echo "function: test_value_is_equal"
endef
