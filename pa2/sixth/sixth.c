#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

//defining node has weight as it is a weighted graph and a vertexIndex to use in our DFS//
typedef struct node {
    char vertex[21];
    int weight;
    int vertexIndex;
    struct node * next;
}node;

//defining our list that will be pointing to our linked list//
typedef struct list{
    char vertexName[21];
    node* head;
}list;

//defining our graph//
typedef struct graph{
    int numVertices;
    list* adjList;
}graph;

//allocating a node with the input value (went over in class)//
node* nodeAllocate(char input[], int inputWeight, int vertexCount){
    struct node* temp = malloc(sizeof(struct node));
    strcpy(temp->vertex, input);
    temp->next=0;
    temp->weight = inputWeight;
    temp->vertexIndex = vertexCount;
    return temp;
}

//allocating a node with the input value (went over in class)//
node* nodeWeightAllocate(int inputWeight, int vertexCount){
    struct node* temp = malloc(sizeof(struct node));
    temp->next=0;
    temp->weight = inputWeight;
    temp->vertexIndex = vertexCount;
    return temp;
}

//making a Graph//
graph* makeGraph(FILE* fp){
    //grabs the number of vertices in file
    int vertices;
    fscanf(fp, "%d\n", &vertices);
    //allocates memory for our graph structure//
    graph* undGraph = malloc(sizeof(graph));
    //assigns the number of verticies//
    undGraph->numVertices = vertices;
    //allocates memory for our array//
    undGraph->adjList= malloc(vertices*sizeof(list));

    //inputting our vertices//
    for(int i = 0; i < vertices; i++){
        //initially points all of our vertices to null//
        undGraph->adjList[i].head = 0;
        //inputs the name of our vertices//
        fscanf(fp, "%s\n", undGraph->adjList[i].vertexName); 
    }

    char vertexHold[21];

    //order graph//
    for(int i =0; i < undGraph->numVertices; i++){
        for(int j = i + 1; j < undGraph->numVertices; j++){
            if(strcmp(undGraph->adjList[i].vertexName, undGraph->adjList[j].vertexName) > 0){
                strcpy(vertexHold,undGraph->adjList[i].vertexName);
                strcpy(undGraph->adjList[i].vertexName, undGraph->adjList[j].vertexName);
                strcpy(undGraph->adjList[j].vertexName, vertexHold);
            }
        }
    }
    
    return undGraph;
}

//input edge//
void inputEdge(graph* undGraph, char source[], char adj[], int weight){
    int i = 0;
    int vertexCount = 0;

    //traversing through our list so we can give our node the vertexIndex//
    while(strcmp(undGraph->adjList[vertexCount].vertexName, adj) != 0){
        vertexCount++;
    }

    //allocate a node for our adj vertex//
    node* temp = nodeAllocate(adj, weight, vertexCount);

    //traversing through our list until we reach the source vertex//
    while(strcmp(undGraph->adjList[i].vertexName, source) != 0){
        i++;
    }

    //if there is no head initialized already//
    if(undGraph->adjList[i].head == 0){
        // temp->next = undGraph->adjList[i].head;
        undGraph->adjList[i].head = temp;
    //if head is initialized//
    }else{
        //assign 2 ptrs to the head//
        node* ptr = undGraph->adjList[i].head;
        node* lag = undGraph->adjList[i].head;
        //if nothing is connected to the head//
        if(ptr->next == 0){
            //and the head is smaller than our input//
            if(strcmp(ptr->vertex,temp->vertex)<0){
                //attach our input node after our head//
                ptr->next = temp;
            //if the head is bigger than our input//
            }else{
                //point our input node to head//
                temp->next = ptr;
                //reassign head//
                undGraph->adjList[i].head = temp;
            }
        //if something is connected to the head//
        }else{
            //traverse down the LL until our input node is smaller than out ptr node//
            while(ptr->next != 0 && strcmp(ptr->vertex,temp->vertex)<0){
                lag = ptr;
                ptr = ptr->next;
            }
            //if we reached the end of the LL//
            if(ptr->next == 0){
                //if our ptr is less than our input vertex
                if(strcmp(ptr->vertex,temp->vertex)<0){
                    //add out input node to the end//
                    ptr->next = temp;
                }else{
                    temp->next = ptr;
                    lag->next = temp;
                }
            //if we are not at the end of the LL//
            }else{
                //and if ptr did not move from our head (signaling our input node should be our new head)//
                if(ptr == undGraph->adjList[i].head){
                    //point our input node to head//
                    temp->next = ptr;
                    //reassign head//
                    undGraph->adjList[i].head = temp;
                //if ptr did move//
                }else{
                    //put our input node between lag and ptr//
                    temp->next = ptr;
                    lag->next = temp;
                }
            }
        }
    }
}

