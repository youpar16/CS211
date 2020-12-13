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
    int visited;
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
    temp->visited = 0;
    return temp;
}

//push method taken from my pa1//
node* push(struct node* head, int input){
    char hold[3] = "A";
    struct node* insertNode = nodeAllocate(hold, 0, input);
    //if the LL is empty//
    if(head == 0){
        //just return the node//
        return insertNode;
    }else{
        //put inserNode to the front//
        insertNode->next = head;
        head = insertNode;
    }
    return head;
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

//conducts topological sort on the graph, and input our ordering into an array//
node* topSort(graph* finalGraph, int* visitedCheck, int index, node* head){
    //mark the vertex as visited//
    visitedCheck[index] = 1;

    //going through the adjacent vertices of our input//
    for(node* ptr = finalGraph->adjList[index].head; ptr; ptr = ptr->next){
        //if it is not visited//
        if(visitedCheck[ptr->vertexIndex] == 0){
            //recursively call DFS on that adjacent vertex//
            head = topSort(finalGraph, visitedCheck, ptr->vertexIndex, head);
        }
    }
    //before returning out of recursive loop push our index into our stack//
    head = push(head, index);
    return head;
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

void shortestPath(graph* finalGraph, char sourceVertex[], int* distance, int* visitedCheck){
    int vertexCount = 0;
    //traversing through our list so we can find the vertexIndex of our sourceVertex//
    while(strcmp(finalGraph->adjList[vertexCount].vertexName, sourceVertex) != 0){
        vertexCount++;
    }
    //initializing all of the distance to infinity//
    for(int i = 0; i < finalGraph->numVertices; i++){
            distance[i] = INT_MAX;
    }

    //set the distance from of the source vertex to 0//
    distance[vertexCount] = 0;
    node*head = 0;
    //making a topological sort of our graph FROM our source vertex//
    head = topSort(finalGraph, visitedCheck, vertexCount, head);
   
    //implementing the shortestPath pseudocode//
    for(node*ptr = head; ptr; ptr = ptr->next){
        for(node* pt2 = finalGraph->adjList[ptr->vertexIndex].head; pt2; pt2 = pt2->next){
            if(distance[pt2->vertexIndex] > distance[ptr->vertexIndex] + pt2->weight){
                distance[pt2->vertexIndex] = distance[ptr->vertexIndex] + pt2->weight;
            }
        }
    }
   
   //free memory of the topologicalsort//
   freeMemoryLL(head);
}

void printShort(graph* finalGraph, int* distance){
    printf("\n");
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
}

// int cycleCheck(graph* finalGraph, int* cycleVisit, int vertexIndex){
//     cycleVisit[vertexIndex] = 1;
//     int indexKeep;
//     for(node*ptr = finalGraph->adjList[vertexIndex].head; ptr; ptr = ptr->next){
//         indexKeep = ptr->vertexIndex;
//         if(cycleVisit[ptr->vertexIndex] == 2){
//             continue;
//         }
//         if(cycleVisit[ptr->vertexIndex] == 1){
//             return 1;
//         }
//         if(cycleCheck(finalGraph, cycleVisit, ptr->vertexIndex)){
//             return 1;
//         }
//     }
//     cycleVisit[indexKeep] = 2;
//     return 0;
// }

int main (int argc, char* argv[argc +1]){
    FILE* fp = fopen(argv[1], "r");
    graph* finalGraph = makeGraph(fp);
    char source[21];
    char destination[21];
    int weight = 0;

    //goes through the first txt file to assemble our graph//
    while(fscanf(fp, "%s %s %d\n", source, destination, &weight) != EOF){
        inputEdge(finalGraph, source, destination, weight);
    }

    // int* cycleVisit = malloc(finalGraph->numVertices*sizeof(int*));
    // for(int i = 0; i < finalGraph->numVertices; i++){
    //     cycleVisit[i] = 0;
    // }
    // int cycle = 2;
    // for(int i = 0; i < finalGraph->numVertices; i++){
    //     if(cycleVisit[i] == 0){
    //         cycle = cycleCheck(finalGraph, cycleVisit, i);
    //     }
    // }
    // if(cycle == 1){
    //     printf("\nCYCLE");
    // }

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
        //resets our vertices visit array//
        for(int i = 0; i < finalGraph->numVertices; i++){
            visitedCheck[i] = 0;
        }
        //performs shortest path method on our source Vertex//
        shortestPath(finalGraph, sourceVertex, distance, visitedCheck);
        //prints it//
        printShort(finalGraph, distance);
    }
 
    //appropriately closes all file pointers and frees all memory//
    printf("\n");
    fclose(fp);
    fclose(fp2);
    // free(cycleVisit);
    free(visitedCheck);
    free(distance);
    freeMemory(finalGraph);
    return EXIT_SUCCESS;
}