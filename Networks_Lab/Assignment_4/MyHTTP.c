#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

void *connection_handler(void *socket_desc);

int main(int argc, char *argv[])
{
    int socket_desc, client_sock, c, *new_sock;
    struct sockaddr_in server, client;
    pthread_t sniffer_thread;
    
    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
        return 1;
    }
    puts("Socket created");
    
    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);
    
    // Bind
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
    
    // Listen
    listen(socket_desc , 3);
    
    // Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    
    while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)))
    {
        puts("Connection accepted");
        
        new_sock = malloc(1);
        *new_sock = client_sock;
        
        if (pthread_create(&sniffer_thread, NULL, connection_handler, (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        
        //Now join the thread, so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }
    
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    
    return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char client_message[2000];
    
    // Send a message to the client
    char *message = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>Hello World</h1></body></html>\n";
    write(sock , message , strlen(message));
    
    // Receive a message from client
    while ((read_size = recv(sock , client_message , 2000 , 0)) > 0)
    {
        // Send the message back to client
        write(sock , client_message , strlen(client_message));
    }
    
    if (read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }
    
    // Free the socket pointer
    free(socket_desc);
    
    return 0;
}
