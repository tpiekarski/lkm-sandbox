#
# pretty-printk::Debug Makefile Include
# <https://github.com/tpiekarski/pretty-printk>
# ---
# Copyright 2020 Thomas Piekarski <t.piekarski@deloquencia.de>
#
# This file is part of pretty-printk - a pretty way to print to 
# the Linux Kernel Ring Buffer
# 
# pretty-printk is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
# 
# pretty-printk is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with pretty-printk. If not, see <https://www.gnu.org/licenses/>.
# 
#

# Pass debug=1 to enable debugging output (make debug=1)
debug = 0

ifeq ($(debug),1)
	DEBUG_FLAGS = -O -g -DPP_DEBUG
else
	DEBUG_FLAGS = -O2
endif
