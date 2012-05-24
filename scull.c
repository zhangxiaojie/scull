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

struct file_operations scull_fops ={
	.owner = THIS_MODULE,
//	.read = scull_read,
//	.write = scull_write,
	.open = scull_open,
	.release = scull_release,
};

struct scull_dev{
	struct cdev cdev;
};

int scull_major,scull_minor=0,scull_count=1;
struct scull_dev *scull_devices;


static int scull_init(void)
{
	int err,devno;
	dev_t dev = 0;
	scull_devices = kmalloc(scull_count*sizeof(struct scull_dev),GFP_KERNEL);
	err=alloc_chrdev_region(&dev,scull_minor,scull_count,"scull");	
	scull_major = MAJOR(dev);
	if(err<0) {
		printk(KERN_WARNING "scull:can't get major %d\n",scull_major);
		return err;
	}
	devno = MKDEV(scull_major,scull_minor);
	printk(KERN_WARNING "scull:devno=%d\n",devno);
	cdev_init(&scull_devices->cdev,&scull_fops);
	scull_devices->cdev.owner=THIS_MODULE;
	scull_devices->cdev.ops = &scull_fops;
	err=cdev_add(&scull_devices->cdev,devno,1);
	if(err<0) {
		printk(KERN_WARNING "scull:can't add dev\n");
		return err;
	}
	return 0;
}

static void scull_exit(void)
{
	dev_t devno = MKDEV(scull_major,scull_minor);
	cdev_del(&scull_devices->cdev);
	unregister_chrdev_region(devno, scull_count);
}

module_init(scull_init);
module_exit(scull_exit);
