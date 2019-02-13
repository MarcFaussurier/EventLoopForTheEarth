//
// Created by marcfsr on 2019-02-09.
//

#include "web.h"
#include <iostream>

using namespace std;
using namespace ipolitic;

int runServer(EventLoop * ev)
{
    int listenfd, connfd;
    char hostname[MAXLINE] = "0.0.0.0", port[MAXLINE] = "8080";
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    listenfd = Open_listenfd(port);
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); //line:netp:tiny:accept
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE,
                    port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);


        int is_static;
        char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
        char filename[MAXLINE], cgiargs[MAXLINE];
        rio_t rio;

        /* Read request line and headers */
        Rio_readinitb(&rio, connfd);
        if (!Rio_readlineb(&rio, buf, MAXLINE))  //line:netp:doit:readrequest
            continue;
        printf("%s", buf);
        sscanf(buf, "%s %s %s", method, uri, version);       //line:netp:doit:parserequest
        if (strcasecmp(method, "GET")) {                     //line:netp:doit:beginrequesterr
            clienterror(connfd, method, "501", "Not Implemented",
                        "Tiny does not implement this method");
            continue;
        }                                                    //line:netp:doit:endrequesterr
        read_requesthdrs(&rio);
        is_static = parse_uri(uri, filename, cgiargs);
        Defer nextTest = bNewPromise(ev, filename, [connfd, filename, is_static, uri, cgiargs, buf](Defer d) -> void {
            const char *  _filename = &(filename[0]);
            bool    _is_static = is_static;
            const char *  _uri = &(uri[0]);
            const char *  _cgiargs = &(cgiargs[0]);
            const char *  _buf = &(buf[0]);
            int     _fd = connfd;

            doit(_filename, _is_static, _uri, _cgiargs, _buf, _fd);                                             //line:netp:tiny:doit
            Close(connfd);
            d.resolve();
        });
                 //line:netp:tiny:close
    }
}
/* $end tinymain */

/*
 * doit - handle one HTTP request/response transaction
 */
/* $begin doit */
void doit(const char * filename, bool is_static, const char * uri, const char *cgiargs, const char * buf, int fd)
{
    struct stat sbuf;

    char pathToRoot[] = "./../www/";
    size_t newLen = strlen(pathToRoot) + strlen(filename) + 1;
    char newChr[newLen];
    int startAt = 0;
    int i = 0;
    for (; i < newLen; i++) {
        if (i < strlen(pathToRoot)) {
            startAt = i;
            newChr[i] = pathToRoot[i];
        } else {
            if ((newChr[i] = filename[i - startAt + 1]) == '\0') {
                break;
            }
        }
    }
    cout << "New char " << newChr << endl;
    if (stat(newChr, &sbuf) < 0) {                     //line:netp:doit:beginnotfound
	clienterror(fd, newChr, "404", "Not found",
		    "Tiny couldn't find this file");
	return;
    }                                              //line:netp:doit:endnotfound

    if (is_static) { /* Serve static content */
	if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) { //line:netp:doit:readable
	    clienterror(fd, newChr, "403", "Forbidden",
			"Tiny couldn't read the file");
	    return;
	}
	serve_static(fd, newChr, sbuf.st_size);        //line:netp:doit:servestatic
    }
    else { /* Serve dynamic content */
	if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) { //line:netp:doit:executable
	    clienterror(fd, newChr, "403", "Forbidden",
			"Tiny couldn't run the CGI program");
	    return;
	}
	serve_dynamic(fd, newChr, cgiargs);            //line:netp:doit:servedynamic
    }
}
/* $end doit */

/*
 * read_requesthdrs - read HTTP request headers
 */
/* $begin read_requesthdrs */
void read_requesthdrs(rio_t *rp)
{
    char buf[MAXLINE];

    Rio_readlineb(rp, buf, MAXLINE);
    printf("%s", buf);
    while(strcmp(buf, "\r\n")) {          //line:netp:readhdrs:checkterm
	Rio_readlineb(rp, buf, MAXLINE);
	printf("%s", buf);
    }
    return;
}
/* $end read_requesthdrs */

