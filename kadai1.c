#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256

int main(void)
{
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
    perror("path required");
    exit(1);
  }
  
  tp = strtok(temp,":");
  strcpy(host,tp);
  
  if((tp = strtok(NULL,"")) != NULL) {
    port = atoi(tp);
  } else {
    port = 80;
  }

  printf("host = %s\n",host);
  printf("port = %d\n",port);
  printf("path = %s\n",path);
  
  return 0;
}
