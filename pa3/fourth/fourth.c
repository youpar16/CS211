#include<stdio.h>
#include<stdlib.h>
#include<math.h>

//defining node//
typedef struct node {
    long num;
    int decimal;
    struct node * next;
}node;

//allocating a node with the input value (went over in class)//
node* nodeAllocate(int input, int decimal){
    struct node* temp = malloc(sizeof(struct node));
    temp->num = input;
    temp->decimal = decimal;
    temp->next=0;
    return temp;
}

//add node to the end(from pa1)//
struct node* enqueue(struct node* head, long input, int decimal){
    struct node* insertNode = nodeAllocate(input, decimal);
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

//add node to the front(from pa1)//
struct node* push(struct node* head, long input, int decimal){
    struct node* insertNode = nodeAllocate(input, decimal);
    //if the LL is empty//
    if(head == 0){
        //just return the node//
        return insertNode;
    }
    //put inserNode to the front//
    insertNode->next = head;
    return insertNode;
}

//converting the fraction number to binary//
node* toBinary(double num, node* head){
    double integer, fraction;
    fraction = modf(num, &integer);
    //add in a decimal to the LL//
    head = push(head, 0, 1);

    //adding in the integer part of the fraction//
    int input = 0;
    int x = integer;
    while(x > 0){
        //set input as the remainder when divided by 2//
        input = fmod(x, 2);
        //set num as num divided by 2//
        x = x / 2;
        //push into our LL//
        head = push(head, input, 0);
    }
    //adding in the fraction part of the fraction//
    double fInput = fraction;
    while(fInput != 0){
        //multiply by 2//
        fInput = fInput * 2;
        //if its less than 1//
        if(fInput < 1){
            //put in 0 at the end of the LL//
            head = enqueue(head, 0, 0);
        //if its greater than 1 or is 1//
        }else{
            //subtract 1//
            fInput = fInput - 1;
            //add in 1 at the end of the LL//
            head = enqueue(head, 1, 0);
        }
    }
    return head;
}

//determining the Caonical representation given our binary fraction representation//
void toCaonical(node* head, int bits){
    //determining the value of E//
    int E = 0;
    //if the head of the LL is a decimal//
    if(head->decimal == 1){
        //traverse through the LL//
        for(node* ptr = head->next; ptr; ptr = ptr->next){
            //if its a 0, keep traversing while decrementing E//
            if(ptr->num != 1){
                E--;
            //once we get to the first 1//
            }else{
                //decrement once more and stop the for loop//
                E--;
                break;
            }
        }
    //if the head of the LL is not a decimal//
    }else{
        //traverse through the LL//
        for(node* ptr = head; ptr; ptr = ptr->next){
            //if it is not a decimal add 1 to our E count//
            if(ptr->decimal == 0){
                E++;
            //if it is a decimal subtract 1 from our E count and break out of the loop//
            }else{
                E--;
                break;
            }
        }
    }
    //because we know our M is between 1 and 2, print one and a decimal "1."//
    int one = 1;
    printf("%d", one);
    printf(".");
    //keep a count of the # of bits(decimals) we want our M value to have//
    int count = bits;
    int zero = 0;
    node*ptr = head;
    //if E was negative//
    if(E < 0){
        //traverse through the LL until we get to the decimal//
        for(int i = E; i < 0; i++){
            ptr = ptr->next;
        }
        //while our count is over 0//
        while(count > 0){
            //if there is a next in our LL//
            if(ptr->next){
                //print that binary digit and traverse//
                printf("%ld", ptr->next->num);
                ptr = ptr->next;
            //if it doesnt exist just print zeros//
            }else{
                printf("%d", zero);
            }
            //decrement count//
            count--;
        }
    //if our E is 0//
    }else if(E == 0){
        //get our LL ptr to the decimal//
        ptr = ptr->next;
        //while count is above 0//
        while(count > 0){
            //if there is a next in our LL and that happens to be a decimal//
            if(ptr->next && ptr->next->decimal ==1){
                //skip past it//
                ptr = ptr->next;
            }
            //if there is a next//
            if(ptr->next){
                //print that binary digit and traverse//
                printf("%ld", ptr->next->num);
                ptr = ptr->next;
            //if it doesnt exist just print zeros//
            }else{
                printf("%d", zero);
            }
            //decrement count//
            count--;
        }
    //if our E is positive//
    }else{
        //while count is above 0//
        while(count > 0){
            //if there is a next in our LL and that happens to be a decimal//
            if(ptr->next && ptr->next->decimal ==1){
                //skip past it//
                ptr = ptr->next;
            }
            //if there is a next//
            if(ptr->next){
                //print that binary digit and traverse//
                printf("%ld", ptr->next->num);
                ptr = ptr->next;
            //if it doesnt exist just print zeros//
            }else{
                printf("%d", zero);
            }
            //decrement count//
            count--;
        }
    }
    //print a space after our M and then print our E//
    printf(" ");
    printf("%d\n", E);
}

//free our LL memory//
void freeMemory(struct node* head){
    struct node* ptr = head;
    while(head != 0){
        ptr = head;
        head = head->next;
        free(ptr);
    }
}

int main(int argc, char* argv[argc+1]){
    if(argc < 2){
        printf("insufficient arguments\n");
        return EXIT_SUCCESS;
    }
    FILE* fp = fopen(argv[1], "r");
    double num;
    int bits;
    //while pointer is not at the end of the file//
    while(fscanf(fp, "%lf %d\n", &num, &bits) != EOF){
        node* head = 0;
        head = toBinary(num, head);
        toCaonical(head, bits);
        freeMemory(head);
    }
    fclose(fp);
    return EXIT_SUCCESS;
}