//frees memory from a graph//
void freeMemory(graph* finalGraph){
    int num = finalGraph->numVertices;
    //goes through list//
    for(int i = 0; i < num; i++){
        //goes through LL//
        while(finalGraph->adjList[i].head){
            node* ptr = finalGraph->adjList[i].head;
            node* lag = finalGraph->adjList[i].head;
            lag = ptr;
            ptr = ptr->next;
            finalGraph->adjList[i].head = ptr;
            free(lag);
        }
    }

    free(finalGraph->adjList);
    free(finalGraph);
}

//a modified version of the insert method from my pa1/second that orders nodes by their WEIGHT//
//insert node in ascending order//
node* insert(node* head, int inputWeight, int vertexIndex){
    //allocate a node for the inserted value//
    node* insertNode =  nodeWeightAllocate(inputWeight, vertexIndex);
    //if the input linked list is empty, just return the new node that we made//
    if(head == 0){
        return insertNode;
    }
    //if the input value is smaller than our current head//
    if(inputWeight < head->weight){
        //designate our insertNode->next to our current head and return our insertNode//
        insertNode->next = head;
        return insertNode;
    }else{
        //set a ptr to head//
        struct node* ptr = head;
        //set a lag ptr also to head//
        struct node* lag = head;
        //while our ptr is not at the end of our list, and our ptr-> data is smaller than our input value//
        while(ptr->next != 0 && ptr->weight < inputWeight){
            //traverse down the linked list//
            lag = ptr;
            ptr = ptr->next;
        }
        //if our current ptr->data is the same as our input or greater than//
        if(inputWeight == ptr->weight || inputWeight > ptr->weight){
            if(ptr->next == 0){
                ptr->next = insertNode;
                return head;
            }
            insertNode->next = lag->next;
            lag->next = insertNode;
            return head;
        }
        //otherwise input our new node
        insertNode->next = lag->next;
        lag->next = insertNode;
        return head;
    }
}

//method to free memory from a given linked list//
void freeMemoryLL(struct node* head){
    struct node* ptr = head;
    while(head != 0){
        ptr = head;
        head = head->next;
        free(ptr);
    }
}

//pop method taken from my pa1/third//
node* pop(node* head){
    if(head == 0){
        return head;
    }
    //set ptr to head//
    node* ptr = head;
    //move head to the next node//
    head = head->next;
    //free ptr memory//
    free(ptr);
    return head;
}

//fix the distance in our LL//
node* fix(node*head, int vertexIndex, int newWeight){
    node* ptr = head;
    node* lag = head;
    //if the distance that is needed to be fixed is in the front of the LL//
    if(head->vertexIndex == vertexIndex){
        //get rid of the head//
        head = head->next;
        //free it//
        free(ptr);
    //if it is not in the front//
    }else{
        //traverse until we reach it//
        while(ptr->vertexIndex != vertexIndex){
            lag = ptr;
            ptr = ptr->next;
        }
        //if the distance that is needed to be fixed is somewhere in the middle//
        if(ptr->next){
            //get rid of it//
            lag->next = ptr->next;
            //free it//
            free(ptr);
        //if its at the end of the LL//
        }else{
            //get rid of the tail//
            lag->next = 0;
            //free it//
            free(ptr);
        }
    }
    //insert it entirely as a new node with the updated weight//
    head = insert(head, newWeight, vertexIndex);
    return head;
}

