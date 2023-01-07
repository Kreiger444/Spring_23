#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
    int sockfd;
    struct sockaddr_in serv_addr;
    
    char buff[100];
    
    sockfd=socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0){
        perror("Unable to create socket");
        exit(0);
    }

    serv_addr.sin_family	= AF_INET;
	serv_addr.sin_addr.s_addr	= INADDR_ANY;
	serv_addr.sin_port	= htons(20000);

    int status;
    status= connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    if(status<0){
        perror("Unable to connect to server");
        exit(0);
    }

    for(int i=0; i<100; i++){
        buff[i]='\0';
    }
    recv(sockfd, buff, 100, 0);

    printf("Server time: %s\n", buff);
    return 0;
}