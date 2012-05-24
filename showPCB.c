#include<linux/cdev.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");

static int blan_open(struct inode*inode,struct file*filp){
	printk(KERN_WARNING "blan:open!\n");
	return 0;
}
static int blan_release(struct inode*inode,struct file*filp){
	printk(KERN_WARNING "blan:release!\n");
	return 0;
}


int blan_major,blan_minor=0,blan_count=1;
struct file_operations blan_fops ={
	.owner = THIS_MODULE,
//	.read = blan_read,
//	.write = blan_write,
	.open = blan_open,
	.release = blan_release,
};

static int showPCB_init(void)
{
	int err,devno;
	struct cdev *blan_cdev;	
	dev_t dev = 0;
	err=alloc_chrdev_region(&dev,blan_minor,blan_count,"blan");	
	blan_major = MAJOR(dev);
	if(err<0) {
		printk(KERN_WARNING "blan:can't get major %d\n",blan_major);
		return err;
	}
	devno = MKDEV(blan_major,blan_minor);
	printk(KERN_WARNING "blan:devno=%d\n",devno);
	//cdev_init(&dev->cdev,null);
	blan_cdev=cdev_alloc();	
	blan_cdev->ops=&blan_fops;
	err=cdev_add(blan_cdev,devno,1);
	if(err<0) {
		printk(KERN_WARNING "blan:can't add dev\n");
		return err;
	}
	return 0;
}

static void showPCB_exit(void)
{
	dev_t devno = MKDEV(blan_major,blan_minor);
	unregister_chrdev_region(devno,1);
}

module_init(showPCB_init);
module_exit(showPCB_exit);
