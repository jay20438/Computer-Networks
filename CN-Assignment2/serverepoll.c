#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <fcntl.h> 
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <poll.h>

#define MAX_CONNECTIONS 10

 long factorial(int n)
{
  return ((n == 1 || n == 0) ? 1 : n*factorial(n-1));
}

int create_tcp_server_socket()
{
   struct sockaddr_in saddr;
   int fd,ret_val;
   
   // Create a TCP socket
   fd = socket(AF_INET,SOCK_DGRAM,0);
   //earlier IPPROTO_TCP
   if(fd == -1)
   {
     fprintf(stderr,"socket failed [%s]\n",strerror(errno));
     return -1;
   }
   
   printf("Create a socket with fd: %d\n",fd);
   
   saddr.sin_family = AF_INET;
   saddr.sin_port = htons(7001);
   saddr.sin_addr.s_addr = INADDR_ANY;
   
   // bind the socket to port 7000 on the local host
   ret_val = bind(fd,(struct sockaddr *)&saddr,sizeof(struct sockaddr_in));
   if(ret_val != 0)
   {
     fprintf(stderr,"bind failed [%s]\n", strerror(errno));
     close(fd);
     return -1;
   }
   
   //listen for incoming connections
   ret_val = listen(fd,5);
   if(ret_val != 0)
   {
     fprintf(stderr,"error [%s]\n",strerror(errno));
     close(fd);
     return -1;
   }
   return fd;
   }
   
   int main()
   {
   FILE *filePointer;
     filePointer = fopen("serverepoll.txt","w");
   struct sockaddr_in new_addr;
   int server_fd,ret_val,i;
   int status = fcntl(server_fd, F_SETFL, fcntl(server_fd, F_GETFL, 0) | O_NONBLOCK);

	if (status == -1){
	  perror("calling fcntl");
   socklen_t addrlen;
   char buf[1024];
   char store[1024];
   int all_connections[MAX_CONNECTIONS];
   struct pollfd *pfds;
   int nfds = MAX_CONNECTIONS-1,num_open_fds = nfds;
   struct pollfd pollfds[MAX_CONNECTIONS+1];
   
   server_fd = create_tcp_server_socket();
   
   if(server_fd == -1)
   {
     fprintf(stderr,"Failed to create a sever\n");
     return -1;
   }
   
   efd = epoll_create1(0);
   pollfd.data.fd = server_fd;
   pollfd.events = EPOLLIN;
   s = epoll_ctl(efd,EPOLL_CTL_ADD,server_fd,&pollfd);
   
   if(s == -1)
   {
     perror("Error epoll_ctl");
     abort()
   }
   int useClient = 0;
   for(int i = 1;i < MAX_CONNECTIONS;i++)
   {
     pollfds[i].data.fd = 0;
     pollfds[i].events = EPOLLIN;
   }
   
   char *client_ip = inet_ntoa(new_addr.sin_addr);
		 int client_port = ntohs(new_addr.sin_port);
  
	     	 printf("Client IP address: %s\n",client_ip);
		 printf("Client Port number: %d\n",client_port);
		 
		 char store_port[20];
		 fputs(client_ip,filePointer);
		 sprintf(store_port,"%d",client_port);
		 fputs(store_port,filePointer);
   int k = 0;
   while(k < 20)
   {
     ret_val = epoll_wait(efd,pollfds,MAX_CONNECTIONS,-1);
     
     if(ret_val == 0)
       break;  
       
     int j;
     for(j = 0;j < ret_val;j++)
     {
       int fd = pollfds[i].data.fd;
       memset(buf,0,sizeof(buf));
       
       int val = read(pollfds[i].data.fd,buf,1024);
       
       if(val == -1)
       {
         pollfds[i].data.fd = 0;
         pollfds[i].events = 0;
         useClient--;
         printf("Error");
     }
     else if(val == 0)
     {
       pollfds[i].data.fd = 0;
       pollfds[i].events = 0;
       useClient--;
       break;
     }
     else
     {
          printf("Client: %s\n", buf);
	  int x = atoi(buf);
	  long value = factorial(x);
	  sprintf(store,"%ld",value);
    	  printf("Server: %s\n", store);
    	  send(pollfds[i].fd,store,1024,0);
    	  fputs(store,filePointer);
          fputs("\n",filePointer);  
     }
    }
    k++;
   }
     
     for (i=0;i < MAX_CONNECTIONS;i++) 
     {
             close(pollfds[i].data.fd);
        
     }
     free(pollfds);
     close(server_fds);
     fclose(filePointer);
     return 0;
   }            
             
              
