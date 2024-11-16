#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
 
// PORT number
#define PORT 7799
 
 
 long factorial(int n)
{
  return ((n == 1 || n == 0) ? 1 : n*factorial(n-1));
}

int main()
{
	FILE *fp2 ;
  fp2 = fopen("serverfork.txt","w");
    int sockfd, ret;
 
    struct sockaddr_in serverAddr;
 
    int clientSocket;
 
    struct sockaddr_in client_addr;
 
    socklen_t addr_size;
 
    pid_t childpid;
 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    char buffer[1024];
    char store[1024];
 
    if (sockfd < 0) 
    {
        printf("Error in connection.\n");
        exit(1);
    }
 
    printf("Server Socket is created.\n");
 
    memset(&serverAddr, '\0',sizeof(serverAddr));
 
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
 
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
 
    ret = bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
 
    // Error handling
    if (ret < 0) 
    {
        printf("Error in binding.\n");
        exit(1);
    }
 
    // Listening for connections (upto 10)
    if (listen(sockfd, 10) == 0) 
    {
        printf("Listening...\n\n");
    }
 
    int cnt = 0;
    int q = 0;
    while (q < 20) 
    { 
        clientSocket = accept(sockfd, (struct sockaddr*)&client_addr,&addr_size);
 
	char *client_ip = inet_ntoa(client_addr.sin_addr);
  int client_port = ntohs(client_addr.sin_port);
  
  printf("Client IP address: %s\n",client_ip);
  printf("Client Port number: %d\n",client_port);
  
  char store_port[20];
  fputs(client_ip,fp2);
  sprintf(store_port,"%d",client_port);
  fputs(store_port,fp2);
  
        // Error handling
        if (clientSocket < 0) 
        {
            exit(1);
        }
 
        // Displaying information of
        // connected client
        printf("Connection accepted from %s:%d\n",
               inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
 
        printf("Clients connected: %d\n\n",++cnt);
 
        // Creates a child process
        if ((childpid = fork()) == 0) 
        {
	  int i;
	  for(i = 1;i <= 20;i++)
	  {
	    bzero(buffer,1024);
            read(clientSocket, buffer, sizeof(buffer));
            printf("Client: %s\n", buffer);
            int x = atoi(buffer);
	    long value = factorial(x);
	    sprintf(store,"%ld",value);
            // Closing the server socket id
            close(sockfd);
            sleep(10);
           
            printf("Server: %s\n", store);
    	    write(clientSocket, store, sizeof(store));
    	    fputs(store,fp2);
	    fputs("\n",fp2); 
    	    }
        }
        q++;
    }
 
    // Close the client socket id
    close(clientSocket);
    fclose(fp2);
    return 0;
}
