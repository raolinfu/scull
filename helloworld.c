#include <linux/init.h>
#include<linux/fs.h>
#include<linux/printk.h>
#include<linux/cdev.h>
#include<linux/semaphore.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include"scull.h"

int scull_major;
int scull_minor;
loff_t scull_llseek(struct file * filp, loff_t off, int b){
	return off;
}
ssize_t scull_read(struct file *filp, char __user* buff, size_t count, loff_t *offp){
	return 0;
}
ssize_t  scull_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp){
	return 0;
}
int scull_ioctl(struct inode *inode, struct file *flip, unsigned int cmd, unsigned long b){
	return 0;
}
int scull_open(struct inode *inode, struct file *flip){
	struct scull_dev *dev;
	dev = container_of(inode->i_cdev, struct scull_dev, cdev);
	flip->private_data = dev;
	if((flip->f_flags & O_ACCMODE) == O_WRONLY){
		
	}
	return 0;
}
int scull_release(struct inode *inode, struct file *filp){
	return 0;
}

struct file_operations scull_fops = {
	.owner = THIS_MODULE,
	.llseek = scull_llseek,
	.read = scull_read,
	.write = scull_write,
	.open = scull_open,
	.release = scull_release,
};
static void scull_setup_dev(struct scull_dev *dev, int index){
	int err, devno = MKDEV(scull_major, scull_minor + index);
	cdev_init(&dev->cdev, &scull_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &scull_fops;
	err = cdev_add(&dev->cdev, devno, 1);
	if(err)
		printk(KERN_NOTICE "Error %d adding scull %d", err, index);
}
static int __init lkm_init(void)
{
	printk("Hello World\n");
	return 0;
}

static void __exit lkm_exit(void)
{
	printk("Goodbye\n");
}

module_init(lkm_init);
module_exit(lkm_exit);
MODULE_LICENSE("GPL");

