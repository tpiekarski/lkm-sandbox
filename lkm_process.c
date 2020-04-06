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

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Piekarski");
MODULE_DESCRIPTION("Accessing current process information");
MODULE_VERSION("0.1");

#define LKM_PROCESS_OUTPUT_SIZE 64

static char *info;

static char *get_process_information(void)
{
	char *buffer;
	int written;

	buffer = NULL;
	written = 0;

	buffer = kmalloc(LKM_PROCESS_OUTPUT_SIZE, GFP_USER);

	if (buffer == NULL) {
		printk(KERN_WARNING
		       "%s: Failed allocating memory for process information.\n",
		       THIS_MODULE->name);

		return NULL;
	}

	written = snprintf(buffer, LKM_PROCESS_OUTPUT_SIZE,
			   "%s: Current process is '%s' (pid %i)\n",
			   THIS_MODULE->name, current->comm, current->pid);

	if (written == 0) {
		printk(KERN_WARNING "%s: Failed writing process information.\n",
		       THIS_MODULE->name);

		return NULL;
	}

	return buffer;
}

static void print_process_information(void)
{
	info = get_process_information();

	if (info == NULL) {
		printk(KERN_WARNING "%s: Failed getting process information.\n",
		       THIS_MODULE->name);

		return;
	}

	printk(KERN_INFO "%s", info);
	kfree(info);
}

static int __init lkm_process_init(void)
{
	print_process_information();

	return 0;
}

static void __exit lkm_process_exit(void)
{
	print_process_information();
}

module_init(lkm_process_init);
module_exit(lkm_process_exit);
