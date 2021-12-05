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
	char hello[20] = "helloworld\n";
    fd= open("/dev/raolinhu0",O_RDWR,S_IRUSR|S_IWUSR);
    if(fd!=-1)
    {
		/** for(i=0;i<101;i++)
		  *     msg[i]='\0';
		  * read(fd,msg,100);
		  * printf("%s\n",msg); */
		read(fd,msg,20);
		printf("%s\n",msg);
		write(fd, hello, strlen(hello));
		// write(fd, hello, 5);
		close(fd);
    }
    else
    {
        printf("device open failure,%d\n",fd);
    }
    return 0;
}
