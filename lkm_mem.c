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
#define LKM_MEM_PROC_BUFFER_ENTRY "buffer"
#define LKM_MEM_PROC_FREE_ENTRY "free"
#define LKM_MEM_PROC_SHARED_ENTRY "shared"
#define LKM_MEM_PROC_TOTAL_ENTRY "total"
#define LKM_SWAP_PROC_PARENT "swap"
#define LKM_SWAP_PROC_FREE_ENTRY "free"
#define LKM_SWAP_PROC_TOTAL_ENTRY "total"

struct sysinfo si;
struct proc_dir_entry *lkm_proc_parent;
struct proc_dir_entry *mem_proc_parent;
struct proc_dir_entry *mem_proc_buffer_entry;
struct proc_dir_entry *mem_proc_free_entry;
struct proc_dir_entry *mem_proc_shared_entry;
struct proc_dir_entry *mem_proc_total_entry;
struct proc_dir_entry *swap_proc_parent;
struct proc_dir_entry *swap_proc_free_entry;
struct proc_dir_entry *swap_proc_total_entry;

static int lkm_value_show(struct seq_file *seq, void *v)
{
	seq_put_decimal_ull(seq, "", *(unsigned long *)seq->private);
	seq_putc(seq, '\n');

	return 0;
}

struct proc_dir_entry *
lkm_proc_create_single_data(const char *name, unsigned long *value,
			    struct proc_dir_entry *parent)
{
	struct proc_dir_entry *entry;

	entry = proc_create_single_data(name, LKM_PROC_PERMISSION, parent,
					lkm_value_show, value);

	if (entry == NULL) {
		printk(KERN_ERR "lkm_mem: Failed to create %s.\n", name);
		// todo: How to abort loading module in gentle way?
	}

	return entry;
}

struct proc_dir_entry *lkm_proc_mkdir(const char *name,
				      struct proc_dir_entry *parent)
{
	struct proc_dir_entry *entry;

	entry = proc_mkdir(name, parent);

	if (entry == NULL) {
		printk(KERN_ERR
		       "lkm_mem: Failed to create parent %s in proc.\n",
		       name);

		// todo: How to abort loading module in gentle way?
	}

	return entry;
}

void lkm_remove_proc_entry(struct proc_dir_entry *entry, const char *name,
			   struct proc_dir_entry *parent)
{
	// todo: Check if name is stored in proc_dir_entry and use it instead of passing it.
	if (entry != NULL) {
		remove_proc_entry(name, parent);
	}
}

static int __init lkm_mem_init(void)
{
	lkm_proc_parent = lkm_proc_mkdir(LKM_PROC_PARENT, NULL);

	si_meminfo(&si);

	mem_proc_parent = lkm_proc_mkdir(LKM_MEM_PROC_PARENT, lkm_proc_parent);

	mem_proc_buffer_entry = lkm_proc_create_single_data(
		LKM_MEM_PROC_BUFFER_ENTRY, &si.bufferram, mem_proc_parent);

	mem_proc_free_entry = lkm_proc_create_single_data(
		LKM_MEM_PROC_FREE_ENTRY, &si.freeram, mem_proc_parent);

	mem_proc_shared_entry = lkm_proc_create_single_data(
		LKM_MEM_PROC_SHARED_ENTRY, &si.sharedram, mem_proc_parent);

	mem_proc_total_entry = lkm_proc_create_single_data(
		LKM_MEM_PROC_TOTAL_ENTRY, &si.totalram, mem_proc_parent);

	swap_proc_parent =
		lkm_proc_mkdir(LKM_SWAP_PROC_PARENT, lkm_proc_parent);

	swap_proc_free_entry = lkm_proc_create_single_data(
		LKM_SWAP_PROC_FREE_ENTRY, &si.freeswap, swap_proc_parent);

	swap_proc_total_entry = lkm_proc_create_single_data(
		LKM_SWAP_PROC_TOTAL_ENTRY, &si.totalswap, swap_proc_parent);

	return 0;
}

static void __exit lkm_mem_exit(void)
{
	lkm_remove_proc_entry(mem_proc_buffer_entry, LKM_MEM_PROC_BUFFER_ENTRY,
			      mem_proc_parent);

	lkm_remove_proc_entry(mem_proc_free_entry, LKM_MEM_PROC_FREE_ENTRY,
			      mem_proc_parent);

	lkm_remove_proc_entry(mem_proc_shared_entry, LKM_MEM_PROC_SHARED_ENTRY,
			      mem_proc_parent);

	lkm_remove_proc_entry(mem_proc_total_entry, LKM_MEM_PROC_TOTAL_ENTRY,
			      mem_proc_parent);

	lkm_remove_proc_entry(mem_proc_parent, LKM_MEM_PROC_PARENT,
			      lkm_proc_parent);

	lkm_remove_proc_entry(swap_proc_free_entry, LKM_SWAP_PROC_FREE_ENTRY,
			      swap_proc_parent);

	lkm_remove_proc_entry(swap_proc_total_entry, LKM_SWAP_PROC_TOTAL_ENTRY,
			      swap_proc_parent);

	lkm_remove_proc_entry(swap_proc_parent, LKM_SWAP_PROC_PARENT,
			      lkm_proc_parent);

	lkm_remove_proc_entry(lkm_proc_parent, LKM_PROC_PARENT, NULL);
}

module_init(lkm_mem_init);
module_exit(lkm_mem_exit);
