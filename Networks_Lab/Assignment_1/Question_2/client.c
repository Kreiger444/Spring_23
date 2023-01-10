#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>



char* readinput()
{
#define CHUNK 200
   char* input = NULL;
   char tempbuf[CHUNK];
   size_t inputlen = 0, templen = 0;
   do {
       fgets(tempbuf, CHUNK, stdin);
       templen = strlen(tempbuf);
       input = realloc(input, inputlen+templen+1);
       strcpy(input+inputlen, tempbuf);
       inputlen += templen;
    } while (templen==CHUNK-1 && tempbuf[CHUNK-2]!='\n');
    return input;
}


int main()
{
    int sockfd;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Cannot create socket");
        exit(0);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(20000);

    int status;
    status = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    if (status < 0)
    {
        perror("Unable to bind local address\n");
        exit(0);
    }

    do
    {
        char *result = readinput();
        int sodhi= sizeof(result);
        // printf("%d\n", sodhi);


        if (!strcmp(result, "-1"))
        {
            break;
        }

        char buff[sizeof(result)];
        strcpy(buff, result);
        free(result);
        // printf("%lu\n", sizeof(buff));
        send(sockfd, buff, strlen(buff) + 1, 0);

        // printf("sent\n");
        float answer;
        recv(sockfd, &answer, sizeof(buff)+1, 0);
        printf("Result: %f", answer);

    } while (1);
    close(sockfd);
    return 0;
}