//
// Created by alon on 12/7/22.
//
#include "Sender.h"
#define chank 50000

int main(){
    FILE * fPointer;
    fPointer = fopen( "2mb_file.txt","r");
    struct stat st;
    stat("2mb_file.txt", &st);
    int size = (int)st.st_size;
    char allFIle [size];
    int halfSize = size/2;
    char firstHalf[halfSize];
    fgets(allFIle, size ,fPointer);
    int index = 0;
    for(int i = 0; i < halfSize; i++){
        firstHalf[index] = allFIle[index];
        index++;
    }
    char secondHalf[halfSize];
    for(int i = 0; i < halfSize; i++){
        secondHalf[index] = allFIle[index];
        index++;
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
    if(checkP < 0){
        printf("inet_pton() failed");
    }

    int connectCheck = connect(sock,(struct sockaddr*) &receiver_adderess,sizeof(receiver_adderess));
    if(connectCheck == -1){
        printf("connect() failed");
    }
    char again = 'y';
    while (again == 'y' || again == 'Y') {
        char CC[7] ="reno";
        int checkSSO = setsockopt(sock,IPPROTO_TCP,TCP_CONGESTION,CC, strlen(CC) );
        if(checkSSO == -1){
            printf("setsockopt() failed");
        }
        //send the first part:
        if(send(sock, firstHalf, sizeof (firstHalf), 0) == -1){
            printf("error in send()");
        }


//authentication check!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        int gotX;
        recv(sock, &gotX, sizeof(gotX),0);
        if(gotX == 7351^4015){
            printf("GOOD!\n");
        }
        else{
            printf("BAD!\n");
        }


//change CC algorithm.
        strcpy(CC, "cubic");
        int checkSSO2 = setsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, CC, strlen(CC));
        if (checkSSO2 == -1) {
            printf("second setsockopt() failed");
        }
//
//send the second part
        if( send(sock, secondHalf, chank, 0) == -1){
            printf("send() failed");
        }
//
// do u want to send again?
        again ='0';
        while ((again !='y') &&(again !='Y') && (again !='N') &&(again !='n')  ) {
            printf("do u want to send again? (y/n)\n");
            if( scanf("%c", &again) != 1){
                printf("error in scanf()");

            }
        }
    }
 //send exit message ????????????????
    printf("sending exit massage");
    send(sock, "",1,0);
//close tcp connection:
    printf("closing connection...\n");
    close(sock);
    printf("connection closed\n");
//close the file
    fclose(fPointer);
}