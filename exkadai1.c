#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#define MAX_LINE 1024

int main(void)
{
  while (1) {
    // URL解析器
    char input[MAX_LINE],scheme[MAX_LINE],temp[MAX_LINE],path[MAX_LINE],host[MAX_LINE];
    char *tp;
    int port;
    
    printf("Input URL:\n");
    fgets(input, sizeof(input), stdin);
    
    tp = strtok(input, ":/");
    strcpy(scheme,tp);
    if (strcmp(scheme,"http") != 0) {
      perror("Scheme must be \"http\"");
      exit(1);
    }
    
    if((tp = strtok(NULL, "/\n")) != NULL) {
      strcpy(temp,tp);
    } else {
      perror("host required");
      exit(1);
    }
    
    if((tp = strtok(NULL, "\n")) != NULL) {
      strcpy(path,tp);
    } else {
      strcpy(path,"");
    }
    
    tp = strtok(temp,":");
    strcpy(host,tp);
    
    if((tp = strtok(NULL,"")) != NULL) {
      port = atoi(tp);
    } else {
      port = 8080;
    }
    // URL解析器ここまで
    
    FILE *fp;
    char hostname[MAX_LINE],buf[MAX_LINE],request[MAX_LINE];
    int c, i, s;
    struct hostent *hp;
    struct sockaddr_in sin;
    
    if ((hp = gethostbyname("www-proxy")) == NULL) {
      fprintf(stderr, "%s: unknown host.\n", hostname);
      exit(1);
    }
    
    if((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      perror("client socket()");
      exit(1);		
    }
    
    bzero(&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    memcpy(&sin.sin_addr, hp->h_addr, hp-> h_length);
    
    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
      perror("client connect()");
      exit(1);	
    }
    
    fp = fdopen(s, "r");
    
    sprintf(request,"GET http://%s/%s HTTP/1.1 \r\n Host: %s \r\n\r\n",host,path,host);
    send(s,request,strlen(request),0);
    
    while(recv(s,buf,sizeof(buf),0)) {
      printf("%s",buf);
      memset(&buf,'\0',sizeof(buf));
    }
    
    close(s);
  }
  return 0;
}

