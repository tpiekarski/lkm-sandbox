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

#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Piekarski");
MODULE_DESCRIPTION("Module for accessing the debug filesystem");
MODULE_VERSION("0.1");

#define LKM_DEBUGFS_DIR "lkm"
#define LKM_DEBUGFS_CONTENT_LEN 200
#define LKM_DEBUGFS_PERMISSION 0644

static ssize_t debug_read(struct file *fp, char *buffer, size_t count,
			  loff_t *position);
static ssize_t debug_write(struct file *fp, const char *buffer, size_t count,
			   loff_t *position);

char content[LKM_DEBUGFS_CONTENT_LEN];
static struct dentry *debug_root;
static struct dentry *debug_file_entry;
static u64 value;
static int file_value;

static const struct file_operations fops = { .owner = THIS_MODULE,
					     .read = debug_read,
					     .write = debug_write };

static ssize_t debug_read(struct file *fp, char *buffer, size_t count,
			  loff_t *position)
{
	return simple_read_from_buffer(buffer, count, position, content,
				       LKM_DEBUGFS_CONTENT_LEN);
}
static ssize_t debug_write(struct file *fp, const char *buffer, size_t count,
			   loff_t *position)
{
	if (count > LKM_DEBUGFS_CONTENT_LEN) {
		return -EINVAL;
	}

	return simple_write_to_buffer(content, LKM_DEBUGFS_CONTENT_LEN,
				      position, buffer, count);
}

static int __init lkm_debugfs_init(void)
{
	printk(KERN_INFO "%s: Entering debugfs and looking around.\n",
	       THIS_MODULE->name);

	if (!debugfs_initialized()) {
		printk(KERN_ERR "%s: debugfs is not initialized.\n",
		       THIS_MODULE->name);

		return -ENODEV;
	}

	debug_root = debugfs_create_dir(LKM_DEBUGFS_DIR, NULL);

	if (debug_root == NULL) {
		printk(KERN_ERR
		       "%s: Failed creating directory %s in debugfs.\n",
		       THIS_MODULE->name, LKM_DEBUGFS_DIR);

		return -ENODEV;
	}

	value = 42;
	debugfs_create_u64("value", LKM_DEBUGFS_PERMISSION, debug_root, &value);

	debug_file_entry =
		debugfs_create_file("message", LKM_DEBUGFS_PERMISSION,
				    debug_root, &file_value, &fops);

	strncpy(content, "Hello, debugfs!\n\0", LKM_DEBUGFS_CONTENT_LEN);

	if (debug_file_entry == NULL) {
		printk(KERN_ERR "%s: Failed creating file.\n",
		       THIS_MODULE->name);

		return -EIO;
	}

	return 0;
}

static void __exit lkm_debugfs_exit(void)
{
	printk(KERN_INFO "%s: Leaving debugfs.\n", THIS_MODULE->name);

	if (debug_root != NULL) {
		debugfs_remove_recursive(debug_root);
		debug_file_entry = NULL;
		debug_root = NULL;
	}
}

module_init(lkm_debugfs_init);
module_exit(lkm_debugfs_exit);
