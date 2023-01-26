#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>

void sender(int newsockfd, char *buffer, size_t buffsize)
{
    char *sendbuff = buffer;

    while (buffsize > 0)
    {
        if (buffsize < 50)
        {
            int n = send(newsockfd, sendbuff, buffsize, 0);
            if (n < 0)
            {
                exit(0);
            }
            sendbuff += n;
            buffsize -= n;
        }
        else
        {
            int n = send(newsockfd, sendbuff, 50, 0);
            if (n < 0)
            {
                exit(0);
            }
            sendbuff += n;
            buffsize -= n;
        }
    }
}

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    srand(atoi(argv[1]));
    int clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Socket creation failed\n");
        exit(0);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(atoi(argv[1]));

    int status = bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    if (status < 0)
    {
        perror("Bind failed\n");
        exit(0);
    }

    int flag = listen(sockfd, 5);

    if (flag < 0)
    {
        perror("Listen error");
        exit(0);
    }
    printf("port %s\n", argv[1]);

    while (1)
    {
        int newsockfd;
        clilen = sizeof(cliaddr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
        char buff[50];
        if (newsockfd < 0)
        {
            perror("Accept error\n");
            exit(0);
        }
        char *recstr;
        int alloc_size = 50;
        recstr = (char *)malloc(sizeof(char) * alloc_size);
        int recstrlen = 0;
        while (1)
        {
            int reclen = recv(newsockfd, buff, 50, 0);
     
            while (recstrlen + reclen >= alloc_size)
            {
                alloc_size += 50;
            }
            recstr = realloc(recstr, alloc_size);
            strcat(recstr, buff);
            if (buff[reclen - 1] == 'd' || buff[reclen-1] == 'e')
            {
                break;
            }
        }
        if (strcmp(buff, "Send Load") == 0)
        {
            int random = rand() % 100 + 1;
            sprintf(buff, "%d", random);
            sender(newsockfd, buff, strlen(buff) + 1);
            printf("Load sent %d\n", random);
        }
        else if (strcmp(buff, "Send Time") == 0)
        {
            time_t timer;
            timer = time(NULL);
            char *time_send = ctime(&timer);

            sender(newsockfd, time_send, strlen(time_send) + 1);
        }
        else{
            sender(newsockfd, "0", 2);
        }
        close(newsockfd);
    }
    close(sockfd);
    return 0;
}