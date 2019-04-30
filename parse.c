/*
 * logbot v0.1alpha
 *
 * parse.c
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
#include "include.h"

int chopuserhost(char *buffer, struct userhost *info)
{
  char *tmpptr;
  tmpptr=buffer;
  if(*tmpptr)
    tmpptr++;
  else
    return(0);
  info->nick=tmpptr;
  while(*tmpptr && *tmpptr != '!')
    tmpptr++;
  if(*tmpptr)
    *tmpptr='\0';
  else
    return(0);
  tmpptr++;
  if(*tmpptr)
  {
    info->user=tmpptr;
    while(*tmpptr && *tmpptr != '@')
      tmpptr++;
    if(*tmpptr)
      *tmpptr='\0';
    else
      return(0);
  }
  else
    return(0);
  tmpptr++;
  if(*tmpptr)
     info->host=tmpptr;
  else
    return(0);
  return(1);
}

int chopline(char *buffer, struct parse *line)
{
  int i=0,t;
  char *tmpptr;
  tmpptr=buffer;
  line->object[i]=tmpptr;
  i++;
  while (*tmpptr)
  {
    if(*tmpptr==' ')
    {
      *tmpptr='\0';
      tmpptr++;
      if(*tmpptr)
      {
	line->object[i]=tmpptr;
	i++;
      }
    }
    else
    {
      tmpptr++;
    }
  }
  if(i>0)
  {
    line->num=i;
    return(1);
  }
  else
    return(0);
 for(t=0 ; t < i ; t++)
  printf("%s\n", line->object[t]);
 return(0);
}
