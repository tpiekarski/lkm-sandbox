#
# LKM Sandbox::Makefile for Rehearsal
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
REHEARSALS_BUILD=$(SELF_DIR)build

.PHONY: rehearsals # Force targets to be re-made independent of depending files

all:
	$(info Compiling all rehearsal source files)
	test -d $(REHEARSALS_BUILD) || mkdir $(REHEARSALS_BUILD)
	gcc -o $(REHEARSALS_BUILD)/execve execve.c
	gcc -o $(REHEARSALS_BUILD)/execve-target execve-target.c
	gcc -o $(REHEARSALS_BUILD)/fork fork.c
	gcc -o $(REHEARSALS_BUILD)/io_ports io_ports.c
	gcc -o $(REHEARSALS_BUILD)/read read.c
	gcc -o $(REHEARSALS_BUILD)/simple_circular_buffer circular-buffers/simple_circular_buffer.c
	gcc -o $(REHEARSALS_BUILD)/write write.c

clean:
	@rm -f $(REHEARSALS_BUILD)/*
