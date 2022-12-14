#include "Receiver.h"
#define FILE_SIZE 2167736
#define chank 50000

struct timeNode{
    double time;
    struct timeNode *next;
};
typedef struct timeNode TN;

void printPacketTime(TN *head, int *Pindex, int index, int sum, double time, int packet){
    TN *temporary = head;
    Pindex = &index;

    while(temporary != NULL){
        index++;
        time = temporary->time;
        sum += time;
        if(packet == 1){
            printf("time of first part of Packet %d is: %f \n" , index , time);
        }
        else{
            printf("time of second part of Packet %d is: %f \n" , index , time);

        }
        temporary = temporary->next;
    }
}

TN *createNewNode(double time){
    TN *result = malloc(sizeof(TN));
    result->time = time;
    result->next = NULL;
    return result;
}


int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in senderAddress;
    memset(&senderAddress, 0, sizeof(senderAddress)); //not necessary to insert si_zero because everything is 0 now.
    senderAddress.sin_family = AF_INET;
    senderAddress.sin_port = htons(port);
    int checkP = inet_pton(AF_INET, (const char *) IP, &senderAddress.sin_addr);
    if (checkP <= 0) {
        printf("inet_pton() failed\n");
    }
    int Bcheck = bind(sock, (struct sockaddr *) &senderAddress, sizeof(senderAddress));
    if (Bcheck == -1) {
        printf("error while binding\n");
    }
    int Lcheck = listen(sock, 1);
    if (Lcheck == -1) {
        printf("error in listen()\n");
    }

    //the first node is a Demo node
    TN *firstNodePart1;
    TN *firstNodePart2;
   

    char MsgBuffer[FILE_SIZE / 2] = {'0'};

    memset(&senderAddress, 0, sizeof(senderAddress));
    unsigned int senderAddressLen = sizeof(senderAddress);
    int senderSock = accept(sock, (struct sockaddr *) &senderAddress, &senderAddressLen);
    if (senderSock == -1) {
        printf("accept() failed\n");
        close(sock);
        return -1;
    }
    while(1) {
        //receive the first part + measure the time of first part:
        clock_t start , end;
        start = clock();
        int got = 0;
        int bytes = 0;
        while (got < FILE_SIZE/2) {
            bytes = (int)(recv(senderSock, MsgBuffer,chank, 0));
            if(bytes == -1){
                printf("error in recv()\n");
                return -1;
            }
            if(bytes < 2){ 
                break;
            }
             got += bytes;
        }
        end = clock();

        if(bytes < 2){
            break;
        }
        double measureTime =(double) (end - start)/CLOCKS_PER_SEC;
        //save the time:
        TN *node;
        node = createNewNode(measureTime);
        node->next = firstNodePart1;
        firstNodePart1 = node;
        //********************send back authentication:**********************
        //
        int x = (7351^4015);
        if((int)(send(senderSock, &x, sizeof(x) - 1, 0)) == -1){
           printf("error in send()\n");
        }
        //the sender receive the authentication and check if its right.
        //
        //*********************^to be completed^*****************************
        //
        //
        //change CC Algorithm:
        if(setsockopt(senderSock,IPPROTO_TCP,TCP_CONGESTION,"cubic", 5) == -1) {
            printf("setsockopt() failed\n");
        }
        //receive the second part + measure the time of second part
        start = clock();
        got = 0;

        while (got < FILE_SIZE / 2){
            bytes = (int)(recv(senderSock, MsgBuffer,chank , 0));
            if(bytes == -1){
                printf("error in recv()\n");
                return -1;
            }
            if( bytes < 2) {
                break;
            }
            got += bytes;
        }
        end = clock();
        if(bytes < 2 ) {
            break;
        }
        measureTime =(double) (end - start)/CLOCKS_PER_SEC;
        //save the time

        TN *node2;
        node2 = createNewNode(measureTime);
        node2->next = firstNodePart2;
        firstNodePart2 = node2;

        //if get exit message:
            //print times
            //print avrg time of first part
            //print avrg time of second part

    }
    int *Pindex1;
    int *Pindex2;
    int index1;
    int index2;
    int sum1 = 0;
    int sum2 = 0;

    
    firstNodePart1 = firstNodePart1->next;
    printPacketTime(firstNodePart1, Pindex1, index1, sum1, 0, 1);

    firstNodePart2 = firstNodePart2->next;
    printPacketTime(firstNodePart2, Pindex2, index2, sum2, 0, 2);

    //CHECK WHY "index1", "index2" = 0 !!!!!!!
    printf("%d\n", index1);
    printf("%d\n", index2);


    float avg = (float)(sum1 / index1);
    printf("the average of first part is: %f\n" , avg);
    
    avg = (float)(sum2 / index2);
    printf("the average of second part is: %f\n" , avg);
    
    return 0;
}