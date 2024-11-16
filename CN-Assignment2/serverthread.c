#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <fcntl.h> 
#include <unistd.h> 
#include<pthread.h>
#include<errno.h>

#define NTHREADS 10

void *connection_handler(void *);
pthread_t thread_id[NTHREADS];
pthread_mutex_t lock;
int service_count, sockfd,d;
struct sockaddr_in server , client_addr;

 long factorial(int n)
{
  return ((n == 1 || n == 0) ? 1 : n*factorial(n-1));
}


// Socket create
int sock_create( )
{
    sockfd= socket(AF_INET , SOCK_STREAM , 0);

    if (sockfd <0)
    {
        printf("Could not create socket");  
        return 1;
    }
    puts("Socket created");
    memset(&server,0,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr =  inet_addr("127.0.0.1");
    server.sin_port = htons(7799);

}      
// Bind
int sock_bind()
{
    int b= bind(sockfd,(struct sockaddr *)&server , sizeof(server));
    if (b <0)
    {
        perror("Bind failed. Error");
        return 1;
    }
    puts("Bind");

}
// Listen
int sock_listen()
{
   listen(sockfd , 10);
}
//Connection accept
int sock_accept()
{
    int s = sizeof(struct sockaddr_in);
    d = accept(sockfd, (struct sockaddr *)&client_addr, (socklen_t*)&s);
    
    char *client_ip = inet_ntoa(client_addr.sin_addr);
    int client_port = ntohs(client_addr.sin_port);
  
    printf("Client IP address: %s\n",client_ip);
    printf("Client Port number: %d\n",client_port);
		 
    char store_port[20];
    fputs(client_ip,filePointer2);
    sprintf(store_port,"%d",client_port);
    fputs(store_port,filePointer2);
    
    if (d < 0)
    {
        perror("accept failed");
        return 1;
    } 
    puts("Connection accepted");
}

int main(int argc , char *argv[])
{  
   int client_sock;
   sock_create();
   sock_bind();
   sock_listen();
   

    pthread_attr_t attr;
    int i,j;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    printf("Creating threads\n");

    int cli_sock=client_sock;

    for (i = 0; i < NTHREADS ; i++)
    {
        pthread_create(&(thread_id[i]), &attr, connection_handler, (void*) &cli_sock);
    }

    pthread_attr_destroy(&attr); 
    for(j=0; j < NTHREADS; j++)
    {
        pthread_join( thread_id[j], NULL);
    }
    pthread_exit(NULL);
    fclose(filePointer2);
    return 0;
}

void *connection_handler(void *sfd)
{   
     FILE *filePointer2;
   filePointer2 = fopen("serverthread.txt","w");
    sock_accept();
    int sock = d;
    int read_size=0;
    char client_message[1024];
    char store[1024];
        int i;
	  for(i = 1;i <= 20;i++)
	  {
	    bzero(client_message,1024);
            recv(sock, client_message, sizeof(client_message),0);
            printf("Client: %s\n", client_message);
            int x = atoi(client_message);
	    long value = factorial(x);
	    sprintf(store,"%ld",value);
            // Closing the server socket id
            close(sockfd);
            // Send a confirmation message
            // to the client
            printf("Server: %s\n", store);
    	    send(sock, store, sizeof(store),0);
    	    fputs(store,filePointer2);
	    fputs("\n",filePointer2); 
    	    }
    
     if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("Recv failed");    
    }
    pthread_mutex_lock(&lock);
    service_count++;
    pthread_mutex_unlock(&lock);

    pthread_exit((void*) sfd);
    return 0;
}
