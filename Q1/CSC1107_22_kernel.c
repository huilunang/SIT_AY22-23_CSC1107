#include <linux/init.h>    // Default macros for __init and __exit functions
#include <linux/module.h>  // module functions
#include <linux/device.h>  // Needed to support kernel driver module
#include <linux/kernel.h>  // Debug macros
#include <linux/fs.h>      // Linux file systems support
#include <linux/uaccess.h> // Needed for 'copy_to_user'/'copy_from_user' functions
#include <crypto/hash.h>   // Support to hashing cryptographic functions
#include <linux/slab.h>    // In-kernel dynamic memory allocation
#include <linux/types.h>   // Required for custom data types

// Module info
MODULE_LICENSE("GPL");
MODULE_AUTHOR("CSC1107_22");
MODULE_DESCRIPTION("CSC1107_22 Project Module");

#define DEV_NAME "CSC1107_22"        // Name of the character device
#define DEV_CLASS "CSC1107_22_CLASS" // Name of the device class
#define BUF_SIZE 1024                // Size of plaintext buffer

// Functions prototypes
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

// global definitions
static int dev_num;                        // Major number of the character device
static struct class *csc1107_class = NULL; // Device driver class pointer
static struct device *csc1107_dev = NULL;  // Device driver pointer
static size_t open_count = 0;              // Number of types this device has been opened
static userspace_t userspace;              // Data from userspace
static char digest[BUF_SIZE];              // Result of hashing process

// Userspace struct
typedef enum hash
{
    MD5 = 0,
    SHA1,
    SHA256
} hash_t;

typedef struct userspace
{
    u8 plaintext[BUF_SIZE];
    hash_t algorithm;
} userspace_t;

// File operation struct
// Map local functions to default system calls
static struct file_operations fo = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
};

// Entry point functions(__init and __exit)
// Initalising a character device
static int __init device_init(void)
{
    pr_info("%s: Loading, please wait...\n", DEV_NAME);

    // Get a new major number for the character device
    dev_num = register_chrdev(0, DEV_NAME, &fo);
    if (dev_num < 0)
    { // Check errors
        pr_alert("%s: Error while trying to register major number\n", DEV_NAME);
        return dev_num;
    }

    pr_info("%s: New device successfully registered(major number: %d)\n", DEV_NAME, dev_num);

    // Register device class
    csc1107_class = class_create(THIS_MODULE, DEV_CLASS);
    if (IS_ERR(csc1107_class))
    {                                         // Check errors
        unregister_chrdev(dev_num, DEV_NAME); // Unregister device major number
        pr_alert("%s: unable to register device class\n", DEV_NAME);
        return PTR_ERR(csc1107_class);
    }
    pr_info("%s: Device class successfully register\n", DEV_NAME);

    // Register character device
    csc1107_dev = device_create(csc1107_class, NULL, MKDEV(dev_num, 0), NULL, DEV_NAME);
    if (IS_ERR(csc1107_dev))
    {                                         // Check errors
        class_destroy(csc1107_class);         // Destroy device class
        unregister_chrdev(dev_num, DEV_NAME); // Unregister device major number
        pr_alert("%s: failed to create a new device\n", DEV_NAME);
        return PTR_ERR(csc1107_dev);
    }

    pr_info("%s: device driver successfully created\n", DEV_NAME);

    return 0;
}

static void __exit device_exit(void)
{
    device_destroy(csc1107_class, MKDEV(dev_num, 0)); // Remove device
    class_unregister(csc1107_class);                  // Unregister the class
    class_destroy(csc1107_class);                     // Destroy the class
    unregister_chrdev(dev_num, DEV_NAME);             // Unregister device major number

    pr_info("%s: module successfully unloaded\n", DEV_NAME);
}

/* This function is invoked each time an userspace process tries
 * to open the character device with 'open()' syscall. It is usually
 * being used to set up the environment for the rest of the module but
 * since we do not have anything to configure, we will just
 * print out a message */
static int device_open(struct inode *inodep, struct file *filep)
{
    pr_info("%s: This device has been opened %d time(s)\n", DEV_NAME, (int)++open_count);

    return 0;
}

/* This function is invoked each time an userspace process
 * closes the character device. It is usually being used
 * to free allocated resources but since we have not anything
 * to do, we will just print out a message */
static int device_release(struct inode *inodep, struct file *filep)
{
    pr_info("%s: device closed successfully\n", DEV_NAME);

    return 0;
}


