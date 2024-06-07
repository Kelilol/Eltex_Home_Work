#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/mutex.h>

extern int led_state;
static struct kobject *example_kobject;
static struct mutex sysfs_mutex;

static ssize_t led_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%d\n", led_state);
}

static ssize_t led_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    int temp;
    if (sscanf(buf, "%du", &temp) == 1) {
        mutex_lock(&sysfs_mutex);
        led_state = temp;
        mutex_unlock(&sysfs_mutex);
    }
    return count;
}

static struct kobj_attribute led_attribute = __ATTR(led_state, 0660, led_show, led_store);

static int __init my_sysfs_init(void) {
    int error = 0;
    pr_debug("sysfs_module: initializing\n");

    mutex_init(&sysfs_mutex);

    example_kobject = kobject_create_and_add("led_control", kernel_kobj);
    if (!example_kobject) return -ENOMEM;

    error = sysfs_create_file(example_kobject, &led_attribute.attr);
    if (error) pr_debug("sysfs_module: failed to create sysfs file\n");

    return error;
}

static void __exit my_sysfs_exit(void) {
    pr_debug("sysfs_module: exiting\n");
    kobject_put(example_kobject);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Edigaryev");
module_init(my_sysfs_init);
module_exit(my_sysfs_exit);


