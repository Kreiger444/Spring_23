#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

int main(){
    int sockfd, newsockfd;
    struct sockaddr_in cli_addr, serv_addr;
    int clilen;


    char buff[100];

    sockfd=socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0){
        perror("Cannot create socket");
        exit(0);
    }

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=INADDR_ANY;
    serv_addr.sin_port=htons(20000);

    int status;
    status=bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    if(status<0){
        perror("Unable to bind local address");
        exit(0);
    }

    listen(sockfd, 2);

    for(;;){
        clilen=sizeof(cli_addr);
        newsockfd=accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);

        if(newsockfd<0){
            perror("Acceptance error");
            exit(0);
        }
        
        time_t t;
        t=time(NULL);
        strcpy(buff, ctime(&t));
        send(newsockfd, buff, strlen(buff)+1, 0);
        printf("%s", buff);
        close(newsockfd);
    }

}