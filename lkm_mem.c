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
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Piekarski");
MODULE_DESCRIPTION("Exposing memory and swap statistics to /proc");
MODULE_VERSION("0.1");

#define LKM_MEM_PROC_PARENT "lkm"
#define LKM_MEM_PROC_ENTRY "mem"
#define LKM_MEM_PROC_PERMISSION 0444

struct sysinfo si;
struct proc_dir_entry *mem_proc_entry;
struct proc_dir_entry *mem_proc_parent;

static int lkm_mem_show(struct seq_file *seq, void *v);

static int __init lkm_mem_init(void) {
    mem_proc_parent = proc_mkdir(LKM_MEM_PROC_PARENT, NULL);

    if (mem_proc_parent == NULL) {
        printk(KERN_ERR "lkm_mem: Failed to create parent /proc/%s", LKM_MEM_PROC_PARENT);

        return 1;
    }

    mem_proc_entry = proc_create_single(LKM_MEM_PROC_ENTRY, LKM_MEM_PROC_PERMISSION, mem_proc_parent, lkm_mem_show);

    if (mem_proc_entry == NULL) {
        printk(KERN_ERR "lkm_mem: Failed to create entry /proc/%s/%s", LKM_MEM_PROC_PARENT, LKM_MEM_PROC_ENTRY);

        return 1;
    } 

    si_meminfo(&si);

    return 0;
}

static void __exit lkm_mem_exit(void) {
    if (mem_proc_entry != NULL) {
        remove_proc_entry(LKM_MEM_PROC_ENTRY, mem_proc_parent);
    }

    if (mem_proc_parent != NULL) {
        remove_proc_entry(LKM_MEM_PROC_PARENT, NULL);
    }
}

static int lkm_mem_show(struct seq_file *seq, void *v) {
    seq_put_decimal_ull(seq, "", si.totalram);
    seq_putc(seq, '\n');

    return 0;
}

module_init(lkm_mem_init);
module_exit(lkm_mem_exit);
