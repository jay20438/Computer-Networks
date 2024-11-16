 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <unistd.h>
 #include <errno.h>
 #include <netinet/in.h> 
 #include <netdb.h> 

 int main () {
     struct sockaddr_in saddr;
     int fd, ret_val;
     char buffer[1024];


     fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      
     if (fd == -1) 
     {
         fprintf(stderr, "socket failed [%s]\n", strerror(errno));
         return -1;
     }
     printf("Created a socket with fd: %d\n", fd);


     saddr.sin_family = AF_INET;         
     saddr.sin_port = htons(7799); 
     saddr.sin_addr.s_addr = inet_addr("127.0.0.1");


     ret_val = connect(fd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
     if (ret_val == -1) 
     {
         fprintf(stderr, "connect failed [%s]\n", strerror(errno));
         close(fd);
         return -1;
     }
     printf("The Socket is now connected\n");

     sleep(5);
 
     int i;
     for(i = 1;i <= 20;i++)
     {
     sprintf(buffer,"%d",i);
  
     printf("Client: %s\n", buffer);
     ret_val = send(fd,buffer, sizeof(buffer), 0);
     
     bzero(buffer,1024);
     recv(fd, buffer, sizeof(buffer),0);
     printf("Server: %s\n", buffer);
     }
     //close socket
     close(fd);
     return 0;
 }
