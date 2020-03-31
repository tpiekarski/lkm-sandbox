/*
 * LKM Sandbox::Memory, Swap and /proc
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
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Piekarski");
MODULE_DESCRIPTION("Exposing separated memory and swap statistics to /proc");
MODULE_VERSION("0.1");

#define LKM_PROC_PERMISSION 0444
#define LKM_PROC_PARENT "lkm"
#define LKM_MEM_PROC_PARENT "mem"
#define LKM_MEM_PROC_TOTAL_ENTRY "total"
#define LKM_MEM_PROC_FREE_ENTRY "free"

struct sysinfo si;
struct proc_dir_entry *lkm_proc_parent;
struct proc_dir_entry *mem_proc_parent;
struct proc_dir_entry *mem_proc_total_entry;
struct proc_dir_entry *mem_proc_free_entry;

static int lkm_value_show(struct seq_file *seq, void *v)
{
	seq_put_decimal_ull(seq, "", *(unsigned long *)seq->private);
	seq_putc(seq, '\n');

	return 0;
}

void lkm_proc_create_single_data(struct proc_dir_entry *entry,
				 unsigned long *value, const char *name)
{
	entry = proc_create_single_data(name, LKM_PROC_PERMISSION,
					mem_proc_parent, lkm_value_show, value);

	if (entry == NULL) {
		printk(KERN_ERR "lkm_mem: Failed to create /proc/%s/%s/%s.\n",
		       LKM_PROC_PARENT, LKM_MEM_PROC_PARENT, name);

		// todo: What and how to return error-code without having
		// ifs in calling function all over the place? Loading of
		// module should be aborted in some way and a clean unload
		// (if possible) initiated.
	}
}

void lkm_remove_proc_entry(struct proc_dir_entry *entry, const char *name,
			   struct proc_dir_entry *parent)
{
	// todo: check if name is stored in proc_dir_entry and remove one arg.
	if (entry != NULL) {
		remove_proc_entry(name, parent);
	}
}

static int __init lkm_mem_init(void)
{
	lkm_proc_parent = proc_mkdir(LKM_PROC_PARENT, NULL);

	if (lkm_proc_parent == NULL) {
		printk(KERN_ERR
		       "lkm_mem: Failed to create parent /proc/%s for lkm.\n",
		       LKM_PROC_PARENT);

		return 1;
	}

	mem_proc_parent = proc_mkdir(LKM_MEM_PROC_PARENT, lkm_proc_parent);

	if (mem_proc_parent == NULL) {
		printk(KERN_ERR
		       "lkm_mem: Failed to create parent /proc/%s/%s for mem.\n",
		       LKM_PROC_PARENT, LKM_MEM_PROC_PARENT);

		return 1;
	}

	si_meminfo(&si);

	lkm_proc_create_single_data(mem_proc_total_entry, &si.totalram,
				    LKM_MEM_PROC_TOTAL_ENTRY);

	lkm_proc_create_single_data(mem_proc_free_entry, &si.freeram,
				    LKM_MEM_PROC_FREE_ENTRY);

	return 0;
}

static void __exit lkm_mem_exit(void)
{
	lkm_remove_proc_entry(mem_proc_total_entry, LKM_MEM_PROC_TOTAL_ENTRY,
			      mem_proc_parent);

	lkm_remove_proc_entry(mem_proc_free_entry, LKM_MEM_PROC_FREE_ENTRY,
			      mem_proc_parent);

	lkm_remove_proc_entry(mem_proc_parent, LKM_MEM_PROC_PARENT,
			      lkm_proc_parent);

	lkm_remove_proc_entry(lkm_proc_parent, LKM_PROC_PARENT, NULL);
}

module_init(lkm_mem_init);
module_exit(lkm_mem_exit);
