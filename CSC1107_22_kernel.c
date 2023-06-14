#include <linux/init.h>
#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h> // printk()
#include <linux/slab.h>   // kmalloc()
#include <linux/fs.h>     // everything...
#include <linux/errno.h>  //error codes
#include <linux/types.h>  // size_t
#include <linux/proc_fs.h>
#include <linux/fcntl.h> // O_ACCMODE
#include <asm/system.h>  // cli(), *_flags
#include <asm/uaccess.h> // copy_from/to_user

void device_open() {}

/* Read from kernel
    - invoked upon "Enter"/"Space" key

    return:
        char* sentence, user_hash, hash_type, kernel_hash, compare
*/
void device_read() {}

/* Write into kernel
    - print user space payloads and generated hash in kernel space
    - print result of comparing user space hash is same as kernel space hash

    param:
        char* sentence, user_hash, hash_type
    return:
        None
*/
void device_write() {}

void device_release() {}
