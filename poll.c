#include<sys/types.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include<sys/poll.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<stdio.h>
#include<fcntl.h>
#include<string.h>

#define TEST_MAGIC 'r' //定义幻数
#define TEST_MAX_NR 2 //定义命令的最大序数
#define TEST_CLEAR _IO(TEST_MAGIC, 1)
#define TEST_OFFSET _IO(TEST_MAGIC, 2)

int main()
{
    int fd,i;
    fd= open("/dev/raolinhu0",O_RDWR, O_NONBLOCK);
    struct pollfd pfd;
    int n;

    pfd.fd = fd;  /* stdin */
	pfd.events = ( POLLIN | POLLRDNORM | POLLOUT | POLLWRNORM );

    while (1) {
		n = poll(&pfd, 1, -1);
    
		if( ( pfd.revents & POLLIN )  == POLLIN )
		{
		  printf("Kernel_val POLLIN\n");
		}
		if( ( pfd.revents & POLLRDNORM )  == POLLRDNORM )
		{
		  printf("Kernel_val POLLRDNORM\n");
		}
		if( ( pfd.revents & POLLOUT )  == POLLOUT )
		{
		  printf("Kernel_val POLLOUT\n");
		}
		if( ( pfd.revents & POLLWRNORM )  == POLLWRNORM )
		{
		  printf("Kernel_val POLLWRNORM\n");
		}
		sleep(1);

	    printf("user poll\n"); 
		if (n < 0)
			break;
    }
/**     printf("fd:%d\n", fd); 
  *     int ret;
  *     struct timeval tv;
  *     while(1){
  *            printf("user poll\n");
  *         tv.tv_sec = 60;
  *         tv.tv_usec = 0;
  *         fd_set fds;
  *         FD_ZERO(&fds);
  *         FD_SET(fd,&fds);
  *         ret = select(fd+1, &fds, NULL, NULL,&tv);
  *
  *         if (ret == -1)
  *              perror("select error");
  *         else if (ret)
  *               printf("return success\n");
  *         else if (ret == 0)
  *             printf("time out\n");
  *     }
  *  */
    return 0;
}
