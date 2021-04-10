#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#define MAX_MSG 100
#define SERVER_ADDR "127.0.0.1"
#define CLIENT_ADDR "127.0.0.1"

int main (int argc, char* argv[]) {
    int sd, rc, i,n, portno;
    struct sockaddr_in clientAddr, servAddr;
    char line[MAX_MSG];

    /**********************************/ 
    /* build server address structure */
    /**********************************/ 

    portno = atoi(argv[1]); 
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    servAddr.sin_port = htons(portno);

    /**********************************/ 
    /* build client address structure */
    /**********************************/ 

    bzero((char *)&clientAddr, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = INADDR_ANY;
    clientAddr.sin_port = htons(0);

    /************************/ 
    /* create stream socket */
    /************************/ 

    sd = socket(AF_INET, SOCK_STREAM, 0);

    printf("successfully created stream socket \n");

    /**************************/ 
    /* bind local port number */
    /**************************/ 

    bind(sd, (struct sockaddr *) &clientAddr, sizeof(clientAddr));

    printf("bound local port successfully\n");

    /*********************/ 
    /* connect to server */
    /*********************/

    connect(sd, (struct sockaddr *) &servAddr, sizeof(servAddr));
    printf("connected to server successfully\n");

    /***********************/
    /* send data to server */
    /***********************/

    do{
    	printf("Command: 1 to add, 2 to sub, 3 to div, 4 to mul\n");
        printf("Enter command : ");
        scanf("%s", line);
        send(sd, line, strlen(line) + 1, 0);
        
        printf("Enter 1st number : "), scanf("%s", line);

        send(sd, line, strlen(line) + 1, 0);
        printf("data sent (%s)\n", line);  
        printf("Enter 2nd number : "), scanf("%s", line);
        send(sd, line, strlen(line) + 1, 0);
        printf("data sent (%s)\n", line);

        n=recv(sd, line, MAX_MSG, 0);

        printf("Answer %s\n", line);

    } while(strcmp(line, "quit"));

    printf("closing connection with the server\n");
    close(sd);
}
