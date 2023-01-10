#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

float calculator(char *str)
{
    float num1 = 0;
    int i = 0;
    int flag = 1;
    int flag_bracket = 1;
    char op = '+';
    float multiplier = 10;
    float multiplier_bracket = 10;
    float num2 = 0;
    int temp = 1;
    int temp_bracket = 1;
    float num3 = 0;
    if (str[0] == '-')
    {
        temp = 0;
        i++;
    }
    char op_bracket = '+';
    // printf("%lu", sizeof(str));
    while (str[i] != '\0')
    {
        if (str[i] == '\n')
        {
            multiplier = 10;
            if (temp == 0)
            {
                num2 = num2 * -1;
                temp == 1;
            }
            if (op == '+')
            {
                num1 = num1 + num2;
            }
            if (op == '-')
            {
                num1 = num1 - num2;
            }
            if (op == '*')
            {
                num1 = num1 * num2;
            }
            if (op == '/')
            {
                num1 = num1 / num2;
            }
            num2 = 0;
            op = str[i];
            if (str[i + 1] == '-')
            {
                temp = 0;
            }
            else
            {
                temp = 1;
            }
            return num1;
            break;
        }
        //printf("entered while loop\n");
        if (str[i] >= '0' && str[i] <= '9')
        {
            if (flag == 1)
            {
                num2 = 10 * num2 + (float)(str[i] - '0');
                multiplier = 10;
            }
            if (flag == 0)
            {
                num2 = num2 + ((float)(str[i] - '0')) / multiplier;
                multiplier = multiplier * 10;
            }
        }
        if (str[i] == '.')
        {
            flag = 0;
        }
        if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/')
        {
            multiplier = 10;
            if (temp == 0)
            {
                num2 = num2 * -1;
                temp == 1;
            }
            if (op == '+')
            {
                num1 = num1 + num2;
            }
            if (op == '-')
            {
                num1 = num1 - num2;
            }
            if (op == '*')
            {
                num1 = num1 * num2;
            }
            if (op == '/')
            {
                num1 = num1 / num2;
            }
            num2 = 0;
            op = str[i];
            flag = 1;
            if (str[i + 1] == '-')
            {
                temp = 0;
            }
            else
            {
                temp = 1;
            }
        }
        //printf("%f, %f, %d\n", num1, num2, i);
        //*********************************************************

        if (str[i] == '(')
        {
            //printf("bracket opened\n");
            if (str[i + 1] == '-')
            {
                temp_bracket = 0;
                i++;
            }
            i++;
            while (1)
            {
                //printf("entered brackets while loop\n");
                if (str[i] >= '0' && str[i] <= '9')
                {
                    if (flag_bracket == 1)
                    {
                        num3 = 10 * num3 + (float)(str[i] - '0');
                        multiplier_bracket = 10;
                    }
                    if (flag_bracket == 0)
                    {
                        num3 = num3 + ((float)(str[i] - '0')) / multiplier_bracket;
                        multiplier_bracket = multiplier_bracket * 10;
                    }
                }
                if (str[i] == '.')
                {
                    flag_bracket = 0;
                }
                if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/')
                {
                    multiplier_bracket = 10;
                    if (temp_bracket == 0)
                    {
                        num3 = num3 * -1;
                        temp_bracket == 1;
                    }
                    if (op_bracket == '+')
                    {
                        num2 = num2 + num3;
                    }
                    if (op_bracket == '-')
                    {
                        num2 = num2 - num3;
                    }
                    if (op_bracket == '*')
                    {
                        num2 = num2 * num3;
                    }
                    if (op_bracket == '/')
                    {
                        num2 = num2 / num3;
                    }
                    num3 = 0;
                    op_bracket = str[i];
                    flag_bracket = 1;
                    if (str[i + 1] == '-')
                    {
                        temp_bracket = 0;
                    }
                    else
                    {
                        temp_bracket = 1;
                    }
                }
                //printf("%f, %f, %d\n", num2, num3, i);
                if (str[i] == ')')
                {
                    multiplier_bracket = 10;
                    if (temp_bracket == 0)
                    {
                        num3 = num3 * -1;
                        temp_bracket == 1;
                    }
                    if (op_bracket == '+')
                    {
                        num2 = num2 + num3;
                    }
                    if (op_bracket == '-')
                    {
                        num2 = num2 - num3;
                    }
                    if (op_bracket == '*')
                    {
                        num2 = num2 * num3;
                    }
                    if (op_bracket == '/')
                    {
                        num2 = num2 / num3;
                    }
                    num3 = 0;
                    op_bracket = '+';
                    flag_bracket=1;
                    printf("Exiting loop\n");
                    break;
                }
                i++;
            }
            num3 = 0;
        }
        //*********************************************
        i++;
    }
}

int main()
{

    int sockfd, newsockfd;
    int clilen;
    struct sockaddr_in cli_addr, serv_addr;
    char buff[50];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Cannot create socket");
        exit(0);
    }
    // printf("1\n");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(20000);

    int status;
    status = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    if (status < 0)
    {
        perror("Unable to bind local address\n");
        exit(0);
    }
    // printf("2\n");
    listen(sockfd, 5);

    while (1)
    {
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

        if (newsockfd < 0)
        {
            perror("Accept error\n");
            exit(0);
        }
        // printf("3\n");
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
            if (buff[reclen - 1] == '\0')
            {
                
                break;
            }

        }
        float answer=calculator(recstr);
        printf("%f", answer);
        send(newsockfd, &answer, sizeof(answer), 0);
    }
}