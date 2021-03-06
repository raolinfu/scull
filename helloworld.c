#include<linux/init.h>
#include<linux/poll.h>
#include<linux/wait.h>
#include<linux/slab.h>		/* kmalloc() */
#include <linux/moduleparam.h>
#include<linux/stat.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/printk.h>
#include<linux/cdev.h>
#include<linux/semaphore.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include"scull.h"

int scull_major = SCULL_MAJOR;
int scull_minor = 0;
int flag = 1; //表示可写
int write_offset = 0;
int read_offset = 0;

static int nums = 1;		// 有多少设备

/** module_param(scull_major, int, S_IRUGO);
  * module_param(scull_minor, int, S_IRUGO); */
struct scull_dev *scull_devices;	/* 设备数组 */

loff_t scull_llseek(struct file * filp, loff_t off, int b){
	return off;
}

static unsigned int scull_poll(struct file *filp, poll_table *wait)
{
	struct scull_dev *dev = filp->private_data;
	unsigned int mask = 0;

	// 如果等待队列inq状态变了 会再次调用scull_poll
	// 其实是每次调用scull_poll时都注册一下等待队列 等待队列状态变化了又来
	poll_wait(filp, &dev->inq,  wait);
	// poll_wait(filp, &dev->outq, wait);
	// mask |= POLLOUT | POLLWRNORM;	/* writable */
	// flag == 0表示可读
	if(flag == 0)
		mask |= POLLIN | POLLRDNORM;	/* readable */
	printk("poll raolinhu\n");
	return mask;
}

// 读取操作
static ssize_t scull_read(struct file *filp, char __user* buf, size_t count, loff_t *offp){
	struct scull_dev *dev = filp->private_data;
	char *str = dev->buff;
	// char str[20] = "hello world";
	ssize_t retval = 0;
	// 读进程休眠
	wait_event_interruptible(dev->inq, flag == 0);

	/** if (mutex_lock_interruptible(&dev->lock))
	  *     return -ERESTARTSYS; */
	mutex_lock(&dev->lock);
	flag = 1;
	read_offset = write_offset > count? count: write_offset;

	if (copy_to_user(buf, str, read_offset)) {
		retval = -EFAULT;
	}
	printk("read from raolinhu\n");
	mutex_unlock(&dev->lock);

	return retval;
}
static ssize_t  scull_write(struct file *filp, const char __user *buf, size_t count, loff_t *offp){
	struct scull_dev *dev = filp->private_data;
	char *str = dev->buff;
	ssize_t retval = -ENOMEM;
	write_offset = count > 100? 100: count;
	printk("write_offset raolinhu:%u\n", write_offset);

	if (copy_from_user(str, buf, write_offset)) {
		printk("copy to raolinhu wrong count:%lu\n", count);
		retval = -EFAULT;
		return retval;
	}

	mutex_lock(&dev->lock);
	flag = 0;
	// 写进程唤醒读进程
	wake_up_interruptible(&dev->inq); 
	mutex_unlock(&dev->lock);

	return retval;
}

// 打开操作
static int scull_open(struct inode *inode, struct file *filp){
	struct scull_dev *dev;

	dev = container_of(inode->i_cdev, struct scull_dev, cdev);
	filp->private_data = dev;

	if ( (filp->f_flags & O_ACCMODE) == O_WRONLY) {
	}

	return 0;
}
long scull_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int retval = 0;
    
	if (_IOC_TYPE(cmd) != SCULL_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > SCULL_IOC_MAXNR) return -ENOTTY;
	switch(cmd) {
		case TEST_CLEAR:
			printk("clear ioctl\n");
			break;
		case TEST_OFFSET:
			printk("offset ioctl\n");
			break;
		default:
			return -ENOTTY;

	}
	return retval;
}

static int scull_release(struct inode *inode, struct file *filp){
	return 0;
}

struct file_operations scull_fops = {
	.owner = THIS_MODULE,
	.llseek = scull_llseek,
	.read = scull_read,
	.poll =	scull_poll,
	.write = scull_write,
	.open = scull_open,
	.unlocked_ioctl = scull_ioctl,
	.release = scull_release,
};

// 初始化每个设备
static void scull_setup_cdev(struct scull_dev *dev, int index)
{
	int err, devno = MKDEV(scull_major, scull_minor + index);
	cdev_init(&dev->cdev, &scull_fops);
	dev->cdev.owner = THIS_MODULE;
	err = cdev_add(&dev->cdev, devno, 1);

	if (err)
		printk(KERN_NOTICE "Error %d adding scull%d", err, index);
}

// 模块初始化
static int init_func(void)
{
	dev_t dev = 0;
	int result, i;
	// 注册设备号
	if(scull_major){
		dev = MKDEV(scull_major, scull_minor);
		result = register_chrdev_region(dev, nums, "raolinhu");	
	}else{
		result = alloc_chrdev_region(&dev, scull_minor, nums, "raolinhu");	
		scull_major = MAJOR(dev);
	}
	if(result < 0){
		printk("fail\n");
		return result;
	}
	scull_devices = kmalloc(nums * sizeof(struct scull_dev), GFP_KERNEL);
	if (!scull_devices) {
		result = -ENOMEM;
	}
	memset(scull_devices, 0, nums * sizeof(struct scull_dev));

	// 初始化每个设备 初始化等待队列
	for (i = 0; i < nums; i++) {
		init_waitqueue_head(&(scull_devices[i].inq));
		init_waitqueue_head(&(scull_devices[i].outq));
		mutex_init(&scull_devices[i].lock);
		scull_setup_cdev(&scull_devices[i] , i);
	}
	printk("init driver finished\n");
	return 0;
}

// 模块清理
static void exit_func(void)
{
	int i;
	dev_t devno = MKDEV(scull_major, scull_minor);

	/* Get rid of our char dev entries */
	if (scull_devices) {
		for (i = 0; i < nums; i++) {
			cdev_del(&scull_devices[i].cdev);
		}
		kfree(scull_devices);
	}
	unregister_chrdev_region(devno, nums);
	printk("clean up driver finished\n");
}

module_init(init_func);
module_exit(exit_func);
MODULE_LICENSE("GPL");

