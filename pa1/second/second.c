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

void freeMemory(struct node* head){
    struct node* ptr = head;
    while(head != 0){
        ptr = head;
        head = head->next;
        free(ptr);
    }
}

//insert node in ascending order//
struct node* insert(struct node* head, int input){
    //allocate a node for the inserted value//
    struct node* insertNode =  allocate(input);
    //if the input linked list is empty, just return the new node that we made//
    if(head == 0){
        return insertNode;
    }
    //if the input value is smaller than our current head//
    if(input < head->data){
        //designate our insertNode->next to our current head and return our insertNode//
        insertNode->next = head;
        return insertNode;
    }else{
        //set a ptr to head//
        struct node* ptr = head;
        //set a lag ptr also to head//
        struct node* lag = head;
        //while our ptr is not at the end of our list, and our ptr-> data is smaller than our input value//
        while(ptr->next != 0 && ptr->data < input){
            //traverse down the linked list//
            lag = ptr;
            ptr = ptr->next;
        }
        //if our current ptr->data is the same as our input//
        if(input == ptr->data){
            free(insertNode);
            //just return the original linked list//
            return head;
        }
        //to input at the end of the LL//
        if(input > ptr->data){
            ptr->next = insertNode;
            return head;
        }
        //otherwise input our new node
        insertNode->next = lag->next;
        lag->next = insertNode;
        return head;
    }
}

//delete a node ignore if node doesnt exist//
struct node* delete(struct node* head, int input){
    //if input LL doesnt exist just return it//
    if(head == 0){
        return head;
    }
    struct node* ptr = head;
    //if the input value is less than the head, it doesn't exist return head//
    if(input < head->data){
        return head;
    }
    //if head is the input value and it is the only node, just return null//
    if(input == head->data && head->next == 0){
        free(head);
        return 0;
    }
    //if head is the input value and there are nodes following it//
    if(input == head->data && head->next != 0){
        //set ptr to head->next and return ptr//
        head = head->next;
        //free memory of the head node//
        free(ptr);
        return head;
    }
    struct node* lag = head;
    //while ptr is less than input
    while(ptr->data < input){
        //traverse through the LL with a ptr and a lag //
        lag = ptr;
        //if ptr->next exists, traverse
        if(ptr->next){
            ptr = ptr->next;
        //if at any point there is no ptr->next, we reached the end of the LL and the node we must delete DNE, therefore return the LL//    
        }else{
            return head;
        }
    }
    //when ptr reaches the node that needs to be deleted//
    if(ptr->data == input){
        //if it is not the last node//
        if(ptr->next != 0){
            //set lag->next to ptr->next which unlinks ptr from the LL//
            lag->next = ptr->next;
            ptr->next = 0;
        //if it is the last node//
        }else{
            lag->next = 0;
        }
    }
    //if ptr data is greater than the input by the end of the while loop, number does not exist return head //
    if(ptr-> data > input){
        return head;
    }
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
    int num;
    char operation[7];
    if(fp == 0){
        printf("error\n");
        return EXIT_SUCCESS;
    }

    while(fscanf(fp, "%s %d\n", operation, &num) != EOF){
        if(operation[0] == 'I'){
            head = insert(head,num);
            printLL(head);
            printf("\n");
        }
        if(operation[0] == 'D'){
            head = delete(head,num);
            printLL(head);
            printf("\n");
        }
    }
    fclose(fp);
    freeMemory(head);

    return EXIT_SUCCESS;
}