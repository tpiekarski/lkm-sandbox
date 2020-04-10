/*
 * LKM Sandbox::DebugFS
 * <https://github.com/tpiekarski/lkm-sandbox>
 * ---
 * Copyright 2020 Thomas Piekarski <t.piekarski@deloquencia.de>
 *
 * This file is part of LKM Sandbox.
 * 
 * LKM Sandbox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * LKM Sandbox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with LKM Sandbox. If not, see <https://www.gnu.org/licenses/>.
 * 
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Piekarski");
MODULE_DESCRIPTION("Module for accessing the debug filesystem");
MODULE_VERSION("0.1");

static int __init lkm_debugfs_init(void)
{
	printk(KERN_INFO "%s: Entering debugfs and looking around.\n",
	       THIS_MODULE->name);

	return 0;
}

static void __exit lkm_debugfs_exit(void)
{
	printk(KERN_INFO "%s: Leaving debugfs.\n", THIS_MODULE->name);
}

module_init(lkm_debugfs_init);
module_exit(lkm_debugfs_exit);
