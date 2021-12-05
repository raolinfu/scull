#include<linux/init.h>
#include<linux/slab.h>		/* kmalloc() */
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/printk.h>
#include<linux/cdev.h>
#include<linux/semaphore.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include"scull.h"

int scull_major =   SCULL_MAJOR;
int scull_minor =   0;
int scull_nr_devs = SCULL_NR_DEVS;	/* number of bare scull devices */
int scull_quantum = SCULL_QUANTUM;
int scull_qset =    SCULL_QSET;
struct scull_dev *scull_devices;	/* allocated in scull_init_module */

struct scull_qset{
	void **data;
	struct scull_qset *next;
};
struct scull_dev{
	struct scull_qset *data;
	int quantum;
	int qset;
	unsigned long size;
	unsigned int access_key;
	struct semaphore sem;
	struct cdev cdev;
};

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
int scull_trim(struct scull_dev *dev){
	struct scull_qset *next, *dptr;
	int qset = dev->qset;
	int i;
	for(dptr = dev->data; dptr; dptr = next){
		if(dptr->data){
			for(i = 0; i < qset; i++)
				kfree(dptr->data[i]);
			kfree(dptr->next);
			dptr->data = NULL;
		}
		next = dptr->next;
		kfree(dptr);
	}
	dev->size = 0;
	// dev->quantum = scull_quantum;
	// dev->qset = scull_qset;
	dev->data = NULL;
	return 0;
}
int scull_open(struct inode *inode, struct file *flip){
	struct scull_dev *dev;
	dev = container_of(inode->i_cdev, struct scull_dev, cdev);
	flip->private_data = dev;
	if((flip->f_flags & O_ACCMODE) == O_WRONLY){
		scull_trim(dev);		
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
	int result, i;
	dev_t dev = 0;

	if (scull_major) {
		dev = MKDEV(scull_major, scull_minor);
		result = register_chrdev_region(dev, scull_nr_devs, "scull");
	} else {
		result = alloc_chrdev_region(&dev, scull_minor, scull_nr_devs,
				"scull");
		scull_major = MAJOR(dev);
	}
	if (result < 0) {
		printk(KERN_WARNING "scull: can't get major %d\n", scull_major);
		return result;
	}

	scull_devices = kmalloc(scull_nr_devs * sizeof(struct scull_dev), GFP_KERNEL);
	if (!scull_devices) {
		result = -ENOMEM;
		goto fail;  /* Make this more graceful */
	}
	memset(scull_devices, 0, scull_nr_devs * sizeof(struct scull_dev));

        /* Initialize each device. */
	for (i = 0; i < scull_nr_devs; i++) {
		scull_devices[i].quantum = scull_quantum;
		scull_devices[i].qset = scull_qset;
		mutex_init(&scull_devices[i].lock);
		scull_setup_cdev(&scull_devices[i], i);
	}

        /* At this point call the init function for any friend device */
	dev = MKDEV(scull_major, scull_minor + scull_nr_devs);
	dev += scull_p_init(dev);
	dev += scull_access_init(dev);

#ifdef SCULL_DEBUG /* only when debugging */
	scull_create_proc();
#endif

	return 0; /* succeed */

  fail:
	scull_cleanup_module();
	return result;
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

