#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
int main()
{
    int fd,i;
    char msg[101];
	char hello[20] = "helloworld";
    fd= open("/dev/raolinhu0",O_RDWR,S_IRUSR|S_IWUSR);
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
