#include<sys/types.h>
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
    char msg[101];
	char hello[20] = "helloworld";
    fd= open("/dev/raolinhu0",O_RDWR,S_IRUSR|S_IWUSR);
	ioctl(fd, TEST_CLEAR); 
	int flag = 1;
    if(fd!=-1)
    {
		while(flag){
			scanf("%s", hello);
			write(fd, hello, strlen(hello));
		}
		close(fd);
    }
    else
    {
        printf("device open failure,%d\n",fd);
    }
    return 0;
}