//method to find the shortest path from source to all connected vertices through dijkstras//
void shortestPath(graph* finalGraph, char sourceVertex[], int* distance){
    int vertexCount = 0;
    node* head = 0;
    //traversing through our list so we can find the vertexIndex of our sourceVertex//
    while(strcmp(finalGraph->adjList[vertexCount].vertexName, sourceVertex) != 0){
        vertexCount++;
    }
    //initializing all of the distance to infinity//
    for(int i = 0; i < finalGraph->numVertices; i++){
            distance[i] = INT_MAX;
    }

    // int visited[100];
    // for(int i = 0; i < 100; i++){
    //     visited[i] = 0;
    // }
    //set the distance from of the source vertex to 0//
    distance[vertexCount] = 0;
    //initialize the head as the source vertex//
    head = insert(head, distance[vertexCount], vertexCount);

    //while our priority queue still exists//
    while(head){
        //set a variable to the vertex index of the adjacent vertex with the smallest distance (would just be the head as we ordered our LL by weight)//
        int vertexIndex1 = head->vertexIndex;
        //pop out the head//
        head = pop(head);
        //for all of the neighbors of of the vertex we just popped//
        for(node*ptr = finalGraph->adjList[vertexIndex1].head; ptr; ptr = ptr->next){
            //if the distance of it is still INFINITY (meaning we have not visited it yet)//
            if(distance[ptr->vertexIndex] == INT_MAX){
                //set the distance as the distance of where it came from PLUS its weight//
                distance[ptr->vertexIndex] = distance[vertexIndex1] + ptr->weight;
                // visited[ptr->vertexIndex] = vertexIndex1;
                //insert this vertex into our LL//
                head = insert(head, distance[ptr->vertexIndex], ptr->vertexIndex);
            //if it had been visited already//
            }else{
                //if the distance of the previous vertex PLUS the weight is LESS than its current distance//
                if(distance[ptr->vertexIndex] > distance[vertexIndex1] + ptr->weight){
                    //update it//
                    distance[ptr->vertexIndex] = distance[vertexIndex1] + ptr->weight;
                    //fix its weight in the LL//
                    head = fix(head, ptr->vertexIndex, distance[ptr->vertexIndex]);
                }
            }
        }
    }
    freeMemoryLL(head);
}

//method to print out our shortest path results//
void printShort(graph* finalGraph, int* distance){
    for(int i = 0; i < finalGraph->numVertices; i++){
        printf("%s", finalGraph->adjList[i].vertexName);
        printf(" ");
        if(distance[i] == INT_MAX || distance[i] == -(INT_MAX)){
            printf("INF");
        }else{
            printf("%d", distance[i]);
        }
        printf("\n");
    }
    printf("\n");
}

int main (int argc, char* argv[argc +1]){
    FILE* fp = fopen(argv[1], "r");
    graph* finalGraph = makeGraph(fp);
    char source[21];
    char destination[21];
    int weight = 0;

    while(fscanf(fp, "%s %s %d\n", source, destination, &weight) != EOF){
        inputEdge(finalGraph, source, destination, weight);
    }

    //initializes that none of the vertices were visited//
    int* visitedCheck = malloc(finalGraph->numVertices*sizeof(int*));
    for(int i = 0; i < finalGraph->numVertices; i++){
        visitedCheck[i] = 0;
    }
    //where we will store our sourceVertex name//
    char sourceVertex[22];
    //allocating memory for the array where we will store our shortest path distances//
    int* distance = malloc(finalGraph->numVertices*sizeof(int*));
    FILE* fp2 = fopen(argv[2], "r");

    while(fscanf(fp2, "%s\n", sourceVertex)!= EOF){
        shortestPath(finalGraph, sourceVertex, distance);
        printShort(finalGraph, distance);
    }

    //appropriately closes all file pointers and frees all memory//
    fclose(fp);
    fclose(fp2);
    free(visitedCheck);
    free(distance);
    freeMemory(finalGraph);
    return EXIT_SUCCESS;
}