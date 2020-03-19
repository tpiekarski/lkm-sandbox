#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

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
    .open = device_open,
    .read = device_read,
    .release = device_release,
    .write = device_write
};

static ssize_t device_read(struct file* flip, char* buffer, size_t len, loff_t* offset) {
    // todo: implement reading operation

    return 0;
}

static ssize_t device_write(struct file* flip, const char* buffer, size_t len, loff_t *offset) {
    // todo: implement writing operation

    return 0;
}

static int device_open(struct inode* inode, struct file* file) {
    if (device_open_count > 0) {
        printk(KERN_INFO "Sandbox device already open.");
        return -EBUSY;
    }

    printk(KERN_INFO "Opening sandbox device.");
    device_open_count++;
    try_module_get(THIS_MODULE);

    return 0;
}

static int device_release(struct inode* inode, struct file* file) {
    printk(KERN_INFO "Closing sandbox device.");
    device_open_count--;
    module_put(THIS_MODULE);

    return 0;
}

static int __init lkm_device_init(void) {
    printk(KERN_INFO "Initialize Sandbox Device Module...\n");

    return 0;
}

static void __exit lkm_device_exit(void) {
    printk(KERN_INFO "Exiting Sandbox Device Module...\n");
}

module_init(lkm_device_init);
module_exit(lkm_device_exit);