//
// Created by alon on 12/7/22.
//

#include "Receiver.h"
#define FILE_SIZE 2167736
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
    char MsgFirstHalf[FILE_SIZE / 2] = { '0'};
    char MsgSecondHalf[(FILE_SIZE / 2) + 1] = { '0'};
    while(1) {
        memset(&senderAddress, 0, sizeof(senderAddress));
        unsigned int senderAddressLen = sizeof(senderAddress);
        int senderSock = accept(sock, (struct sockaddr *) &senderAddress, &senderAddressLen);
        if (senderSock == -1) {
            printf("accept() failed");
            close(sock);
            return -1;
        }
        //receive the second part + measure the time of second part:
        clock_t start , end;
        start = clock();
        int got = 0;
        int bytes = 0;
        while (got <= FILE_SIZE / 2) {
            bytes = (int)(recv(senderSock, MsgFirstHalf,(FILE_SIZE/2), 0));
            if(bytes == -1){
                printf("error in recv()");
            }
            if(bytes <= 2) {
                break;
            }
            got = got + bytes;
        }
        end = clock();
        if(bytes <= 2) {
            break;
        }
        double measureTime =(double) (end - start)/CLOCKS_PER_SEC;
        //save the time:
        struct timeNode n;
        n.time = measureTime;
        n.next = NULL;
        lastNodePart1->next = &n;
        lastNodePart1 = &n;
        //********************send back authentication:**********************
        char CC[6] ="reno";
        if(setsockopt(senderSock,IPPROTO_TCP,TCP_CONGESTION,CC, strlen(CC) ) == -1){
            printf("setsockopt() failed");
        }
        //
        //*********************to be compleated******************
        //
        //
        //change CC Algorithm:
        strcpy(CC , "cubic");
        if(setsockopt(senderSock,IPPROTO_TCP,TCP_CONGESTION,CC, strlen(CC) ) == -1) {
            printf("setsockopt() failed");
        }
        //receive the second part + measure the time of second part
        start = clock();
        got = 0;
        int add = 0;
        if(FILE_SIZE % 2 == 1){
            add ++;
        }
        while (got <= (FILE_SIZE / 2) + add) {
            bytes = (int)(recv(senderSock, MsgSecondHalf,(FILE_SIZE/2) + 1, 0));
            if(bytes == -1){
                printf("error in recv()");
            }
            if( bytes <=2) {
                break;
            }
            got = got + bytes;
        }
        end = clock();
        if(bytes < 2 ) {
            break;
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
            // brake;
    }
    int index1 = 1;
    int index2 = 1;
    double sum1 = 0;
    double sum2 = 0;
    struct timeNode * Pnode = firstNodePart1.next;
    while(Pnode != 0){
        double time = (Pnode->time);
        sum1 = sum1 +time;
        printf("time of first part , of Packet %d is: %f \n" , index1 , time);
        Pnode = Pnode->next;
    }
    double avg = sum1 / index1 ;
    printf("the average of first part is: %f\n" , avg);
    Pnode = firstNodePart2.next;
    while(Pnode != 0) {
        double time = (Pnode->time);
        sum2 = sum2 + time;
        printf("time of second part , of Packet %d is: %f \n", index1, time);
        Pnode = Pnode->next;
    }
    avg = sum2 / index2 ;
    printf("the average of second part is: %f\n" , avg);
return 0;
}