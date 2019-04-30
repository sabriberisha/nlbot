/*
 * lnbot v0.1alpha
 * 
 * include.h
 *
 * (c) sabri@cluecentral.net
 *
 * last revision: Fri May 16 12:14:28 CEST 2003
 */

#ifndef _INCLUDE_H	/* _INCLUDE_H */
#define _INCLUDE_H
#define MAXNICKLEN 10
#define MAXUSERLEN 15
#define MAXHOSTLEN 160
#define MAXUSERNUM 2048
#define MAXLASTNUM 10
#define MAXBUFLEN 1024

struct me
{
  char nick[MAXNICKLEN];
  char *user;
  char *realname;
  int status;
} me;
char *channel;

#define ME_REG 1
#define ME_INCHANNEL 2

struct server
{
  char *host;
  char *name;
  int status;
  int fd;
} server;

struct users
{
  char nick[MAXNICKLEN];
  char user[MAXUSERLEN];
  char host[MAXHOSTLEN];
} *user[MAXUSERNUM];

struct userhost
{
  char *nick;
  char *user;
  char *host;
};

struct parse
{
  char *object[1024];
  int num;
};

struct window
{
  char *last[MAXLASTNUM];
} window;
#endif _INCLUDE_H	/* _INCLUDE_H */
