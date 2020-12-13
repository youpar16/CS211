#include<stdio.h>
#include<stdlib.h>

//defining node//
struct node {
    int data;
    struct node * next;
};

//allocating a node with the input value (went over in class)//
struct node* allocate(int val){
    struct node* temp = malloc(sizeof(struct node));
    temp->data=val;
    temp->next=0;
    return temp;
}

void freeMemory(struct node* head){
    struct node* ptr = head;
    while(head != 0){
        ptr = head;
        head = head->next;
        free(ptr);
    }
}

//printing the LL//
void printLL(struct node* head){
    //if LL is empty, print "EMPTY"//
    if(head == 0){
        printf("EMPTY");
        free(head);
    }
    //traversing through the linked list//
    while(head != 0){
        //if the node is not the last node print the value along with a space//
        if(head->next != 0){
            printf("%d", head->data);
            printf(" ");
        //if the node is the last node
        }else{
            //just print the node no space necessary//
            printf("%d", head->data);
        }
        head = head->next;
    } 
}

struct node* enqueue(struct node* head, int input){
    struct node* insertNode = allocate(input);
    //if the LL is empty//
    if(head == 0){
        //just return the node//
        return insertNode;
    }
    //create a ptr to traverse down the list until it reaches the end//
    struct node* ptr = head;
    while(ptr->next != 0){
        ptr = ptr->next;
    }
    //insert the insertNode at the end//
    ptr->next = insertNode;
    return head;
}

struct node* push(struct node* head, int input){
    struct node* insertNode = allocate(input);
    //if the LL is empty//
    if(head == 0){
        //just return the node//
        return insertNode;
    }
    //put inserNode to the front//
    insertNode->next = head;
    return insertNode;
}

struct node* pop(struct node* head){
    if(head == 0){
        return head;
    }
    //set ptr to head//
    struct node* ptr = head;
    //move head to the next node//
    head = head->next;
    //free ptr memory//
    free(ptr);
    return head;
}

int main (int argc, char* argv[argc +1]){
    if(argc < 2){
        printf("insufficient arguments\n");
        return EXIT_SUCCESS;
    }
    struct node* head = 0;
    FILE* fp = fopen(argv[1], "r");
    char operation[9];
    int num;
    if(fp == 0){
        printf("error\n");
    }
    while(fscanf(fp, "%s %d\n", operation, &num) != EOF){
        if(operation[1] == 'N'){
            head = enqueue(head,num);
            printLL(head);
            printf("\n");
        }
        if(operation[1] == 'U'){
            head = push(head,num);
            printLL(head);
            printf("\n");
        }
        if(operation[1] == 'O'){
            head = pop(head);
            printLL(head);
            printf("\n");
        }
    }
    fclose(fp);
    freeMemory(head);

    return EXIT_SUCCESS;
}