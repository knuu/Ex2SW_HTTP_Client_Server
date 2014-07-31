#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#define MAX_LINE 1024

int main(int argc, char* argv[])
{
  FILE *fp,*fp1;
  int c, i, s, ns, port,ret;
  struct sockaddr_in sin, fsin;
  char buf1[1024],buf2[1024],*tp;
  char Not_Found[MAX_LINE] = "HTTP/1.1 404 Not Fonund\r\nContent-Type: text/html; charset=us-ascii\r\n\r\n<HTML><HEAD>Not Found</HEAD>\r\n<BODY>\r\nThe requested URL /index.html was not found on this server.\r\n</BODY></HTML>\r\n";
  char OK[MAX_LINE] ="HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=iso-2022-jp\r\n\r\n";
  socklen_t fromlen = sizeof(struct sockaddr_in);
  
  if((argc <= 1) || ((port = atoi(argv[1])) == 0)) {
    perror("no port number");
    exit(1);		
  }
  
  if((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("client socket()");
    exit(1);		
  }
  
  bzero(&sin, sizeof(sin)); /* sinを0で初期化 */
  sin.sin_family = AF_INET; /* アドレスファミリ．今回はAF_INETで固定 */
  sin.sin_port = htons(port); /* ポート番号 */
  sin.sin_addr.s_addr = INADDR_ANY; /* IPアドレス */
  
  if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
    perror("server bind()");
    exit(1);	
  }
  
  if (listen(s, 128) == -1) {
    perror("server listen()");
    exit(1);	
  }
  
  if ((ns = accept(s, (struct sockaddr *)&fsin, &fromlen)) == -1) {
    perror("server accept()");	
    exit(1);
  }
  
  fp = fdopen(ns, "r");
  
  fgets(buf1, MAX_LINE, fp);
  tp = strtok(buf1," /");
  tp = strtok(NULL," /");
  strcpy(buf1,tp);
  if ((fp1 = fopen(buf1,"rb")) == NULL) {
    write(ns,Not_Found,strlen(Not_Found));
    exit(1);
  } else {
    write(ns,OK,strlen(OK));
  }
  while((fgets(buf2,sizeof(buf2),fp1)) != NULL) {
    write(ns,buf2,strlen(buf2));
  }
  fclose(fp1);
  close(s);
  return 0;
}

