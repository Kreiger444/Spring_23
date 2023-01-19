#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <time.h>
#include <poll.h>


#define MAXLINE 1024

int main(){
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    sockfd=socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd<0){
        perror("Socket creation failed\n");
        exit(0);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=INADDR_ANY;
    servaddr.sin_port=htons(8181);

    int status=bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    
    if(status<0){
        perror("Bind failed\n");
        exit(0);
    }
    

    int n;
    socklen_t len;
    char buffer[MAXLINE];
    len=sizeof(cliaddr);
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, ( struct sockaddr *) &cliaddr, &len); 
    buffer[n] = '\0'; 
    // printf("%s\n", buffer); 
    
    time_t curr_time;
    curr_time=time(NULL);
    char* buff;
    buff = (char*) malloc(sizeof(char)*50);
    strcpy(buff, ctime(&curr_time));
    sendto(sockfd, (const char*)buff, strlen(buff)+1, 0, (const struct sockaddr *) &cliaddr, sizeof(cliaddr));
    // printf("time sent");
    close(sockfd);
    return 0;
}