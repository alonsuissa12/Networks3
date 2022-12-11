//
// Created by alon on 12/7/22.
//
#include <sys/stat.h>
#include "Sender.h"
#define port 8888
#define IP "127.0.0.1"

int main(){
    FILE * fPointer;
    fPointer = fopen( "2mb_file.txt","r");
    struct stat st;
    stat("2mb_file.txt", &st);
    int size = st.st_size;
    //assuming that the size is even (NEED TO CHECK FOR ODD)
    int halfSize = size/2;
    char firstHalf[halfSize];
    fgets(firstHalf, halfSize ,fPointer);
    if(size % 2 == 1){
        char secondHalf[halfSize +1];
        fgets(secondHalf, halfSize + 1, fPointer + halfSize);
    }
    else {
        char secondHalf[halfSize];
        fgets(secondHalf, halfSize, fPointer + halfSize);
    }

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
    //setsockopt(sock  );
//send the first part
//
//change CC algorithm.
//
//send the second part
//
// do u want to send again?
//
//


//close the file
}