//
// Created by alon on 12/7/22.
//

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

     struct timeNode{
        double time;
        struct timeNode  *next;
    } ;
    //the first node is a Demo node
    struct timeNode firstNodePart1;
    firstNodePart1.time =0.0;
    firstNodePart1.next = NULL;
    struct timeNode firstNodePart2;
    firstNodePart1.time =0.0;
    firstNodePart1.next = NULL;
    struct timeNode* lastNodePart1 = &firstNodePart1;
    struct timeNode* lastNodePart2 = &firstNodePart2;

    while(1) {
        memset(&senderAddress, 0, sizeof(senderAddress));
        unsigned int senderAddressLen = sizeof(senderAddress);
        //receive the second part + measure the time of second part:
        clock_t start , end;
        start = clock();
        int senderSocket = accept(sock, (struct sockaddr *) &senderAddress, &senderAddressLen);
        end = clock();
        double measureTime =(double) (end - start)/CLOCKS_PER_SEC;
        if(senderSocket == -1){
            printf("accept() failed");
            close(sock);
            return -1;
        }
        //save the time:
        struct timeNode n;
        n.time = measureTime;
        n.next = NULL;
        lastNodePart1->next = &n;
        lastNodePart1 = &n;
        //send back authentication
        char CC[6] ="reno";
        if(setsockopt(sock,IPPROTO_TCP,TCP_CONGESTION,CC, strlen(CC) ) == -1){
            printf("setsockopt() failed");
        }
        //
            //to be compleated!!!^^^
        //
        //
        //change CC Algorithm
        strcpy(CC , "cubic");
        if(setsockopt(sock,IPPROTO_TCP,TCP_CONGESTION,CC, strlen(CC) ) == -1) {
            printf("setsockopt() failed");
        }
        //receive the second part + measure the time of second part
        start = clock();
        senderSocket = accept(sock, (struct sockaddr *) &senderAddress, &senderAddressLen);
        end = clock();
        if(senderSocket == -1){
            printf("accept() failed \nclosing socket");
            close(sock);
            return -1;
        }

        //save the time
        lastNodePart2->next = &n;
        n.time =measureTime;
        n.next = NULL;
        lastNodePart2 = &n;
        //if get exit message:
            //print times
            //print avrg time of first part
            //print avvrg time of secind part
            //brake;
    }
return 0;
}