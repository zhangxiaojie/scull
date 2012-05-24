#include<linux/cdev.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");

static int scull_open(struct inode*inode,struct file*filp){
	printk(KERN_WARNING "scull:open!\n");
	return 0;
}
static int scull_release(struct inode*inode,struct file*filp){
	printk(KERN_WARNING "scull:release!\n");
	return 0;
}


int scull_major,scull_minor=0,scull_count=1;
struct file_operations scull_fops ={
	.owner = THIS_MODULE,
//	.read = scull_read,
//	.write = scull_write,
	.open = scull_open,
	.release = scull_release,
};

static int scull_init(void)
{
	int err,devno;
	struct cdev *scull_cdev;	
	dev_t dev = 0;
	err=alloc_chrdev_region(&dev,scull_minor,scull_count,"scull");	
	scull_major = MAJOR(dev);
	if(err<0) {
		printk(KERN_WARNING "scull:can't get major %d\n",scull_major);
		return err;
	}
	devno = MKDEV(scull_major,scull_minor);
	printk(KERN_WARNING "scull:devno=%d\n",devno);
	//cdev_init(&dev->cdev,null);
	scull_cdev=cdev_alloc();	
	scull_cdev->ops=&scull_fops;
	err=cdev_add(scull_cdev,devno,1);
	if(err<0) {
		printk(KERN_WARNING "scull:can't add dev\n");
		return err;
	}
	return 0;
}

static void scull_exit(void)
{
	dev_t devno = MKDEV(scull_major,scull_minor);
	unregister_chrdev_region(devno,1);
}

module_init(scull_init);
module_exit(scull_exit);
