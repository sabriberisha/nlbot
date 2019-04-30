/*
 * nlbit v0.1alpha
 *
 * exec.c
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
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include "include.h"


void netprintf();

void my_exec(char *prog)
{
  pid_t child;
  int compipe[2];
  int result;
  char buffer[MAXBUFLEN];
  extern char **environ;

  if(prog[(strlen(prog)-1)]=='\r')
    prog[(strlen(prog)-1)]='\0';

  printf("prog: %s len: %d\n\n", prog,strlen(prog));
  printf("Trying %s\n", prog);
  if(pipe(compipe)<0)
  {
    netprintf("PRIVMSG %s :Could not open communications pipe",channel);
    return;
  }

  if((child=fork())<0)
  {
    netprintf("PRIVMSG %s :Could not fork()",channel);
    return;
  }

  if(child==0)
  {
    char *argument[2];
    printf("Trying execve %s\n", prog);
    if (dup2(compipe[1],1)<0)
    {
      netprintf("PRIVMSG %s :Could not redirect stdout()",channel);
      exit(0);
    }
    argument[0]=prog;
    argument[1]='\0';
    execve(prog,argument,environ);
    exit(0);
  }
  printf("waitpid! %s\n", prog);
  waitpid(child,'\0','\0');
  bzero(buffer,sizeof(buffer));
  fcntl(compipe[0], F_SETFL, O_NONBLOCK);
  result=read(compipe[0],buffer,sizeof(buffer));
  printf("Done!\n");
  netprintf("PRIVMSG %s :%s",channel,buffer);
  return;
}
