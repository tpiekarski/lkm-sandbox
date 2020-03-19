/*
 * LKM Skeleton
 *
 */

// Linux Kernel Headers for Module Development
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>


// Module Meta Data (For available license see include/linux/module.h)
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Piekarski");
MODULE_DESCRIPTION("A skeleton for Linux Kernel Development");
MODULE_VERSION("0.1");

//
// Module Init & Exit
//

static int __init lkm_skeleton_init(void) {    
    // todo: implement init
    return 0;
}

static void __exit lkm_skeleton_exit(void) {
    // todo: implement exit
}


// Registering init and exit functions
module_init(lkm_skeleton_init);
module_exit(lkm_skeleton_exit);

