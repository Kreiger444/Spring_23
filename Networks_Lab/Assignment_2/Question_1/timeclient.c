#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <time.h>

#define MAXLINE 1024

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("Socket creation failed");
        exit(0);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8181);
    inet_aton("127.0.0.1", &servaddr.sin_addr);
    char *buffer = "Request for time";
    sendto(sockfd, (const char *)buffer, strlen(buffer), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    // printf("Sent nothing\n");
    char* buff;
    buff = (char*) malloc(sizeof(char)*50);
    int n, len;
    for(int j=0; j<5; j++){
        struct pollfd fdset[1];
        fdset[0].fd=sockfd;
        fdset[0].events=POLLIN;
        int response=poll(fdset,1,3000);
        if(response<0){
            perror("poll error");
            exit(0);
        }
        else if(response==0){
            // printf("time out %d\n",j+1);
            continue;
        }
        else{
            n = recvfrom(sockfd, buff, MAXLINE, 0, (struct sockaddr *)&servaddr, &len);
            printf("Time is: %s", buff);
            break;
        }
        
    }
    close(sockfd);
    return 0;
}