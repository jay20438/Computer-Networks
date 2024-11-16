#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <unistd.h> 
#include<pthread.h>
 
#define NTHREADS 10

void * client_connection(void *arg)
{
  char *ip = "127.0.0.1";
  int port = 7799;

  int sock;
  struct sockaddr_in addr;
  socklen_t addr_size;
  char buffer[1024];

  int n;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
    perror("Socket error");
    exit(1);
  }
  printf("TCP server socket created.\n");

  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);

  connect(sock, (struct sockaddr*)&addr, sizeof(addr));
  printf("Connected to the server.\n");
  
  int i;
  for(i = 1;i <= 20;i++)
  {
  
  sprintf(buffer,"%d",i);
  sleep(5);
  printf("Client: %s\n", buffer);
  write(sock, buffer,1024);
  
  bzero(buffer,1024);
  read(sock, buffer, sizeof(buffer));
  printf("Server: %s\n", buffer);
  }
  close(sock);
  printf("Disconnected from the server.\n");

  pthread_exit(NULL);
}
  
  
int main(int argc , char *argv[])
{        
    int i,j;
    pthread_t thread_id[11];
    printf("Creating threads\n");
    
    for (i = 0; i < NTHREADS ; i++)
    {
        pthread_create(&thread_id[i], NULL, client_connection,NULL);
        sleep(5);
    }
    
    for(j=0; j < NTHREADS; j++)
    {
        pthread_join(thread_id[j], NULL);
        printf("%d:\n",j);
    }
    //pthread_exit(NULL);
    return 0;
}
