/*
 * logbot v0.1alpha
 *
 * initConnect.c
 *
 * (c) sabri@cluecentral.net
 *
 * last revision: Fri May 16 12:14:28 CEST 2003
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include "include.h"

int myconnect(char *host, char *connectport)
{
  int ret=-1,port=-1,fd=0;
  char ipstr[40];
  struct addrinfo hints, *res;
  
  memset(&hints, 0, sizeof(struct addrinfo));
  memset(&ipstr, 0, sizeof(ipstr));
  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_CANONNAME;
  hints.ai_socktype = SOCK_STREAM;
  if ((ret = getaddrinfo(host, connectport, &hints, &res)) != 0)
  {
    printf("*** getaddrinfo() failed\n");
    return(-1);
  }
  if(res->ai_family == AF_INET)
  {
    struct sockaddr_in *mysin = (struct sockaddr_in *) res;
    port=ntohs(mysin->sin_port);
    if (inet_ntop(AF_INET, &mysin->sin_addr, ipstr, sizeof(ipstr)) == NULL)
    {
      printf("*** Unsupported AF type\n");
      return(-1);
    }
  }
  if(res->ai_family == AF_INET6)
  {
    struct sockaddr_in6 *mysin6 = (struct sockaddr_in6 *) res;
    port=ntohs(mysin6->sin6_port);
    if (inet_ntop(AF_INET6, &mysin6->sin6_addr, ipstr, sizeof(ipstr)) == NULL)
    {
      printf("*** Unsupported AF type\n");
      return(-1);
    }
  }
  if(port==-1 || strlen(ipstr)==0)
  {
    printf("*** Unsupported AF type\n");
    return(-1);
  }
  fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
  if((connect(fd,res->ai_addr,res->ai_addrlen)))
  {
    printf("Connect failed\n");
    return(-1);
  }
  return(fd);
}
