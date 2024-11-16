#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){

  char *ip = "127.0.0.1";
  int port = 5566;

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
  for(i = 1;i <= 20;i++){
  
  sprintf(buffer,"%d",i);
  
  printf("Client: %s\n", buffer);
  write(sock, buffer, strlen(buffer));
  
  bzero(buffer,1024);
  read(sock, buffer, sizeof(buffer));
  printf("Server: %s\n", buffer);
  }
  close(sock);
  printf("Disconnected from the server.\n");

  return 0;

}
