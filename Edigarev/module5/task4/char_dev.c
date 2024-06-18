#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h> /* for put_user */

#define DEVICE_NAME "chardev"
#define BUF_LEN 80

static int major;
static char msg[BUF_LEN] = {0};
static struct class *cls;
static struct cdev c_dev;

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char __user *, size_t, loff_t *);

static struct file_operations chardev_fops = {
    .owner = THIS_MODULE,
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
};

static int __init chardev_init(void) {
    dev_t dev;

    /* Allocate a major number for the device */
    if (alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME) < 0) {
        return -1;
    }
    major = MAJOR(dev);

    /* Create device class */
    if ((cls = class_create(DEVICE_NAME)) == NULL) {
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    /* Create device */
    if (device_create(cls, NULL, dev, NULL, DEVICE_NAME) == NULL) {
        class_destroy(cls);
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    /* Initialize the cdev structure and add it to the kernel */
    cdev_init(&c_dev, &chardev_fops);
    if (cdev_add(&c_dev, dev, 1) == -1) {
        device_destroy(cls, dev);
        class_destroy(cls);
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    pr_info("chardev: registered with major number %d\n", major);
    return 0;
}

static void __exit chardev_exit(void) {
    dev_t dev = MKDEV(major, 0);
    cdev_del(&c_dev);
    device_destroy(cls, dev);
    class_destroy(cls);
    unregister_chrdev_region(dev, 1);
    pr_info("chardev: unregistered\n");
}

static int device_open(struct inode *inode, struct file *file) {
    pr_info("chardev: device opened\n");
    return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    pr_info("chardev: device closed\n");
    return 0;
}

static ssize_t device_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset) {
    int bytes_read = 0;
    const char *msg_ptr = msg;

    if (*offset >= strlen(msg)) {
        return 0;
    }

    if (length > strlen(msg) - *offset) {
        length = strlen(msg) - *offset;
    }

    if (copy_to_user(buffer, msg_ptr + *offset, length) != 0) {
        return -EFAULT;
    }

    *offset += length;
    bytes_read = length;
    pr_info("chardev: device read %d bytes\n", bytes_read);

    return bytes_read;
}

static ssize_t device_write(struct file *filp, const char __user *buff, size_t len, loff_t *off) {
    if (len > BUF_LEN - 1) {
        len = BUF_LEN - 1;
    }

    memset(msg, 0, BUF_LEN);
    if (copy_from_user(msg, buff, len) != 0) {
        return -EFAULT;
    }
    msg[len] = '\0';

    pr_info("chardev: device written %ld bytes\n", len);

    return len;
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Edigaryev");

MODULE_VERSION("1.0");
