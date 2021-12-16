/* read_fifo.c*/

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#define FIFO "myfifo"

int main(int argc,char** argv)
{
    /*创建有名管道*/
    if(mkfifo(FIFO,O_CREAT|O_EXCL|777)<0)
        perror("cannot create fifoserver\n");
    /*打开管道,默认阻塞模式*/
    int fd = open(FIFO,O_RDONLY,0);
    //int fd = open(FIFO,O_RDONLY|O_NONBLOCK,0);//非阻塞模式
    if(fd == -1)
    {
        perror("open");
    }

    char read_buf[128];
    int i = 0,read_num;
    while(1)
    {
        memset(read_buf,0,sizeof(read_buf));
        if (read_num = read(fd,read_buf,sizeof(read_buf)) < 0)
        {
            if(errno==EAGAIN)
                printf("no data yet\n");
        }
        printf("This is a read fifo....read %s from FIFO\n",read_buf);
        //sleep(2);
    }
    return 0;
}
