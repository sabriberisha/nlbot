/*
 * nlbot v0.1alpha
 *
 * nio.c
 *
 * (c) sabri@cluecentral.net
 *
 * last revision: Fri May 16 12:14:28 CEST 2003
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include "include.h"

void handler(signal)
int signal;
{
  if(signal==SIGALRM)
  {
    fcntl(server.fd, F_SETFL, O_NONBLOCK);
    return;
  }
  return;
}

void netprintf(const char *fmt, ...)
{
  char p[MAXBUFLEN];
  va_list ap;
  bzero(p,MAXBUFLEN);
  va_start(ap, fmt);
  (void) vsnprintf(p, MAXBUFLEN-3, fmt, ap);
  va_end(ap);
  p[strlen(p)]='\r';
  p[strlen(p)]='\n';
  write(server.fd,p,strlen(p));
}

int netread(char *buffer, int buflen, int fd)
{
  char in;
  int num=0,i=0;
  signal(SIGALRM, handler);
  bzero(buffer,buflen);
  do
  {
    alarm(1800);
    if(read(fd,&in,1)==1)
      buffer[i]=in;
    else
      num=-1;
    alarm(0);
    if(i>0)
      if(buffer[i]=='\n' && buffer[i-1]=='\r')
	num=1;
    i++;
  } while(num==0);
  if(num==-1)
    return(0);
  buffer[i]='\0';
  if(i>0)
    buffer[i-1]='\0';
  if(buffer[i]=='\r' || buffer[i]=='\n')
    buffer[i-1]='\0';
  return(1);
}
