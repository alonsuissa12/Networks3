//
// Created by alon on 12/7/22.
//
#define port 8888
#define IP "127.0.0.1"
#include "Receiver.h"
void main(){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in receiver_adderess;
    memset(&receiver_adderess,0,sizeof(receiver_adderess));
    receiver_adderess.sin_family = AF_INET;
    receiver_adderess.sin_port = htons(port);
    int checkP = inet_pton(AF_INET,(const char*)IP,&receiver_adderess.sin_addr);
    if(checkP <= 0){
        printf("inet_pton() failed");
    }
    int Bcheck = bind(sock,(struct sockaddr *)&receiver_adderess ,sizeof(receiver_adderess));
    if(Bcheck == -1){
        printf("error while binding");
    }
    int Lcheck = listen(sock,1);
    if(Lcheck == -1){
        printf("error in listen()");
    }


}