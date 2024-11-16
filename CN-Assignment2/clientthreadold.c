#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
 
int main(int argc , char *argv[])
{
    int sockfd;
    struct sockaddr_in servaddr;
    char message[1024] , servaddr_reply[1024];


    if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) <0)
    {
        printf("Could not create socket\n");
        return 1;
    }
    puts("Socket created");

    servaddr.sin_family= AF_INET;
    servaddr.sin_port= htons(2100);
    servaddr.sin_addr.s_addr= inet_addr("127.0.0.1");

    if (connect(sockfd , (struct sockaddr *)&servaddr , sizeof(servaddr)) <0)
    {
        perror("Connection failed\n");
        return 1;
    }
    puts("Connected");

    while(1)
    {
        int i;
    for(i = 1;i <= 20;i++)
     {
        sprintf(message,"%d",i);
        printf("Client: %s\n", message);
        if( send(sockfd , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        bzero(message, sizeof(servaddr_reply));
        if( recv(sockfd, servaddr_reply , 2000 , 0) < 0)
        {
            puts("Recv failed");
            printf("Error in receiving data.\n");
            break;
        } 
        else 
        {
            printf("Server: %s\n", servaddr_reply);
            
        }
    }          
    }
        close (sockfd);
        return 0;
}
