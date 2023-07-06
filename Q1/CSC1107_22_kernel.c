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
#define BUF_SIZE 1024                // size of the text buffer

// Functions prototypes
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

// supported has types
typedef enum hash
{
    MD5 = 0,
    SHA512,
    SHA384,
    SHA256,
    SHA1
} hash_t;

// userspace struct
typedef struct userspace
{
    u8 plaintext[BUF_SIZE];
    hash_t algorithm;
    uint8_t hashtext[BUF_SIZE];
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

// global definitions
static int dev_num;                        // Major number of the character device
static struct class *csc1107_class = NULL; // Device driver class pointer
static struct device *csc1107_dev = NULL;  // Device driver pointer
static size_t open_count = 0;              // Number of types this device has been opened
static char digest[BUF_SIZE];              // Result of hashing process
static userspace_t userspace;              // Data from userspace
static size_t bytes_not_copied = 0;        // Error check
static size_t bytes_to_copy = 0;           // Size of digest

/* Initalising a character device */
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

    pr_info("%s: Device driver successfully created\n", DEV_NAME);

    return 0;
}

/* Cleanup resources of character device */
static void __exit device_exit(void)
{
    device_destroy(csc1107_class, MKDEV(dev_num, 0)); // Remove device
    class_unregister(csc1107_class);                  // Unregister the class
    class_destroy(csc1107_class);                     // Destroy the class
    unregister_chrdev(dev_num, DEV_NAME);             // Unregister device major number

    pr_info("%s: Module successfully unloaded\n", DEV_NAME);
}

/* This function is invoked each time an userspace process tries
 * to open the character device with 'open()' syscall. It is usually
 * being used to set up the environment for the rest of the module but
 * since we do not have anything to configure, it will just
 * print out a message */
static int device_open(struct inode *inodep, struct file *filep)
{
    pr_info("%s: This device has been opened %d time(s)\n", DEV_NAME, (int)++open_count);

    return 0;
}

/* This function is invoked each time an userspace process closes the
 * character device with 'close()' syscall. It is usually being used
 * to free allocated resources but since we have not anything
 * to do, it will just print out a message */
static int device_release(struct inode *inodep, struct file *filep)
{
    pr_info("%s: Device closed successfully\n", DEV_NAME);

    return 0;
}

/* This function is invoked each time the 'read()' syscall is called
 * from userspace. In this module, it will use the function 'copy_to_user'
 * to send back to the user the result (i.e., the hashed string) */
static ssize_t device_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
    // 'copy_from_user' pass data to userspace
    //  it returns 0 on success and number of bytes not copied on error
    bytes_not_copied = copy_to_user(buffer, digest, bytes_to_copy);
    // check errors
    if (bytes_not_copied)
    {
        pr_warn("%s: Failed to send result to userspace\n", DEV_NAME);
        return -EFAULT;
    }

    return 0;
}

/* This function is invoked each time the 'write()' syscall is called
 * from userspace. In this module, it will use the function 'copy_from_user'
 * to retrieve data from userspace. It will then hash the plaintext string
 * according to the chosen algorithm using Linux cryptographic APIs */
static ssize_t device_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
    int err;
    char *algo_str;
    struct crypto_shash *algorithm;
    struct shash_desc *desc;

    // 'copy_from_user' gets data from userspace
    //  it returns 0 on success and number of bytes not copied on error
    bytes_not_copied = copy_from_user(&userspace, buffer, sizeof(userspace_t));
    // check errors
    if (bytes_not_copied != 0)
    {
        pr_warn("%s: Error while copying %zu bytes from userspace\n", DEV_NAME, bytes_not_copied);
        return -EFAULT;
    }

    // select hashing algorithm
    switch (userspace.algorithm)
    {
        case MD5:
            algorithm = crypto_alloc_shash("md5", 0, 0);
            bytes_to_copy = 16; // MD5 hash digest = 128 bits = 16 bytes
            algo_str = "MD5";
            break;
        case SHA512:
            algorithm = crypto_alloc_shash("sha512", 0, 0);
            bytes_to_copy = 64; // SHA-512 hash digest = 512 bits = 64 bytes
            algo_str = "SHA512";
            break;
        case SHA384:
            algorithm = crypto_alloc_shash("sha384", 0, 0);
            bytes_to_copy = 48; // SHA-384 hash digest = 384 bits = 48 bytes
            algo_str = "SHA384";
            break;
        case SHA256:
            algorithm = crypto_alloc_shash("sha256", 0, 0);
            bytes_to_copy = 32; // SHA-256 hash digest = 256 bits = 32 bytes
            algo_str = "SHA256";
            break;
        case SHA1:
            algorithm = crypto_alloc_shash("sha1", 0, 0);
            bytes_to_copy = 20; // SHA-1 hash digest = 160 bits = 20 bytes
            algo_str = "SHA1";
            break;
        default:
            pr_alert("%s: Hashing algorithm not recognized\n", DEV_NAME);
            return -EFAULT;
    }

    // check if selected algorithm is available in the system
    if (IS_ERR(algorithm))
    {
        pr_alert("%s: Hashing algorithm not supported\n", DEV_NAME);
        return -EFAULT;
    }

    // allocate memory for descriptor/context which holds
    // information on cryptographic operations
    desc = kmalloc(sizeof(struct shash_desc) + crypto_shash_descsize(algorithm), GFP_KERNEL);
    // check errors
    if (!desc)
    {
        pr_err("%s: failed to allocate memory(kmalloc)\n", DEV_NAME);
        return -ENOMEM;
    }
    desc->tfm = algorithm;

    // initialize shash API
    err = crypto_shash_init(desc);
    if (err)
    {
        pr_err("%s: failed to initialize shash\n", DEV_NAME);
        goto out;
    }

    // execute hash function
    err = crypto_shash_update(desc, userspace.plaintext, strlen(userspace.plaintext));
    if (err)
    {
        pr_err("%s: failed to execute hashing function\n", DEV_NAME);
        goto out;
    }

    // write the result to new char buffer (digest)
    err = crypto_shash_final(desc, digest);
    if (err)
    {
        pr_err("%s: Failed to complete hashing function\n", DEV_NAME);
        goto out;
    }

    // finally, clean up resources
    crypto_free_shash(algorithm);
    kfree(desc);

    // output results
    pr_info("%s: String successfully hashed. Read from this device to get the result\n\n", DEV_NAME);
    pr_info("%s: Original Sentence                 : %s", DEV_NAME, userspace.plaintext);
    pr_info("%s: Hashed Sentence from user space   : %s", DEV_NAME, userspace.hashtext);
    pr_info("%s: Hashing Algorithm                 : %s", DEV_NAME, algo_str);
    pr_info("%s: Hashed Sentence from kernel space : ", DEV_NAME);
    for (size_t i = 0; i < bytes_to_copy; i++)
        pr_cont("%02x", digest[i]);
    pr_cont("\n\n");

    return 0;

// manage errors
out:
    crypto_free_shash(algorithm);
    kfree(desc);
    return err;
}

/* Register __init and __exit functions */
module_init(device_init);
module_exit(device_exit);