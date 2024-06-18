#include <linux/module.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/kd.h>
#include <linux/vt.h>
#include <linux/console_struct.h>
#include <linux/vt_kern.h>
#include <linux/timer.h>
#include <linux/mutex.h>

int led_state = 0;
EXPORT_SYMBOL(led_state);  // Экспортируем символ led_state

static struct timer_list my_timer;
static struct tty_driver *my_driver;
static struct mutex led_mutex;

#define BLINK_DELAY   HZ/5
#define RESTORE_LEDS  0xFF

static void my_timer_func(struct timer_list *ptr) {
    static int state = 0;

    mutex_lock(&led_mutex);
    state = led_state;
    mutex_unlock(&led_mutex);

    (my_driver->ops->ioctl)(vc_cons[fg_console].d->port.tty, KDSETLED, state);
    mod_timer(&my_timer, jiffies + BLINK_DELAY);
}

static int __init led_init(void) {
    int i;

    pr_info("led_module: loading\n");
    for (i = 0; i < MAX_NR_CONSOLES; i++) {
        if (!vc_cons[i].d)
            break;
        pr_info("led_module: console[%i/%i] #%i, tty %lx\n", i,
               MAX_NR_CONSOLES, vc_cons[i].d->vc_num,
               (unsigned long)vc_cons[i].d->port.tty);
    }

    my_driver = vc_cons[fg_console].d->port.tty->driver;

    mutex_init(&led_mutex);
    timer_setup(&my_timer, my_timer_func, 0);
    mod_timer(&my_timer, jiffies + BLINK_DELAY);
    return 0;
}

static void __exit led_exit(void) {
    pr_info("led_module: unloading\n");
    del_timer(&my_timer);
    (my_driver->ops->ioctl)(vc_cons[fg_console].d->port.tty, KDSETLED, RESTORE_LEDS);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Edigaryev");
module_init(led_init);
module_exit(led_exit);