/* Read from kernel
    - invoked upon "Enter"/"Space" key

    return:
        char* sentence, user_hash, hash_type, kernel_hash, compare
*/
// void device_read() {}

/* This function is invoked each time we call the 'read()' syscall
 * from userspace. In our module, we will use the function
 * 'copy_to_user' to send back to the user the result(i.e., the hashed string) */
static ssize_t device_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
    size_t bytes_not_copied = 0;
    // calculate number of bytes to copy in hash digest based on the algo
    // SHA-512 hash digest = 512 bits = 64 bytes
    // SHA-384 hash digest = 384 bits = 48 bytes
    // SHA-256 hash digest = 256 bits = 32 bytes
    // SHA-1 hash digest = 160 bits = 20 bytes
    // MD5 hash digest = 128 bits = 16 bytes
    size_t bytes_to_copy = (userspace.algorithm == SHA512) ? 64 :
                            (userspace.algorithm == SHA384) ? 48 :
                            (userspace.algorithm == SHA256) ? 32 :
                            (userspace.algorithm == SHA1) ? 20 :
                            16;

    // copy_to_user returns 0 on success while it returns
    // the number of bytes not copied on errors
    bytes_not_copied = copy_to_user(buffer, digest, bytes_to_copy);
    if (bytes_not_copied)
    { // Check errors
        pr_warn("%s: Failed to send result to userspace\n", DEV_NAME);
        return -EFAULT;
    }

    return 0;
}


/* Write into kernel
    - print user space payloads and generated hash in kernel space
    - print result of comparing user space hash is same as kernel space hash

    param:
        char* sentence, user_hash, hash_type
    return:
        None
*/
// void device_write() {}

/* This function is invoked each time we call the 'write()' syscall
 * from userspace. In our module, we will use the function 'write_to_user'
 * to retrieve user data. We will then hash the plaintext string according to
 * the chosen algorithm using Linux cryptographic APIs */
static ssize_t device_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
    size_t bytes_not_copied = 0;
    struct crypto_shash *algorithm;
    struct shash_desc *desc;
    int err;

    // 'copy_from_user' method returns 0 on success and the number of bytes not copied
    // on error
    bytes_not_copied = copy_from_user(&userspace, buffer, sizeof(userspace_t));
    if (bytes_not_copied)
    { // Check errors
        pr_warn("%s: error while copying %zu bytes from userspca\n", DEV_NAME, bytes_not_copied);
        return -EFAULT;
    }

    // Select hashing algorithm
    switch (userspace.algorithm)
    {
    case MD5:
        algorithm = crypto_alloc_shash("md5", 0, 0);
        break;
    case SHA1:
        algorithm = crypto_alloc_shash("sha1", 0, 0);
        break;
    case SHA256:
        algorithm = crypto_alloc_shash("sha256", 0, 0);
        break;
    default:
        pr_alert("%s: hashing algorithm not recognized\n", DEV_NAME);
        return -EFAULT;
    }

    // Check if selected algorithm is available in the system
    if (IS_ERR(algorithm))
    { // Check errors
        pr_alert("%s: Hashing algorithm not supported\n", DEV_NAME);
        return -EFAULT;
    }

    desc = kmalloc(sizeof(struct shash_desc) + crypto_shash_descsize(algorithm), GFP_KERNEL);
    if (!desc)
    { // check errors
        pr_err("%s: failed to allocate memory(kmalloc)\n", DEV_NAME);
        return -ENOMEM;
    }
    desc->tfm = algorithm;

    // Initialize shash API
    err = crypto_shash_init(desc);
    if (err)
    {
        pr_err("%s: failed to initialize shash\n", DEV_NAME);
        goto out;
    }

    // Execute hash function
    err = crypto_shash_update(desc, userspace.plaintext, strlen(userspace.plaintext));
    if (err)
    {
        pr_err("%s: failed to execute hashing function\n", DEV_NAME);
        goto out;
    }

    // Write the result to a new char buffer
    err = crypto_shash_final(desc, digest);
    if (err)
    {
        pr_err("%s: Failed to complete hashing function\n", DEV_NAME);
        goto out;
    }

    // Finally, clean up resources
    crypto_free_shash(algorithm);
    kfree(desc);

    pr_info("%s: String successfully hashed. Read from this device to get the result\n", DEV_NAME);

    return 0;

out: // Manage errors
    crypto_free_shash(algorithm);
    kfree(desc);
    return err;
}

// Register __init and __exit functions
module_init(device_init);
module_exit(device_exit);