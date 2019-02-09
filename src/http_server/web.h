//
// Created by marcfsr on 2019-02-09.
//

#ifndef NALP_WEB_H
#define NALP_WEB_H

#include "web_server.h"
#include "./../event_engine/EventLoop.h"
#include "./../event_engine/bpromise.h"

using namespace ipolitic;

int runServer(EventLoop * ev);
void doit(const char * filename, bool is_static, const char * uri, const char *cgiargs,  const char * buf, int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, const char *filename, const char *cgiargs);
void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg);

#endif //NALP_WEB_H
