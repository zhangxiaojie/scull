#include<linux/init.h>
#include<linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");

static int showPCB_init(void)
{
	printk(KERN_ALERT "showPCB kernel insmod \n");
	return 0;
}

static void showPCB_exit(void)
{
	printk(KERN_ALERT "showPCB kernel rmmod \n");
}

module_init(showPCB_init);
module_exit(showPCB_exit);
