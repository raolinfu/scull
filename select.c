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

	while(1){
		int ret;
		struct timeval tv;
		tv.tv_sec = 60;
		tv.tv_usec = 0;
		fd_set read_fds;
		FD_ZERO(&read_fds);
		FD_SET(fd,&read_fds);
		/** read_fds write_fds = NULL excep_fds = NULL */
		ret = select(fd+1, &read_fds, NULL, NULL,&tv);

		if (ret == -1){
			perror("select error");
		}
		else if (ret){
			char msg[101] = {0};
			int cnt = read(fd,msg,20);
			printf("read content: %s\n", msg);
		}
		else if (ret == 0){
			printf("time out\n");
		}
	}

    return 0;
}
