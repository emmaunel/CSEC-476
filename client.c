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

#define PROMPT "client> "
#define BUFFER 1024

#define SERVER "localhost"
#define PORT 8888

int main(){
    char input[50];

    while(1){
        printf(PROMPT);
        fgets(input, sizeof(input), stdin);
    }
    
}