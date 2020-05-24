#
# LKM Sandbox::Makefile for PoCs
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

SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
POCS_BUILD=$(SELF_DIR)build

.PHONY: pocs # Force targets to be re-made independent of depending files

all:
	$(info Compiling all PoC source files)
	gcc -pthread -O -Wall -Wextra -o $(POCS_BUILD)/io-port-permissions io-port-permissions.c

clean:
	@rm -f $(POCS_BUILD)/*