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

// Linux Kernel Headers for Module Development
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

// Additional headers for the /proc filesystem and sequential file I/O
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

// Module Meta Data
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Piekarski");
MODULE_DESCRIPTION("Module for accesing the /proc filesystem");
MODULE_VERSION("0.1");

// Prototypes
static int lkm_proc_open(struct inode *inode, struct file *file);
static int lkm_proc_show(struct seq_file *seq, void *v);

// Definitions
#define LKM_PROC_FILE_NAME "lkm_proc"
#define LKM_PROC_MESSAGE "Hello, /proc!\n"
#define LKM_PROC_PARENT NULL // root of /proc
#define LKM_PROC_PERMISSION 0444

// Global Structures
struct proc_dir_entry *lkm_proc_entry;
static const struct file_operations lkm_proc_fops = {
    .llseek = seq_lseek,
    .open = lkm_proc_open,
    .owner = THIS_MODULE,
    .read = seq_read,
    .release = single_release
};

//
// Module Init & Exit
//

static int __init lkm_proc_init(void) {
    printk(KERN_INFO "Initializing module for accessing /proc/%s...\n", LKM_PROC_FILE_NAME);
    lkm_proc_entry = proc_create(LKM_PROC_FILE_NAME, LKM_PROC_PERMISSION, LKM_PROC_PARENT, &lkm_proc_fops);

    if (lkm_proc_entry == NULL) {
        printk(KERN_ALERT "Failed to create entry '%s' in /proc.", LKM_PROC_FILE_NAME);
    }

    return 0;
}

static void __exit lkm_proc_exit(void) {
    printk(KERN_INFO "Removing /proc/%s...\n", LKM_PROC_FILE_NAME);

    remove_proc_entry(LKM_PROC_FILE_NAME, LKM_PROC_PARENT);
}

static int lkm_proc_open(struct inode *inode, struct file *file) {
    return single_open(file, lkm_proc_show, NULL);
}

static int lkm_proc_show(struct seq_file *seq, void *v) {
    seq_printf(seq, LKM_PROC_MESSAGE);

    return 0;
}

// Registering init and exit functions
module_init(lkm_proc_init);
module_exit(lkm_proc_exit);
