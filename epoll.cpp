#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/epoll.h>
#include <vector>

typedef std::vector<struct epoll_event> PollFdList;

int main(int argc ,char **argv)
{

  int fd;
  char buf[1024];
  int i,res,real_read, maxfd;

  if((fd=open("/dev/stdin",O_RDONLY|O_NONBLOCK)) < 0)
  {
    fprintf(stderr,"open data1 error:%s",strerror(errno));
    return 1;
  }

  PollFdList m_pollfds;
  int epfd = epoll_create1(EPOLL_CLOEXEC);

  struct epoll_event ev;
  ev.events = EPOLLIN | EPOLLPRI;
  ev.data.fd = fd;

  epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
  
  m_pollfds.resize(1024);

  while(1)
  {
    int ret = epoll_wait(epfd, m_pollfds.data(), m_pollfds.size(), 5000);
    if (ret < 0)
    {
      printf("ePoll error : %s\n",strerror(errno));
      return 1;
    }

    if(ret == 0){
      printf("ePoll timeout\n");
      continue;
    }

    for (i = 0; i< 1; i++)
    {
      if (m_pollfds[i].events & EPOLLIN)
      {
        memset(buf, 0, 1024);
        real_read = read(m_pollfds[i].data.fd, buf, 1024);
        if (real_read < 0)
        {
          if (errno != EAGAIN)
          {
            printf("read eror : %s\n",strerror(errno));
            continue;
          }
        }
        else if (!real_read)
        {
          close(m_pollfds[i].data.fd);
          m_pollfds[i].events = 0;
        }
        else
        {
          if (i == 0)
          {
            buf[real_read] = '\0';
            printf("%s", buf);
            if ((buf[0] == 'q') || (buf[0] == 'Q'))
            {
              printf("quit\n");
              return 1;
            }
          }
          else
          {
            buf[real_read] = '\0';
            printf("%s", buf);
          }
        }
      }
    }
  }

  exit(0);
}
