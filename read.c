#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
int main()
{
    int fd,i;
	char hello[20] = "helloworld\n";
    fd= open("/dev/raolinhu0",O_RDWR,S_IRUSR|S_IWUSR);
	int flag = 1;
    if(fd!=-1)
    {
		while(flag){
			char msg[101] = {0};
			int cnt = read(fd,msg,20);
			printf("cnt:%d\n",cnt);
			printf("%s\n",msg);
		}
		close(fd);
    }
    else
    {
        printf("device open failure,%d\n",fd);
    }
    return 0;
}
