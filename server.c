#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX_MSG 100
#define SERVER_ADDR "127.0.0.1"

//#define SERVER_PORT 8081

int main (int argc, char* argv[]) {
    int sd, newSd, cliLen, n,num1,num2,sum, portno, operator;
    struct sockaddr_in cliAddr, servAddr;
    char line[MAX_MSG],line1[MAX_MSG],line2[MAX_MSG];

    /**********************************/ 
    /* build server address structure */
    /**********************************/ 

    portno = atoi(argv[1]);
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    servAddr.sin_port = htons(portno);

    /************************/ 
    /* create stream socket */
    /************************/ 

    sd = socket(AF_INET, SOCK_STREAM, 0);

    printf("successfully created stream socket \n");

    /**************************/ 
    /* bind local port number */
    /**************************/ 

    bind(sd, (struct sockaddr *) &servAddr, sizeof(servAddr));
    printf("bound local port successfully\n");

    /********************************/ 
    /* specify number of concurrent */
    /* clients to listen for        */
    /********************************/ 

    listen(sd,5);

    while(1){
        printf("waiting for client connection on port TCP %u\n",portno);

        /*****************************/
        /* wait for client connection*/
        /*****************************/  

        cliLen = sizeof(cliAddr);
        newSd = accept(sd, (struct sockaddr *) &cliAddr, &cliLen);

        printf("received connection from host [IP %s ,TCP port %d]\n",

                    inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));

        /*****************************/
        /* wait for data from client */
        /*****************************/    
    
        do{
            memset(line,0x0,MAX_MSG);
            n=recv(newSd, line, MAX_MSG, 0); 	
            operator = atoi(line);
            n=recv(newSd, line, MAX_MSG, 0);
            num1=atoi(line);
            n=recv(newSd, line, MAX_MSG, 0);
            num2=atoi(line);
            printf("Used command: ");
            if (operator == 1){
            	printf("Add\n");
                sum=num1+num2;
            }

            else if (operator == 2){
            	printf("Sub\n");
                sum=num1-num2;
            }
        
            else if (operator == 3){
            	printf("Div\n");
                sum= num1/num2;
            }

            else if (operator == 4){   
            	printf("Mul\n"); 
                sum= num1 * num2;
            }
		
        printf("\nnum1 = %d", num1);
        printf("\nnum2 = %d", num2);
        printf("\nsum = %d\n", sum);

        sprintf(line1,"%d",sum);

        printf("received from host [IP %s ,TCP port %d] : %s\n",

                    inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), line1);

    send(newSd, line1, strlen(line1) + 1, 0);
        }while(abs(strcmp(line, "quit")));

    /**************************/
    /* close client connection*/
    /**************************/  

    printf("closing connection with host [IP %s ,TCP port %d]\n",

    inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
    close(newSd);

    }

}
