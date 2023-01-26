#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in serv_addr;

    char buff[50];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Unable to create socket");
        exit(0);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    int status;
    status = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (status < 0)
    {
        perror("Unable to connect to server");
        exit(0);
    }

    char *recstr;
    int alloc_size = 50;
    recstr = (char *)malloc(sizeof(char) * alloc_size);
    int recstrlen = 0;
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

    printf("Server time: %s", recstr);
    return 0;
}