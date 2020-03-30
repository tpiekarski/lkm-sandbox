/*
 * LKM Sandbox::Proc
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
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Piekarski");
MODULE_DESCRIPTION("Module for accesing the /proc filesystem");
MODULE_VERSION("0.1");

static int lkm_proc_show(struct seq_file *seq, void *v);

#define LKM_PROC_FILE_NAME "lkm_proc"
#define LKM_PROC_MESSAGE "Hello, /proc!"
#define LKM_PROC_PARENT NULL // root of /proc
#define LKM_PROC_PERMISSION 0444

struct proc_dir_entry *lkm_proc_entry;

static int __init lkm_proc_init(void)
{
	printk(KERN_INFO "Initializing module for accessing /proc/%s...\n",
	       LKM_PROC_FILE_NAME);
	lkm_proc_entry =
		proc_create_single(LKM_PROC_FILE_NAME, LKM_PROC_PERMISSION,
				   LKM_PROC_PARENT, lkm_proc_show);

	if (lkm_proc_entry == NULL) {
		printk(KERN_ALERT "Failed to create entry '%s' in /proc.",
		       LKM_PROC_FILE_NAME);
	}

	return 0;
}

static void __exit lkm_proc_exit(void)
{
	printk(KERN_INFO "Removing /proc/%s...\n", LKM_PROC_FILE_NAME);

	remove_proc_entry(LKM_PROC_FILE_NAME, LKM_PROC_PARENT);
}

static int lkm_proc_show(struct seq_file *seq, void *v)
{
	seq_puts(seq, LKM_PROC_MESSAGE);
	seq_putc(seq, '\n');

	return 0;
}

module_init(lkm_proc_init);
module_exit(lkm_proc_exit);
