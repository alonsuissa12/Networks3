//
// Created by alon on 12/7/22.
//
#include "Sender.h"
#define port 8888
#define IP "127.0.0.1"

int main(){
    //
    //need to read the file!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        printf("Could not create socket");
    }
    //sockaddr - stract of adress and port
    //the size of ^
    struct sockaddr_in senderAddress;
    memset(&senderAddress,0,sizeof(senderAddress)); //not necessary to insert si_zero because everything is 0 now.
    senderAddress.sin_family = AF_INET;
    senderAddress.sin_port = htons(port);
    int checkP = inet_pton(AF_INET,(const char*)IP,&senderAddress.sin_addr);
    if(checkP <= 0){
        printf("inet_pton() failed");
    }
    int connectCheck = connect(sock,(struct sockaddr*) &senderAddress,sizeof(senderAddress));
    if(connectCheck == -1){
        printf("connect() failed");
    }

}