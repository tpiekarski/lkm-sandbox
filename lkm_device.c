/*
 * LKM Sandbox::Device
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
#include <linux/module.h>
#include <linux/kernel.h>

// Additional Headers for Devices
#include <linux/fs.h>
#include <asm/uaccess.h>

// Module Meta Data (For available license see include/linux/module.h)
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Piekarski");
MODULE_DESCRIPTION("A Sandbox device module for the Linux Kernel");
MODULE_VERSION("0.1");

// Prototypes
static int device_open(struct inode*, struct file*);
static int device_release(struct inode*, struct file*);
static ssize_t device_read(struct file*, char*, size_t, loff_t*);
static ssize_t device_write(struct file*, const char*, size_t, loff_t*);

// Definitions
#define DEVICE_NAME "lkm_device"
#define MESSAGE "Hello, Linux!\n"
#define MESSAGE_BUFFER_LENGTH 15

// Statics
static int major_num;
static int device_open_count = 0;
static char message_buffer[MESSAGE_BUFFER_LENGTH];
static char* message_ptr;
static struct file_operations file_ops = {
    .open    = device_open,
    .read    = device_read,
    .release = device_release,
    .write   = device_write
};

//
// Device Operations
//

static ssize_t device_read(struct file* flip, char* buffer, size_t len, loff_t* offset) {
    int bytes_read = 0;

    printk(KERN_INFO "Starting to read from sandbox device.\n");
    if (*message_ptr == 0) {
        message_ptr = message_buffer;
    }

    while (len && *message_ptr) {
        printk(KERN_INFO "Reading from device...\n");
        put_user(*(message_ptr++), buffer++);
        len--;
        bytes_read++;
    }

    return bytes_read;
}

static ssize_t device_write(struct file* flip, const char* buffer, size_t len, loff_t *offset) {
    printk(KERN_ALERT "Writing to sandbox device is not supported.\n");

    return -EINVAL;
}

static int device_open(struct inode* inode, struct file* file) {
    if (device_open_count > 0) {
        printk(KERN_INFO "Sandbox device already open.\n");
        return -EBUSY;
    }

    printk(KERN_INFO "Opening sandbox device.\n");
    device_open_count++;
    try_module_get(THIS_MODULE);

    return 0;
}

static int device_release(struct inode* inode, struct file* file) {
    printk(KERN_INFO "Closing sandbox device.\n");
    device_open_count--;
    module_put(THIS_MODULE);

    return 0;
}

//
// Module Init & Exit
//

static int __init lkm_device_init(void) {
    printk(KERN_INFO "Initialize Sandbox Device Module...\n");

    strncpy(message_buffer, MESSAGE, MESSAGE_BUFFER_LENGTH);
    message_ptr = message_buffer;
    major_num = register_chrdev(0, DEVICE_NAME, &file_ops);

    if (major_num < 0) {
        printk(KERN_ALERT "Could not register sandbox device: %d\n", major_num);

        return major_num;
    }

    printk(KERN_INFO "Registered sandbox device with major number %d\n", major_num);

    return 0;
}

static void __exit lkm_device_exit(void) {
    printk(KERN_INFO "Exiting Sandbox Device Module...\n");
    unregister_chrdev(major_num, DEVICE_NAME);
}

module_init(lkm_device_init);
module_exit(lkm_device_exit);
