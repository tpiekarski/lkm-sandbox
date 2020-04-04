/*
 * LKM Sandbox::lkm_process
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

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Piekarski");
MODULE_DESCRIPTION("Character device for process information");
MODULE_VERSION("0.1");

static ssize_t device_read(struct file *sf, char *buffer, size_t len,
			   loff_t *offset)
{
	// todo: implement

	// todo: get current process information and output them

	ssize_t s;
	return s;
}

static ssize_t device_write(struct file *sf, const char *buffer, size_t len,
			    loff_t *offset)
{
	// todo: implement

	ssize_t s;
	return s;
}

static int device_open(struct inode *inode, struct file *file)
{
	// todo: implement

	return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
	// todo: implement

	return 0;
}

static int __init lkm_process_init(void)
{
	// todo: implement init function

	return 0;
}

static void __exit lkm_process_exit(void)
{
	// todo: implement exit function
}

module_init(lkm_process_init);
module_exit(lkm_process_exit);
