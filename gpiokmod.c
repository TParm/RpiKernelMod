#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

//** Variables **//
static int toggleSpeed = 1;
static int ioPins[2] = {20, 21};
static int arr_argc = 0;
static int toggleCounter = 0;
static int divider = 0;

module_param(toggleSpeed, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(toggleSpeed, "An integer");

module_param_array(ioPins, int, &arr_argc, 0000);
MODULE_PARM_DESC(ioPins, "An array of integers");

static struct timer_list blink_timer;
static long led1 = 0;
static long led2 = 0;

/* Define GPIOs for LEDs */
static struct gpio leds[] = {
	{4, GPIOF_OUT_INIT_LOW, "LED 1"},
};

/*
 * Timer function called periodically
 */
static void blink_timer_func(struct timer_list *t)
{
	toggleCounter++;
	divider = toggleCounter/2;

	printk(KERN_INFO "Leds toggled %d times\n", divider);
	gpio_set_value(ioPins[0], led1);
	gpio_set_value(ioPins[1], led2);
	
	led1 = !led1;
	led2 = !led2;

	/* schedule next execution */
	blink_timer.expires = jiffies + (toggleSpeed * HZ); // 1 sec.
	add_timer(&blink_timer);
}

/*
 * Module init function
 */
static int __init gpiokmod_init(void)
{
	int i;
	int ret = 0;

	printk(KERN_INFO "Start module 'gpiokmod'\n");
	printk(KERN_INFO "Selected togglespeed: %d\n", toggleSpeed);

	printk(KERN_INFO "Selected gpio pins: \n");
	for (i = 0; i < (sizeof ioPins / sizeof(int)); i++)
	{
		printk(KERN_INFO "myGpioArray[%d] = %d\n", i, ioPins[i]);
	}

	printk(KERN_INFO "got %d arguments for myGpioArray.\n", arr_argc);

	printk(KERN_INFO "%s\n", __func__);

	// register, turn off
	ret = gpio_request_one(ioPins[0], GPIOF_OUT_INIT_LOW, "ioPins[0]");
	ret = gpio_request_one(ioPins[1], GPIOF_OUT_INIT_LOW, "ioPins[1]");

	if (ret)
	{
		printk(KERN_ERR "Unable to request GPioPins: %d\n", ret);
		return ret;
	}

	/* init timer, add timer function */
	//init_timer(&blink_timer);
	timer_setup(&blink_timer, blink_timer_func, 0);

	blink_timer.function = blink_timer_func;
	//blink_timer.data = 1L;							// initially turn LED on
	blink_timer.expires = jiffies + (toggleSpeed * HZ); // 1 sec.
	add_timer(&blink_timer);

	/* -------- From gpiomod_inpirq.c -------- */
	// register LED gpios
	ret = gpio_request_array(leds, ARRAY_SIZE(leds));

	if (ret)
	{
		printk(KERN_ERR "Unable to request GPIOs for LEDs: %d\n", ret);
		return ret;
	}

 	return ret;
}

/*
 * Exit function
 */
static void __exit gpiokmod_exit(void)
{
	int i;

	printk(KERN_INFO "%s\n", __func__);

	// deactivate timer if running
	del_timer_sync(&blink_timer);

	// turn off LED
	gpio_set_value(ioPins[0], 0);
	gpio_set_value(ioPins[1], 0);

	// unregister GPIO
	gpio_free(ioPins[0]);
	gpio_free(ioPins[1]);

	// turn off LEDS
	for (i = 0; i < ARRAY_SIZE(leds); i++)
	{
		gpio_set_value(leds[i].gpio, 0);
	}

	// unregister
	gpio_free_array(leds, ARRAY_SIZE(leds));
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TP");
MODULE_DESCRIPTION("Basic Linux Kernel module taking command line arguments");

module_init(gpiokmod_init);
module_exit(gpiokmod_exit);