#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char *readinput()
{
#define CHUNK 200
    char *input = NULL;
    char tempbuf[CHUNK];
    size_t inputlen = 0, templen = 0;
    do
    {
        fgets(tempbuf, CHUNK, stdin);
        templen = strlen(tempbuf);
        input = realloc(input, inputlen + templen + 1);
        strcpy(input + inputlen, tempbuf);
        inputlen += templen;
    } while (templen == CHUNK - 1 && tempbuf[CHUNK - 2] != '\n');
    return input;
}

void recvfunc(char *buffer, int newsockfd)
{
    char *recstr;
    char buff[50];
    int alloc_size = 50;
    recstr = (char *)malloc(sizeof(char) * alloc_size);
    int recstrlen = 0;
    while (1)
    {
        int reclen = recv(newsockfd, buff, 50, 0);
        buff[reclen]='\0';

        while (recstrlen + reclen >= alloc_size)
        {
            alloc_size += 50;
        }
        recstr = realloc(recstr, alloc_size);
        strcat(recstr, buff);
        if (buff[reclen - 1] == '\0')
        {
            break;
        }
    }
    strcpy(buffer,recstr);
}

int main()
{
    int sockfd;
    struct sockaddr_in serv_addr;

    int i;
    char buff[100];
    char *lastsend;
    lastsend = (char *)malloc(sizeof(char) * 200);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Cannot create socket\n");
        exit(0);
    }

    serv_addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &serv_addr.sin_addr);
    serv_addr.sin_port = htons(20000);

    int status = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (status < 0)
    {
        perror("Unable to connect to server\n");
        exit(0);
    }
    for (int i = 0; i < 100; i++)
    {
        buff[i] = '\0';
    }
    recv(sockfd, buff, 100, 0);
    printf("%s", buff);
    char *inp = readinput();
    strcpy(buff, inp);
    free(inp);
    send(sockfd, buff, strlen(buff) + 1, 0);
    char foundbuff[10];
    recv(sockfd, foundbuff, 10, 0);
    if (strcmp(foundbuff, "NOT-FOUND") == 0)
    {
        exit(0);
    }
    while (1)
    {
        printf("Enter command:");
        char *comm = readinput();
        strcpy(lastsend, comm);
        send(sockfd, lastsend, sizeof(lastsend) + 1, 0);
        printf("\n");
        if (strcmp(lastsend, "exit\n") == 0)
        {
            exit(0);
        }
        recvfunc(buff, sockfd);
        printf("%s\n", buff);
    }
}