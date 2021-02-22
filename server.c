#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write


#define COMMAND "a"
#define DOWNLOAD "d"
#define PING "p"
#define SHELL "s"
#define EXIT "e"

#define MAX 1024

void yay_connection(int sockfd){

    char buf[MAX];

    for(;;){
        bzero(buf, MAX);

        // read the message from client and copy it in buffer
        read(sockfd, buf, sizeof(buf));
        printf("From client: %s\n", buf);
        // bzero(buf, MAX);

        if (strncmp(EXIT, buf, strlen(EXIT)) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        } else if (strncmp(COMMAND, buf, strlen(COMMAND)) == 0) {
            /* code */
        } else if (strncmp(DOWNLOAD, buf, strlen(DOWNLOAD)) == 0) {
            /* code */
        } else if (strncmp(PING, buf, strlen(PING)) == 0) {
            /* code */
        } else if (strncmp(SHELL, buf, strlen(SHELL)) == 0) {
            /* code */
        } 

        // if(buf == 0) {
        //     puts("Client disconnected");
        //     fflush(stdout);
        // }else if(buf == -1){
        //     perror("recv failed");
        
        // }
        
    }

    write(sockfd, "test", 4);
}

int main(){
    int sockfd, connfd, len;
    struct sockaddr_in server, client; 

    //Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        printf("Couldn't create a socket\n");
        exit(0);
    }

    printf("Socket created successfully\n");

     //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 ); // Get user input in future

    //Bind the socket
    if(bind(sockfd, (struct sockaddr *)& server, sizeof(server)) != 0){
        printf("Socket binding failed\n");
        exit(0);
    }

    printf("Socket bind successfully\n");

    // Time to listen
    if(listen(sockfd, 5) != 0){
        printf("For some reason, I can't listen. Good bye\n");
        exit(0);
    }

    printf("Server listening.....\n");
    len = sizeof(client);

    //Accept connection
    connfd = accept(sockfd, (struct sockaddr *)&client, &len);
    if(connfd < 0){
        printf("Server accept failed.\n");
        exit(0);
    }

    yay_connection(connfd);

    close(sockfd);

    // Wait for clients and spawn a new thread 

}