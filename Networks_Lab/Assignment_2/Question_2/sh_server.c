#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <math.h>
#include <dirent.h>

int send_data(int newsockfd, char *buffer, size_t buffsize)
{
    char *sendbuff = buffer;

    while (buffsize > 0)
    {
        if (buffsize < 50)
        {
            int n = send(newsockfd, sendbuff, buffsize, 0);
            if (n < 0)
            {
                return -1;
            }
            sendbuff += n;
            buffsize -= n;
        }
        else
        {
            int n = send(newsockfd, sendbuff, 50, 0);
            if (n < 0)
            {
                return -1;
            }
            sendbuff += n;
            buffsize -= n;
        }
    }
    return 0;
}

void case_pwd(char *finalbuff, int newsockfd)
{
    char s[200];
    if (getcwd(s, 200) == NULL)
    {
        perror("Error");
    }
    else
    {
        strcpy(finalbuff, s);
        send_data(newsockfd, finalbuff, strlen(finalbuff) + 1);

    }
}

void case_cd(char *s, char *finalbuff, int newsockfd)
{
    if (chdir(s) != 0)
    {
        perror("No Such File Or Directory");
    }
    case_pwd(finalbuff, newsockfd);
}

void case_dir(int newsockfd, char* s){
    
    DIR *folder;
    struct dirent *direntry;
    char* finalstring;
    finalstring=(char*)malloc(sizeof(char)*200);
    int alc = 200;
    folder = opendir(s);
    if(folder == NULL)
    {
        strcpy(finalstring, "####");
    }
    else
    {
        while((direntry=readdir(folder))){

            while(alc < strlen(finalstring) + strlen(direntry->d_name)){
                alc += 200;
            }

            finalstring = realloc(finalstring, alc);

            strcat(finalstring, direntry->d_name);
        }
    }
    closedir(folder);

    send_data(newsockfd, finalstring, strlen(finalstring)+1);
}
void recvfunc(char *buff, int newsockfd)
{
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
        if (buff[reclen - 1] == '\0' || buff[reclen-1]=='\n')
        {
            break;
        }
    }
}


int main()
{
    int sockfd, newsockfd;
    int clilen;
    char *finalbuff;
    finalbuff = (char *)malloc(sizeof(char) * 200);
    struct sockaddr_in cli_addr, serv_addr;

    int i;
    char buff[200];
    char *recvbuff;
    recvbuff = (char *)malloc(sizeof(char) * 200);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Cannot create socket\n");
        exit(0);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(20000);

    int status = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (status < 0)
    {
        perror("Unable to bind local address\n");
        exit(0);
    }
    listen(sockfd, 5);

    for (;;)
    {
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

        if (newsockfd < 0)
        {
            perror("Accept error\n");
            exit(0);
        }

        if (fork() == 0)
        {
            close(sockfd);
            strcpy(buff, "LOGIN:");
            send(newsockfd, buff, strlen(buff) + 1, 0);

            recvfunc(recvbuff, newsockfd);
            FILE *filePointer;
            int wordExist = 0;
            int bufferLength = 255;
            char line[bufferLength];
            filePointer = fopen("users.txt", "r");
            while (fgets(line, bufferLength, filePointer))
            {
                char *ptr = strstr(line, recvbuff);
                if (ptr != NULL)
                {
                    wordExist = 1;
                    break;
                }
            }
            fclose(filePointer);
            if (wordExist == 1)
            {
                strcpy(buff, "FOUND");
                send(newsockfd, buff, strlen(buff) + 1, 0);
            }
            else
            {
                strcpy(buff, "NOT-FOUND");
                send(newsockfd, buff, strlen(buff) + 1, 0);
                exit(0);
            }
            while (1)
            {
                recvfunc(buff, newsockfd);
                char cd_store[200];
                char dir_store[200];
                if (strcmp(buff, "pwd\n") == 0)
                {
                    case_pwd(finalbuff, newsockfd);
                }
                else if (buff[0]=='d' && buff[1]=='i' && buff[2]=='r')
                {
                    int flag_1 = 0;
                    for (int k = 3; k < strlen(buff) - 1; k++)
                    {
                        if (flag_1 == 0)
                        {
                            if (buff[k] == ' ')
                            {
                                continue;
                            }
                            else
                            {
                                flag_1 = k;
                                k--;
                                continue;
                            }
                        }
                        else
                        {
                            dir_store[k - flag_1] = buff[k];
                        }
                    }
                    if(strlen(dir_store)==3){
                        case_dir(newsockfd, ".");
                    }
                    case_dir(newsockfd, dir_store);
                }
                else if (buff[0] == 'c' && buff[1] == 'd')
                {
                    int flag = 0;
                    for (int k = 2; k < strlen(buff) - 1; k++)
                    {
                        if (flag == 0)
                        {
                            if (buff[k] == ' ')
                            {
                                continue;
                            }
                            else
                            {
                                flag = k;
                                k--;
                                continue;
                            }
                        }
                        else
                        {
                            cd_store[k - flag] = buff[k];
                        }
                    }
                    case_cd(cd_store, finalbuff, newsockfd);
                }
                else if (strcmp(buff, "exit\n") == 0)
                {
                    break;
                }
            }
            close(newsockfd);
            exit(0);
        }
    }
}