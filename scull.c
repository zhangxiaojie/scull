#include<asm/uaccess.h>
#include<linux/cdev.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");

struct scull_dev{
	struct cdev cdev;
};
struct scull_dev *scull_devices;
static char scull_buffer[256];

int scull_major,scull_minor=0,scull_count=1,scull_inc=0;

static int scull_open(struct inode*inode,struct file*filp){
	printk(KERN_WARNING "scull:open!\n");
	struct scull_dev * dev;
	//lock
	if (scull_inc>0) return -ERESTARTSYS;
	scull_inc++;
	dev = container_of(inode->i_cdev,struct scull_dev,cdev);
	filp->private_data = dev;
	return 0;
}
static int scull_release(struct inode*inode,struct file*filp){
	printk(KERN_WARNING "scull:release!\n");
	scull_inc--;
	return 0;
}

ssize_t scull_read(struct file *filp,char __user *buf,size_t count,loff_t *f_pos)
{
	int result;
	loff_t pos = *f_pos;
	if(pos>=256)
	{
		result = 0;
		goto out;
	}
	if(count>(256-pos))
	{
		count = 256-pos;
	}
	pos +=count;
	if(copy_to_user(buf,scull_buffer+*f_pos,count))
	{
		count = -EFAULT;
		goto out;
	}
	*f_pos = pos;
out:
	return count;
}

ssize_t scull_write(struct file *filp,const char __user *buf,size_t count,loff_t *f_pos)
{
	ssize_t retval = -ENOMEM;
	loff_t pos = *f_pos;
	if(pos>=256)
	{
		goto out;
	}	
	if(count>(256-pos))
	{
		count = 256-pos;
	}
	pos +=count;
	if(copy_from_user(scull_buffer+*f_pos,buf,count)){
		retval = -EFAULT;
		goto out;
	}	
	*f_pos = pos;
	return count;
out:
	return retval;
}

struct file_operations scull_fops ={
	.owner = THIS_MODULE,
	.read = scull_read,
	.write = scull_write,
	.open = scull_open,
	.release = scull_release,
};


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
