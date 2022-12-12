//
// Created by alon on 12/7/22.
//
#include "Sender.h"
#define port 8888
#define IP "127.0.0.1"

int main(){
    printf("a");
    FILE * fPointer;
    fPointer = fopen( "2mb_file.txt","r");
    struct stat st;
    stat("2mb_file.txt", &st);
    int size = st.st_size;
    //assuming that the size is even (NEED TO CHECK FOR ODD)
    int halfSize = size/2;
    char firstHalf[halfSize];
    fgets(firstHalf, halfSize ,fPointer);
    int seconedHalfSize = halfSize;
    if(size % 2 == 1) {
        seconedHalfSize++;
    }
        char secondHalf[seconedHalfSize];
        fgets(secondHalf, seconedHalfSize, fPointer + halfSize);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        printf("Could not create socket");
    }
    struct sockaddr_in receiver_adderess;
    memset(&receiver_adderess,0,sizeof(receiver_adderess));
    receiver_adderess.sin_family = AF_INET;
    receiver_adderess.sin_port = htons(port);
    int checkP = inet_pton(AF_INET,(const char*)IP,&receiver_adderess.sin_addr);
    if(checkP <= 0){
        printf("inet_pton() failed");
    }

    int connectCheck = connect(sock,(struct sockaddr*) &receiver_adderess,sizeof(receiver_adderess));
    if(connectCheck == -1){
        printf("connect() failed");
    }
    char CC[10] ="reno";
    int lenCC = strlen(CC);
    int checkSSO = setsockopt(sock,IPPROTO_TCP,TCP_CONGESTION,CC, lenCC );
    if(checkSSO == -1){
        printf("setsockopt() failed");
    }
// authentication check
//
//
    char again = 'y';
    while (again == 'y' || again == 'Y') {

        send(sock, firstHalf, halfSize, 0);
//send the first part
//
//change CC algorithm.
        strcpy(CC, "cubic");
        lenCC = strlen(CC);
        int checkSSO2 = setsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, CC, lenCC);
        if (checkSSO2 == -1) {
            printf("second setsockopt() failed");
        }
//
//send the second part
        if( send(sock, secondHalf, seconedHalfSize, 0) == -1){
            printf("send() failed");
        }
//
// do u want to send again?
        again = '0';
        while ((again !='y') &&(again !='Y') && (again !='N') &&(again !='n')  ) {
            printf("do u want to send again? (y/n)");
            scanf("%c", &again);
        }
    }
 //send exit message ????????????????
    send(sock, "exit message",13,0);
//close tcp connection:
    printf("closing connection...");
    close(sock);
    printf("connection closed");
//close the file
    fclose(fPointer);
}