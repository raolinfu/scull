/* write_fifo.c*/

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO "myfifo"

int main(int argc,char** argv)
{
    /*创建有名管道*/
    if(mkfifo(FIFO,O_CREAT|O_EXCL|777)<0)
        perror("cannot create fifoserver\n");
    /*打开管道*/
    int fd=open(FIFO,O_RDWR|O_NONBLOCK,0);
    if(fd==-1)
    {
        perror("open");
    }

    char write_buf[128];
    int i = 0,write_num = 0;
    while(1)
    {
        printf("enter something to the myfifo:");
        scanf("%s",write_buf);
        if ((write_num = write(fd,write_buf,sizeof(write_buf))) < 0)
        {
            perror("write");
        }
        printf("This is a write fifo....write %s to the FIFO\n",write_buf);
    }
    return 0;
}
