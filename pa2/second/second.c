#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//defining node//
typedef struct node {
    char vertex[21];
    int weight;
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
node* nodeAllocate(char input[], int inputWeight){
    struct node* temp = malloc(sizeof(struct node));
    strcpy(temp->vertex, input);
    temp->next=0;
    temp->weight = inputWeight;
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
    //allocate a node for our adj vertex//
    node* temp = nodeAllocate(adj, weight);
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
//counts the number of out-degrees a vertex has//
int outDegree(graph* finalGraph, char querieSource[]){
    int i = 0;
    int count = 0;
    //traversing through the adjList until we reach the vertex we want to find the degree of//
    while(strcmp(finalGraph->adjList[i].vertexName, querieSource) != 0){
        i++;
    }
    //traverse and increase count until you reach the end//
    for(node* ptr =finalGraph->adjList[i].head; ptr; ptr = ptr->next){
        count++;
    }
    return count;
}

//counts the number of in-degrees a vertex has//
int inDegree(graph* finalGraph, char querieSource[]){
    int count = 0;
    int num = finalGraph->numVertices;
    //goes through list//
    for(int i = 0; i < num; i++){
        //goes through LL//
        for(node* ptr = finalGraph->adjList[i].head; ptr; ptr = ptr->next){
            if(strcmp(ptr->vertex, querieSource) == 0){
                count++;
            }
        }
    }
    return count;
}

//prints the adjacent verticies from a source vertex//
void adjacentVertices(graph* finalGraph, char querieSource[]){
    int i = 0;
    //traversing through the adjList until we reach the vertex we want to find the degree of//
    while(strcmp(finalGraph->adjList[i].vertexName, querieSource) != 0){
        i++;
    }
    //traverse and print out each vertex//
    for(node* ptr =finalGraph->adjList[i].head; ptr; ptr = ptr->next){
        printf("%s", ptr->vertex);
        printf(" ");
    }
    printf("\n");
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

int main (int argc, char* argv[argc +1]){
    FILE* fp = fopen(argv[1], "r");
    graph* finalGraph = makeGraph(fp);
    char source[21];
    char destination[21];
    int weight = 0;

    while(fscanf(fp, "%s %s %d\n", source, destination, &weight) != EOF){
        inputEdge(finalGraph, source, destination, weight);
    }

    char querie;
    char querieSource[21];
    int outDegreeCount;
    int inDegreeCount;
    FILE* fp2 = fopen(argv[2], "r");
    while(fscanf(fp2, "%c %s\n", &querie, querieSource)!= EOF){
        if(querie == 'o'){
            outDegreeCount = outDegree(finalGraph, querieSource);
            printf("%d\n", outDegreeCount);
        }
        if(querie == 'i'){
            inDegreeCount = inDegree(finalGraph, querieSource);
            printf("%d\n", inDegreeCount);
        }
        if(querie == 'a'){
            adjacentVertices(finalGraph, querieSource);
        }
    }
    fclose(fp);
    fclose(fp2);
    freeMemory(finalGraph);
    return EXIT_SUCCESS;
}