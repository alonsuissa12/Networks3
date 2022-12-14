#include "Receiver.h"
#define FILE_SIZE 2167736
#define CHANK 50000
#define EXIT "I want to exit NOW!!!! thanks :)"

struct timeNode{
    double time;
    struct timeNode *next;
};
typedef struct timeNode TN;

double * printPacketTime(TN *head, double index, double sum, double time, int packet){
    
    static double arr[2]; 
    TN *temporary = head;

    while(temporary != NULL){
        index++;
        time = temporary->time;
        sum += time;
        if(packet == 1){
            printf("Time of first part of Packet %d is: %f \n" , (int)index , time);
        }
        else{
            printf("Time of second part of Packet %d is: %f \n" , (int)index , time);

        }
        temporary = temporary->next;
    }

    arr[0] = index;
    arr[1] = sum;

    return arr;
}

TN *createNewNode(double time){
    TN *result = malloc(sizeof(TN));
    result->time = time;
    result->next = NULL;
    return result;
}

TN* reverse(TN *head){
    TN *prev = NULL;
    TN *next = NULL;
    while(head!= NULL){
        next = head->next;
        head->next = prev;
        prev = head;
        head = next;
    }
    head = prev;
    return head;
}


int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in senderAddress;
    memset(&senderAddress, 0, sizeof(senderAddress)); //Not necessary to insert si_zero because everything is 0 now.
    senderAddress.sin_family = AF_INET;
    senderAddress.sin_port = htons(port);
    int checkP = inet_pton(AF_INET, (const char *) IP, &senderAddress.sin_addr);
    if (checkP <= 0) {
        printf("inet_pton() failed.\n");
    }
    int Bcheck = bind(sock, (struct sockaddr *) &senderAddress, sizeof(senderAddress));
    if (Bcheck == -1) {
        printf("Error while binding.\n");
    }
    int Lcheck = listen(sock, 1);
    if (Lcheck == -1) {
        printf("Error in listen().\n");
    }

    //The first node is a Demo node.
    TN *headNodePart1;
    TN *headNodePart2;

    char MsgBuffer[FILE_SIZE / 2] = {'0'};

    memset(&senderAddress, 0, sizeof(senderAddress));
    unsigned int senderAddressLen = sizeof(senderAddress);
    int senderSock = accept(sock, (struct sockaddr *) &senderAddress, &senderAddressLen);
    if (senderSock == -1) {
        printf("accept() failed.\n");
        close(sock);
        return -1;
    }
    while(1){
        //Receive the first part + measure the time of first part:
        struct timeval start , end;
//        clock_t start , end;
//        start = clock();
        int got = 0;
        int isTimeStarted = 0;
        int bytes = 0;
        while (got < FILE_SIZE/2){
            bytes = (int)(recv(senderSock, MsgBuffer,CHANK, 0));
            if(! isTimeStarted){
                gettimeofday(&start,NULL);
                isTimeStarted = 1;
            }
            if(bytes == -1){
                printf("Error in recv().\n");
                return -1;
            }
  
            if(!(strcmp(EXIT, MsgBuffer))){ 
                printf("Exit message received: '%s'\n\n", MsgBuffer);
                break;
            }
             got += bytes;
        }
        isTimeStarted = 0;
        gettimeofday(&end,NULL);
        if(!(strcmp(EXIT, MsgBuffer))){
            break;
        }
        double sec = (double )(end.tv_sec - start.tv_sec);
        double usec =(double )(end.tv_usec - start.tv_usec);
        double measureTime = sec + (usec / 1000000);
        //Save the time:
        TN *node1;
        node1 = createNewNode(measureTime);
        node1->next = headNodePart1;
        headNodePart1 = node1;

        //********************Send back authentication:**********************

        int x = (7351^4015);
        if((int)(send(senderSock, &x, sizeof(x) - 1, 0)) == -1){
           printf("Error in send().\n");
        }

        //The sender receive the authentication and check if it's right.

        //Change CC Algorithm:
        if(setsockopt(senderSock,IPPROTO_TCP,TCP_CONGESTION,"cubic", 5) == -1) {
            printf("setsockopt() failed.\n");
        }
        got = 0;

        //Receive the second part + measure the time of second part.
        while (got < FILE_SIZE / 2){
            bytes = (int)(recv(senderSock, MsgBuffer,CHANK , 0));
            if(! isTimeStarted){
                isTimeStarted = 1;
                gettimeofday(&start,NULL);
            }
            if(bytes == -1){
                printf("Error in recv().\n");
                return -1;
            }

            if(!(strcmp(EXIT, MsgBuffer))){ 
                printf("Exit message received: '%s'\n\n", MsgBuffer);
                break;
            }
            got += bytes;
        }
        gettimeofday(&end,NULL);
        if(!(strcmp(EXIT, MsgBuffer))){
            break;
        }
        //Saving the time.
         sec = (double )(end.tv_sec - start.tv_sec);
         usec =(double )(end.tv_usec - start.tv_usec);
         measureTime = sec + (usec / 1000000);

        TN *node2;
        node2 = createNewNode(measureTime);
        node2->next = headNodePart2;
        headNodePart2 = node2;

        //If get exit message:
            //Print times.
            //Print avrg time of first part.
            //Print avrg time of second part.

    }
    double *index_sum1;
    double *index_sum2;
    double index1;
    double index2;
    double sum1 = 0;
    double sum2 = 0;
    
    headNodePart1 = reverse(headNodePart1);
    index_sum1 = printPacketTime(headNodePart1, index1, sum1, 0, 1);
    index1 = index_sum1[0];
    sum1 = index_sum1[1];

    printf("\n");

    headNodePart2 = reverse(headNodePart2);
    index_sum2 = printPacketTime(headNodePart2, index2, sum2, 0, 2);
    index2 = index_sum2[0];
    sum2 = index_sum2[1];

    printf("\n");

    double avg = (sum1 / index1);
    printf("The average of first part 1 is: %f\n" , avg);
    
    avg = (sum2 / index2);
    printf("The average of second part 2 is: %f\n" , avg);
    
    return 0;
}