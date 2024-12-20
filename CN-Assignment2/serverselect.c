 #include <stdio.h>
 #include <stdlib.h>
 #include <netinet/in.h> 
 #include <fcntl.h> 
 #include <unistd.h> 
 #include <string.h>
 #include <errno.h>
 #include <arpa/inet.h>
 
 #define MAX_CONNECTIONS 10 
  
 long factorial(int n)
{
  return ((n == 1 || n == 0) ? 1 : n*factorial(n-1));
}

 int create_tcp_server_socket() {
     struct sockaddr_in saddr;
     int fd, ret_val;

     //creating a TCP socket
     fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
     if (fd == -1) {
         fprintf(stderr, "socket failed [%s]\n", strerror(errno));
         return -1;
     }
     printf("Created a socket with fd: %d\n", fd);

     // socket address structure initialized.
     saddr.sin_family = AF_INET;         
     saddr.sin_port = htons(7799);     
     saddr.sin_addr.s_addr = INADDR_ANY; 

     ret_val = bind(fd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
     if (ret_val != 0) {
         fprintf(stderr, "bind failed [%s]\n", strerror(errno));
         close(fd);
         return -1;
     }

     int status = fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);

	if (status == -1)
	{
	  perror("calling fcntl");
	}
	
     ret_val = listen(fd, 5);
     if (ret_val != 0) {
         fprintf(stderr, "listen failed [%s]\n", strerror(errno));
         close(fd);
         return -1;
     }
     return fd;
 }
 int main () {
     FILE *filePointer1;
     filePointer1 = fopen("serverselect.txt","w");
     fd_set read_fd_set;
     struct sockaddr_in client_addr;
     int server_fd, new_fd, ret_val, i;
     socklen_t addrlen;
     char buf[1024];
     char store[1024];
     int all_connections[MAX_CONNECTIONS];

     /* Get the socket server fd */
     server_fd = create_tcp_server_socket(); 
     if (server_fd == -1) 
     {
       fprintf(stderr, "Failed to create a server\n");
       return -1; 
     }   

     for (i=0;i < MAX_CONNECTIONS;i++) 
     {
         all_connections[i] = -1;
     }
     all_connections[0] = server_fd;
     int k = 0;
     while (k < 20) 
     {
         FD_ZERO(&read_fd_set);
         
         for (i=0;i < MAX_CONNECTIONS;i++)
          {
             if (all_connections[i] >= 0) 
             {
                 FD_SET(all_connections[i], &read_fd_set);
             }
         }

         printf("\nUsing select() to listen for incoming events\n");
         ret_val = select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL);

         if (ret_val >= 0 ) 
         {
             printf("Select returned with %d\n", ret_val);
             /* Check if the fd with event is the server fd */
             if (FD_ISSET(server_fd, &read_fd_set))
              { 
                 /* accept the new connection */
                 printf("Returned fd is %d (server's fd)\n", server_fd);
                 new_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen);
                 char *client_ip = inet_ntoa(client_addr.sin_addr);
		 int client_port = ntohs(client_addr.sin_port);
  
	     	 printf("Client IP address: %s\n",client_ip);
		 printf("Client Port number: %d\n",client_port);
		 
		 char store_port[20];
		 fputs(client_ip,filePointer1);
		 sprintf(store_port,"%d",client_port);
		 fputs(store_port,filePointer1);
  
                 if (new_fd >= 0) {
                     printf("Accepted a new connection with fd: %d\n", new_fd);
                     for (i=0;i < MAX_CONNECTIONS;i++) {
                         if (all_connections[i] < 0) {
                             all_connections[i] = new_fd; 
                             break;
                         }
                     }
                 } else {
                     fprintf(stderr, "accept failed [%s]\n", strerror(errno));
                 }
                 ret_val--;
                 if (!ret_val) 
                 continue;
             } 

             
             for (i=1;i < MAX_CONNECTIONS;i++) {
                 if ((all_connections[i] > 0) && (FD_ISSET(all_connections[i], &read_fd_set))) {
                     /* read incoming data */ 
                     ret_val = recv(all_connections[i], buf, 1024, 0);
                     if (ret_val == 0) {
                         printf("Closing connection for fd:%d\n", all_connections[i]);
                         close(all_connections[i]);
                         all_connections[i] = -1; 
                     } 
                     if (ret_val > 0) 
                     {
                             printf("Client: %s\n", buf);
			     int x = atoi(buf);
			     long value = factorial(x);
			     sprintf(store,"%ld",value);
    			     printf("Server: %s\n", store);
    			     send(all_connections[i],store,1024,0);
    			     fputs(store,filePointer1);
			     fputs("\n",filePointer1);  
                     } 
                     if (ret_val == -1) {
                         printf("recv() failed for fd: %d [%s]\n", 
                             all_connections[i], strerror(errno));
                         break;
                     }
                 }
                 ret_val--;
                 if (!ret_val) 
                 continue;
             } 
         }
         k++;
     } 

     //Closing all the sockets
     for (i=0;i < MAX_CONNECTIONS;i++) 
     {
         if (all_connections[i] > 0)
          {
             close(all_connections[i]);
         }
     }
     fclose(filePointer1);
     return 0;
 }
 
