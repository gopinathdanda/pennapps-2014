#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <time.h> 

int main(int argc, char *argv[]){
int listenfd = 0 , connfd = 0; 
struct sockaddr_in serv_addr; 
char recvbuffer[1024]; 
char *message =" I am the server\n\r";
char *string; 
// creating the socket
listenfd = socket(AF_INET, SOCK_STREAM, 0); 

memset(&serv_addr, '0', sizeof(serv_addr)); 

// setting the members of struct serv_addr
serv_addr.sin_family = AF_INET; 
serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
serv_addr.sin_port = htons(5000); 

//binding the socket descriptor to a port 
bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

//listening for connections
listen(listenfd, 10); 
//printf("Reached here\n");
while(1){ 
// accepting connection from client
connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
//send(connfd, message, strlen(message),0); 
if(recv(connfd,recvbuffer,sizeof(recvbuffer),0)< 0){ 
printf("No data received from client");
}
else{
//do something with the data received in recvbuff 
} 
//close the connection
close(connfd); 
sleep(1); 
}
}