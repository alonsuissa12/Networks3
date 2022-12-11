//
// Created by alon on 12/7/22.
//
#define port 8888
#define IP "127.0.0.1"
#include "Receiver.h"
int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in senderAddress;
    memset(&senderAddress, 0, sizeof(senderAddress)); //not necessary to insert si_zero because everything is 0 now.
    senderAddress.sin_family = AF_INET;
    senderAddress.sin_port = htons(port);
    int checkP = inet_pton(AF_INET, (const char *) IP, &senderAddress.sin_addr);
    if (checkP <= 0) {
        printf("inet_pton() failed");
    }
    int Bcheck = bind(sock, (struct sockaddr *) &senderAddress, sizeof(senderAddress));
    if (Bcheck == -1) {
        printf("error while binding");
    }
    int Lcheck = listen(sock, 1);
    if (Lcheck == -1) {
        printf("error in listen()");
    }
    while(1) {
        memset(&senderAddress, 0, sizeof(senderAddress));
        int senderAddressLen = sizeof(senderAddress);
        int senderSocket = accept(sock, (struct sockaddr *) &senderAddress, &senderAddressLen);
        if(senderSocket == -1){
            printf("accept() failed");
            // need to close the socket
            return -1;
        }




    }

}