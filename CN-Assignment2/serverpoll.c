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
   fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
   //earlier IPPROTO_TCP
   if(fd == -1)
   {
     fprintf(stderr,"socket failed [%s]\n",strerror(errno));
     return -1;
   }
   
   printf("Create a socket with fd: %d\n",fd);
   
   saddr.sin_family = AF_INET;
   saddr.sin_port = htons(7799);
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
     fprintf(stderr,"listen failed [%s]\n",strerror(errno));
     close(fd);
     return -1;
   }
   return fd;
   }
   
   int main()
   {
   struct sockaddr_in new_addr;
   int server_fd,ret_val,i;
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
   
   int status = fcntl(server_fd, F_SETFL, fcntl(server_fd, F_GETFL, 0) | O_NONBLOCK);

	if (status == -1)
	{
	  perror("calling fcntl");
	}
   pollfds[0].fd = server_fd;
   pollfds[0].events = POLLIN;
   int useClient = 0;
   
   for(int i = 1;i < MAX_CONNECTIONS;i++)
   {
     pollfds[i].fd = 0;
     pollfds[i].events = POLLIN;
   }
   
   while(1)
   {
     ret_val = poll(pollfds,useClient+1,5000);
     
     if(ret_val >= 0)
     {
       if(pollfds[0].revents & POLLIN)
       {
         printf("Returned fd is %d (server's fd) \n",server_fd);
         int new_fd = accept(server_fd,(struct sockaddr*)&new_addr,&addrlen);
         if(new_fd >= 0)
         {
           printf("Accept a new connnection with fd: %d\n",new_fd);
           for(int i = 1;i < MAX_CONNECTIONS;i++)
           {
             if(pollfds[i].fd == 0)
             {
               pollfds[i].fd = new_fd;
               pollfds[i].events = POLLIN;
               useClient++;
               break;
             }
            }
          }
          else
          {
            fprintf(stderr,"accept failed [%s]\n",strerror(errno));
          }
          
          ret_val--;
          if(!ret_val) continue;
         }
         
         for(i = 1;i < MAX_CONNECTIONS;i++)
         {
           if(pollfds[i].fd > 0 && pollfds[i].revents & POLLIN)
           {
             memset(buf,0,sizeof(buf));
             int bufSize = recv(pollfds[i].fd,buf,sizeof(buf),0);
             if(bufSize == -1)
             {
               pollfds[i].fd = 0;
               pollfds[i].events = 0;
               pollfds[i].revents = 0;
               useClient--;
             }
             
             if(bufSize > 0)
             {
               printf("Client: %s\n", buf);
	       int x = atoi(buf);
	       long value = factorial(x);
	       sprintf(store,"%ld",value);
    	       printf("Server: %s\n", store);
    	       send(pollfds[i].fd,store,1024,0);
             }
             
             if(bufSize == 0)
             { 
               pollfds[i].fd = 0;
               pollfds[i].events = 0;
               pollfds[i].revents = 0;
               useClient--;
             }
           }
         }
       }
       
       else
       printf("Failed to connect.\n");
     }
     
     for (i=0;i < MAX_CONNECTIONS;i++) {
         if (all_connections[i] > 0) {
             close(all_connections[i]);
         }
     }
     
     return 0;
   }            
             
              