/*
 * parse_uri - parse URI into filename and CGI args
 *             return 0 if dynamic content, 1 if static
 */
/* $begin parse_uri */
int parse_uri(char *uri, char *filename, char *cgiargs)
{
    char *ptr;

    if (!strstr(uri, "cgi-bin")) {  /* Static content */ //line:netp:parseuri:isstatic
	strcpy(cgiargs, "");                             //line:netp:parseuri:clearcgi
	strcpy(filename, ".");                           //line:netp:parseuri:beginconvert1
	strcat(filename, uri);                           //line:netp:parseuri:endconvert1
	if (uri[strlen(uri)-1] == '/')                   //line:netp:parseuri:slashcheck
	    strcat(filename, "index.html");               //line:netp:parseuri:appenddefault
	return 1;
    }
    else {  /* Dynamic content */                        //line:netp:parseuri:isdynamic
	ptr = index(uri, '?');                           //line:netp:parseuri:beginextract
	if (ptr) {
	    strcpy(cgiargs, ptr+1);
	    *ptr = '\0';
	}
	else
	    strcpy(cgiargs, "");                         //line:netp:parseuri:endextract
	strcpy(filename, ".");                           //line:netp:parseuri:beginconvert2
	strcat(filename, uri);                           //line:netp:parseuri:endconvert2
	return 0;
    }
}
/* $end parse_uri */

/*
 * serve_static - copy a file back to the client
 */
/* $begin serve_static */
void serve_static(int fd, char *filename, int filesize)
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];

    /* Send response headers to client */
    get_filetype(filename, filetype);       //line:netp:servestatic:getfiletype
    sprintf(buf, "HTTP/1.0 200 OK\r\n");    //line:netp:servestatic:beginserve
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    sprintf(buf, "%sConnection: close\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
    Rio_writen(fd, buf, strlen(buf));       //line:netp:servestatic:endserve
    printf("Response headers:\n");
    printf("%s", buf);

    /* Send response body to client */
    srcfd = Open(filename, O_RDONLY, 0);    //line:netp:servestatic:open
    srcp = (char*) Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);//line:netp:servestatic:mmap
    Close(srcfd);                           //line:netp:servestatic:close
    Rio_writen(fd, srcp, filesize);         //line:netp:servestatic:write
    Munmap(srcp, filesize);                 //line:netp:servestatic:munmap
}

/*
 * get_filetype - derive file type from file name
 */
void get_filetype(char *filename, char *filetype)
{
    if (strstr(filename, ".html"))
	    strcpy(filetype, "text/html");

    else if (strstr(filename, ".gif"))
	    strcpy(filetype, "image/gif");

    else if (strstr(filename, ".png"))
	    strcpy(filetype, "image/png");

    else if (strstr(filename, ".jpg"))
	    strcpy(filetype, "image/jpeg");

    else if (strstr(filename, ".css"))
        strcpy(filetype, "text/css");

    else
	    strcpy(filetype, "text/plain");
}
/* $end serve_static */

/*
 * serve_dynamic - run a CGI program on behalf of the client
 */
/* $begin serve_dynamic */
void serve_dynamic(int fd, const char *filename, const char *cgiargs)
{
    char buf[MAXLINE], *emptylist[] = { NULL };

    /* Return first part of HTTP response */
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));

    if (Fork() == 0) { /* Child */ //line:netp:servedynamic:fork
	/* Real server would set all CGI vars here */
	setenv("QUERY_STRING", cgiargs, 1); //line:netp:servedynamic:setenv
	Dup2(fd, STDOUT_FILENO);         /* Redirect stdout to client */ //line:netp:servedynamic:dup2
	Execve(filename, emptylist, environ); /* Run CGI program */ //line:netp:servedynamic:execve
    }
    Wait(NULL); /* Parent waits for and reaps child */ //line:netp:servedynamic:wait
}
/* $end serve_dynamic */

/*
 * clienterror - returns an error message to the client
 */
/* $begin clienterror */
void clienterror(int fd, char *cause, char *errnum,
		 char *shortmsg, char *longmsg)
{
    char buf[MAXLINE], body[MAXBUF];

    /* Build the HTTP response body */
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
}
/* $end clienterror */
