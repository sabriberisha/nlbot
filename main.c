/*
 * nlbot v0.1alpha
 *
 * main.c
 *
 * (c) sabri@cluecentral.net
 *
 * last revision: Fri May 16 12:14:28 CEST 2003
 */

#include "include.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>

int myconnect();
int netread();
int chopline();
int chopuserhost();
void netprintf();
void my_exec();
char timebuf[15];

char *gettime()
{
  struct timeval ti;
  struct tm *time;
  gettimeofday(&ti,NULL);
  time=localtime(&ti.tv_sec);
  bzero(timebuf,sizeof(timebuf));
  snprintf(timebuf,15,"%.2d:%.2d",time->tm_hour,time->tm_min);
  return(timebuf);
}

void usage(char *myname)
{
  printf("usage: %s -n nick -u user -r realname -h host -c channel\n", myname);
  exit(1);
}

int main(int argc, char *argv[])
{
  char buffer[MAXBUFLEN];
  struct parse line;
  extern char *optarg;
  int opt;
  if(argc < 11)
    usage(argv[0]);
  while ((opt = getopt(argc,argv, "c:h:n:r:u:")) != -1)
  {
    switch(opt)
    {
      case 'c':
	channel=optarg;
	break;
      case 'h':
	server.host=optarg;
	break;
      case 'n':
	snprintf(me.nick,MAXNICKLEN,"%s",optarg);
	break;
      case 'r':
	me.realname=optarg;
	break;
      case 'u':
	me.user=optarg;
	break;
    }
  }
  me.status=0;
  server.fd=-1;
  do
  {
    if(server.fd != -1)
      close(server.fd);
    server.fd=myconnect(server.host,"6666");
    while(!(server.status & (ME_REG)))
    {
      netprintf("USER %s %s %s :%s", me.user,me.user,me.user,me.realname);
      netprintf("NICK %s", me.nick);
      while(!server.status & (ME_REG))
      {
        if(!netread(buffer,sizeof(buffer),server.fd))
	{
	  printf("Could not read\n");
	  exit(1);
	}
	if(!chopline(buffer,&line))
	{
	  printf("Could not chop line\n");
	  exit(1);
	}
	if(!strcasecmp(line.object[1],"433"))
	{
	  snprintf(me.nick,MAXNICKLEN,"%s%d",argv[1],getpid());
	  netprintf("NICK %s", me.nick);
	}
	if(!strcasecmp(line.object[0],"PING"))
	{
	  netprintf("PONG %s", line.object[1]);
	  printf("[%s] *** PING? PONG!\n", gettime());
	  fflush(stdout);
	}
	if(!strcasecmp(line.object[1],"001"))
	  server.status |= ME_REG;
      }
    } 
    me.status |= ME_REG;
    netprintf("JOIN %s", channel);
    do
    {
      if(!netread(buffer,sizeof(buffer),server.fd))
      {
	printf("Could not read\n");
	exit(1);
      }
      if(!chopline(buffer,&line))
      {
	printf("Could not chop line\n");
	exit(1);
      }

	/* start processing commands */

      if(!strcasecmp(line.object[1],"JOIN"))
      {
	struct userhost info;
	if(chopuserhost(line.object[0],&info))
	{
	  if(!strcasecmp(info.nick,me.nick))
	  {
	    me.status |= ME_INCHANNEL;
	  }
	}
      }

      if(!strcasecmp(line.object[1],"PRIVMSG") && !strcasecmp(line.object[2],channel))
      {
	struct userhost info;
	char *exec;
	if(chopuserhost(line.object[0],&info))
	{
	  char msg[MAXBUFLEN];
	  int n;
	  bzero(msg,MAXBUFLEN);
	  if(line.object[3][1]=='!')
	  {
	    if(line.num > 4)
	    {
	      for (n=4 ; n < line.num ; n++)
	      {
	        strcat(msg, line.object[n]);
	        if(n < line.num-1)
	          strcat(msg, " ");
	      }
	    }
	    exec=line.object[3];
	    exec++;
	    exec++;
	    if (line.num == 4)
	      my_exec(exec);
	    else
	      printf("YES Executing %d bin/%s %s\n",line.num,exec,msg);
	    fflush(stdout);
	  }
	  else
	  {
	    printf("Line: %s\n", line.object[3]);
	    fflush(stdout);
	  }
	}
      }

      if(!strcasecmp(line.object[0],"PING"))
	netprintf("PONG %s", line.object[1]);
      if(!(me.status & ME_INCHANNEL))
      {
	netprintf("JOIN %s", channel);
      }
      
    } 
    while ((me.status & ME_REG)==ME_REG);

  }
  while(server.fd);

  exit(0);
}
