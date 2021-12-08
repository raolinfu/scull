#include<linux/wait.h>
#include<linux/cdev.h>
#ifndef SCULL_MAJOR
#define SCULL_MAJOR 0   /* dynamic major by default */
#endif
#ifndef SCULL_NR_DEVS
#define SCULL_NR_DEVS 4    /* scull0 through scull3 */
#endif

#ifndef SCULL_P_NR_DEVS
#define SCULL_P_NR_DEVS 4  /* scullpipe0 through scullpipe3 */
#endif

#ifndef SCULL_QUANTUM
#define SCULL_QUANTUM 4000
#endif

#ifndef SCULL_QSET
#define SCULL_QSET    1000
#endif

#define SCULL_IOC_MAGIC  'r'
#define SCULL_IOC_MAXNR 14

#define TEST_CLEAR _IO(SCULL_IOC_MAGIC, 1)
#define TEST_OFFSET _IO(SCULL_IOC_MAGIC, 2)

struct scull_dev {
	wait_queue_head_t inq, outq;       /* read and write queues */
	char buff[1000];
	struct cdev cdev;
};
