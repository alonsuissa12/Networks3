#include "Receiver.h"

//Struct node that will hold the time took to receive the file and the next node.
struct timeNode{
    double time;
    struct timeNode *next;
};
typedef struct timeNode TN;
//Constructor of timeNode(TN).
TN *createNewNode(double time){
    TN *result = malloc(sizeof(TN));
    result->time = time;
    result->next = NULL;
    return result;
}
//Function which reverse the list 'head'.
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
//Function to print out the times which stashed in the list 'head',and return the length of the list and sum of the times.
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

int main() {
    //initializing a TCP socket.
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in senderAddress;
    //setting to zero the struct senderAddress
    memset(&senderAddress, 0, sizeof(senderAddress));
    senderAddress.sin_family = AF_INET;
    senderAddress.sin_port = htons(port);
    int checkP = inet_pton(AF_INET, (const char *) IP, &senderAddress.sin_addr);
    if (checkP <= 0) {
        printf("inet_pton() failed.\n");
    }
    //opening the socket.
    int Bcheck = bind(sock, (struct sockaddr *) &senderAddress, sizeof(senderAddress));
    if (Bcheck == -1) {
        printf("Error while binding.\n");
    }
    //start listening on the socket (one client at the time)
    int Lcheck = listen(sock, 1);
    if (Lcheck == -1) {
        printf("Error in listen().\n");
    }

    //First node to start the list (demo node). we hold two lists on for each part.
    TN *headNodePart1;
    TN *headNodePart2;
    //Buffer to receive the data from the packets into.
    char MsgBuffer[FILE_SIZE / 2] = {'0'};
    memset(&senderAddress, 0, sizeof(senderAddress));
    unsigned int senderAddressLen = sizeof(senderAddress);
    //accepting the client (the Sender)
    int senderSock = accept(sock, (struct sockaddr *) &senderAddress, &senderAddressLen);
    if (senderSock == -1) {
        printf("accept() failed.\n");
        close(sock);
        return -1;
    }
    //we will stay in the loop and keep receiving massages until we get an exit massage.
    while(1){
        //Receive the first part + measure the time of first part:
        struct timeval start , end;
        int got = 0;
        int isTimeStarted = 0;
        int bytes = 0;
        //Change CC Algorithm.
        if(setsockopt(senderSock,IPPROTO_TCP,TCP_CONGESTION,"reno", 5) == -1) {
            printf("setsockopt() failed.\n");
        }
        while (got < FILE_SIZE/2){
            bytes = (int)(recv(senderSock, MsgBuffer,CHUNK, 0));
            //starts measuring the time on the first time.
            if(! isTimeStarted){
                gettimeofday(&start,NULL);
                isTimeStarted = 1;
            }
            if(bytes == -1){
                printf("Error in recv().\n");
                return -1;
            }
            //if got an exit message.
            if(!(strcmp(EXIT, MsgBuffer))){ 
                printf("Exit message received: '%s'\n\n", MsgBuffer);
                break;
            }
             got += bytes;
        }
        isTimeStarted = 0;
        //if got an exit massage.
        gettimeofday(&end,NULL);
        if(!(strcmp(EXIT, MsgBuffer))){
            break;
        }
        //converting the measure time to seconds format (double).
        double sec = (double )(end.tv_sec - start.tv_sec);
        double usec =(double )(end.tv_usec - start.tv_usec);
        double measureTime = sec + (usec / 1000000);
        //Saving the time.
        TN *node1;
        node1 = createNewNode(measureTime);
        node1->next = headNodePart1;
        headNodePart1 = node1;
        //Sending  authentication.
        int x = (7351^4015);
        if((int)(send(senderSock, &x, sizeof(x) - 1, 0)) == -1){
           printf("Error in send().\n");
        }
        //Change CC Algorithm.
        if(setsockopt(senderSock,IPPROTO_TCP,TCP_CONGESTION,"cubic", 5) == -1) {
            printf("setsockopt() failed.\n");
        }
        //Receive the second part + measure the time of second part.
        got = 0;
        while (got < FILE_SIZE / 2){
            bytes = (int)(recv(senderSock, MsgBuffer,CHUNK , 0));
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
        //converting the measure time to seconds format (double).
         sec = (double )(end.tv_sec - start.tv_sec);
         usec =(double )(end.tv_usec - start.tv_usec);
         measureTime = sec + (usec / 1000000);
        //Saving the time.
        TN *node2;
        node2 = createNewNode(measureTime);
        node2->next = headNodePart2;
        headNodePart2 = node2;
    }
    //this part will start only after getting Exit message.
    double *index_sum1;
    double *index_sum2;
    double index1;
    double index2;
    double sum1 = 0;
    double sum2 = 0;
    //Reversing the list because we want the real order that  the packet arrive(we added to the head of the list).
    headNodePart1 = reverse(headNodePart1);
    //Printing th times of the first part.
    //'index_sum1' will point to array the holds the sum of the times and the length of the list
    printf("times of first part(sent with reno algorithm):\n");
    index_sum1 = printPacketTime(headNodePart1, index1, sum1, 0, 1);
    index1 = index_sum1[0];
    sum1 = index_sum1[1];

    printf("\n");

    //Reversing the list because we want the real order that  the packet arrive(we added to the head of the list).
    headNodePart2 = reverse(headNodePart2);
    //Printing th times of the first part.
    //'index_sum2' will point to array the holds the sum of the times and the length of the list
    printf("times of second part(sent with cubic algorithm):\n");
    index_sum2 = printPacketTime(headNodePart2, index2, sum2, 0, 2);
    index2 = index_sum2[0];
    sum2 = index_sum2[1];

    printf("\n");
    //Printing out the average time of the first part.
    double avg = (sum1 / index1);
    printf("The average of the first part (sent with reno CC algorithm) is: %f\n" , avg);
    //Printing out the average time of the second part.
    avg = (sum2 / index2);
    printf("The average of the second part (sent with cubic CC algorithm) is: %f\n" , avg);
    
    return 0;
}