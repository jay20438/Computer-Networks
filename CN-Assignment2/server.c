#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


long factorial(int n)
{
  return ((n == 1 || n == 0) ? 1 : n*factorial(n-1));
}

int main(){
 FILE *filePointer ;
  filePointer = fopen("sequential.txt","w");
  char *ip = "127.0.0.1";
  int port = 5566;

  int server_sock, client_sock;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;
  char buffer[1024];
  char store[1024];
  int n;

  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0)
  {
    perror("Socket error");
    exit(1);
  }
  printf("TCP server socket created.\n");

  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (n < 0)
  {
    perror("Bind error");
    exit(1);
  }
  printf("Bind to the port number: %d\n", port);

  listen(server_sock, 5);
  printf("Listening\n");

  addr_size = sizeof(client_addr);
  client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
  char *client_ip = inet_ntoa(client_addr.sin_addr);
  int client_port = ntohs(client_addr.sin_port);
  
  printf("Client IP address: %s\n",client_ip);
  printf("Client Port number: %d\n",client_port);
  
  char store_port[20];
  fputs(client_ip,filePointer);
  sprintf(store_port,"%d",client_port);
  fputs(store_port,filePointer);
  printf("Client connected.\n");
  
  int i;
  for(i = 1;i <= 20;i++){
    read(client_sock, buffer, sizeof(buffer));
    printf("Client: %s\n", buffer);
    int x = atoi(buffer);
    long value = factorial(x);
    sprintf(store,"%ld",value);
    printf("Server: %s\n", store);
    write(client_sock, store, strlen(store));
    fputs(store,filePointer);
    fputs("\n",filePointer);    
  }
  
  
    close(client_sock);
    printf("Client disconnected.\n\n");
	fclose(filePointer);
  return 0;
}

