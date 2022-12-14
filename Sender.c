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
        printf("Could not create socket.\n");
    }
    struct sockaddr_in receiver_adderess;
    memset(&receiver_adderess,0,sizeof(receiver_adderess));
    receiver_adderess.sin_family = AF_INET;
    receiver_adderess.sin_port = htons(port);
    int checkP = inet_pton(AF_INET,(const char*)IP,&receiver_adderess.sin_addr);
    if(checkP < 0){
        printf("inet_pton() FAILED.\n");
    }

    int connectCheck = connect(sock,(struct sockaddr*) &receiver_adderess,sizeof(receiver_adderess));
    if(connectCheck == -1){
        printf("connect() FAILED.\n");
    }
    char again = 'y';
    while (again == 'y' || again == 'Y') {
        char CC[7] ="reno";
        int checkSSO = setsockopt(sock,IPPROTO_TCP,TCP_CONGESTION,CC, strlen(CC) );
        if(checkSSO == -1){
            printf("setsockopt() FAILED.\n");
        }

        //Sending the first part:
        printf("Sending file...\n");
        if(send(sock, firstHalf, sizeof(firstHalf), 0) == -1){
            printf("Error in send().\n");
        }


        //Authentication check:
        int gotX;
        recv(sock, &gotX, sizeof(gotX),0);
        if(gotX != (7351^4015)){
            printf("Authentication FAILED.\n");
            break;
        }


        //Changing the CC algorithm.
        strcpy(CC, "cubic");
        int checkSSO2 = setsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, CC, strlen(CC));
        if(checkSSO2 == -1) {
            printf("Second setsockopt() FAILED.\n");
        }

        //Sending the second part:
        if(send(sock, secondHalf, sizeof(secondHalf), 0) == -1){
            printf("send() FAILED.\n");
        }

        //Do u want to send again?
        while(1){
            printf("Do u want to send again? (y/n)\n");
            if(scanf(" %c", &again) != 1){
                printf("Scanning FAILED!.\n");
            }
            while((again != 'y') && (again != 'Y') && (again != 'N') && (again != 'n')) {
                printf("Please enter (y/n).\n");
                scanf(" %c", &again);
            }
            if( again == 'n' || again == 'N') {
                printf("Do you want to exit? (y/n)\n");
                if (scanf(" %c", &again) != 1) {
                    printf("Scanning FAILED!.\n");
                }
                while ((again != 'y') && (again != 'Y') && (again != 'N') && (again != 'n')) {
                    printf("Please enter (y/n).\n");
                    if(scanf(" %c", &again) != 1) {
                        printf("Scanning FAILED!.\n");
                    }
                }
                if (again == 'y' || again == 'Y') {
                    printf("Sending exit massage.\n");
                    send(sock, "I want to exit NOW!!!! thanks :)", 34, 0);

                    //Closing TCP connection:
                    printf("Closing connection...\n");
                    close(sock);
                    printf("Connection closed.\n");

                    //Closing the file.
                    fclose(fPointer);
                    return 0;
                }
            }
            else {
                break;
            }
        }
    }

    //Sending exit message:
    printf("Sending exit massage.\n");
    send(sock, "I want to exit NOW!!!! thanks :)",34,0);

    //Closing TCP connection:
    printf("Closing connection...\n");
    close(sock);
    printf("Connection closed.\n");

    //Closing the file.
    fclose(fPointer);
}