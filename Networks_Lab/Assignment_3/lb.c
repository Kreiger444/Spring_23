#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <time.h>

char* reciever(int sockfd)
{
    char *recstr;
    int alloc_size = 50;
    recstr = (char *)malloc(sizeof(char) * alloc_size);
    int recstrlen = 0;
    char buff[50];
    while (1)
    {

        int reclen = recv(sockfd, buff, 50, 0);
        while (recstrlen + reclen >= alloc_size)
        {
            alloc_size += 50;
        }
        recstr = realloc(recstr, alloc_size);
        strcat(recstr, buff);
        if (buff[reclen - 1] == '\0' || buff[reclen - 1] == '\n')
        {
            break;
        }
    }

    return recstr;
}

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
    int sockfd, newsockfd;
    struct sockaddr_in cliaddr, serv1addr, serv2addr, laddr;

    int timeout = 5000;
    time_t oldtime, newtime;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Cannot create socket");
        exit(0);
    }

    laddr.sin_family = AF_INET;
    laddr.sin_addr.s_addr = INADDR_ANY;
    laddr.sin_port = htons(atoi(argv[1]));

    serv1addr.sin_family = AF_INET;
    serv1addr.sin_addr.s_addr = INADDR_ANY;
    serv1addr.sin_port = htons(atoi(argv[2]));

    serv2addr.sin_family = AF_INET;
    serv2addr.sin_addr.s_addr = INADDR_ANY;
    serv2addr.sin_port = htons(atoi(argv[3]));

    int status = bind(sockfd, (struct sockaddr *)&laddr, sizeof(laddr));
    if (status < 0)
    {
        perror("unable to bind");
        exit(0);
    }

    status = listen(sockfd, 5);
    if (status < 0)
    {
        perror("Listen error");
        exit(0);
    }
    printf("Listening at port: %s...\n", argv[1]);

    char *loadstring = "Send Load";
    char *timestring = "Send Time";

    int load1 = 0, load2 = 0;

    while (1)
    {
        struct pollfd fds[1];
        fds[0].fd = sockfd;
        fds[0].events = POLLIN;

        int temp = poll(fds, 1, timeout);

        if (temp < 0)
        {
            perror("POLL ERROR");
            exit(0);
        }
        else if (temp == 0)
        {

            int var1 = socket(AF_INET, SOCK_STREAM, 0);
            int var2 = socket(AF_INET, SOCK_STREAM, 0);

            char *buff;

            int flag = connect(var1, (struct sockaddr *)&serv1addr, sizeof(serv1addr));
            if (flag < 0)
            {
                perror("cant connect to server 1");
                exit(0);
            }

            sender(var1, loadstring, sizeof(loadstring) + 1);
            buff = reciever(var1);

            load1 = atoi(buff);

            printf("Load received from server 1: %d\n", load1);

            flag = connect(var2, (struct sockaddr *)&serv2addr, sizeof(serv2addr));
            if (flag < 0)
            {
                perror("cant connect to server 2");
                exit(0);
            }

            sender(var2, loadstring, sizeof(loadstring) + 1);
            buff = reciever(var2);

            load2 = atoi(buff);

            printf("Load received from server 2: %d\n", load2);

            timeout = 5000;
        }

        else if (temp > 0)
        {
            int clilen = sizeof(cliaddr);
            newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
            if (newsockfd < 0)
            {
                perror("accept error");
                exit(0);
            }

            if (fork() == 0)
            {
                int serverfd = socket(AF_INET, SOCK_STREAM, 0);
                char *buff;
                if (load1 < load2)
                {

                    int flag = connect(serverfd, (struct sockaddr *)&serv1addr, sizeof(serv1addr));
                    if (flag < 0)
                    {
                        perror("cant connect to server 1");
                        exit(0);
                    }

                    sender(serverfd, timestring, sizeof(timestring) + 1);
                    printf("Sending client request to server 1\n");
                    buff = reciever(serverfd);
                    close(serverfd);
                }
                else
                {

                    int flag = connect(serverfd, (struct sockaddr *)&serv2addr, sizeof(serv2addr));
                    if (flag < 0)
                    {
                        perror("cant connect to server 2");
                        exit(0);
                    }

                    sender(serverfd, timestring, sizeof(timestring) + 1);
                    printf("Sending client request to server 2\n");
                    buff = reciever(serverfd);
                    close(serverfd);

                }

                sender(newsockfd, buff, strlen(buff) + 1);
                close(newsockfd);
                exit(0);
            }
            newtime=time(NULL);
            timeout=timeout-1000*difftime(oldtime, newtime);
            if(timeout<0){
                timeout=0;
            }
        }
    }
}