#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//defining node//
typedef struct node {
    char vertex[21];
    struct node * next;
}node;

//defining our list that will be pointing to our linked list//
typedef struct list{
    char vertexName[21];
    int visit;
    node* head;
}list;

//defining our graph//
typedef struct graph{
    int numVertices;
    list* adjList;
}graph;

//allocating a node with the input value (went over in class)//
node* nodeAllocate(char input[]){
    struct node* temp = malloc(sizeof(struct node));
    strcpy(temp->vertex, input);
    temp->next=0;
    return temp;
}


//input edge//
void inputEdge(graph* undGraph, char source[], char adj[]){
    int i = 0;
    //allocate a node for our adj vertex//
    node* temp = nodeAllocate(adj);
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
    
    return undGraph;
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

struct node* enqueue(struct node* head, char input[]){
    struct node* insertNode = nodeAllocate(input);
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

//performs BFS on a source vertex//
void BFS(graph* finalGraph, char querieSource[]){
    node* head = 0;
    int numVertices = finalGraph->numVertices;

    //traversing through our list and marking them all as NOT visited//
    for(int i = 0; i < numVertices; i++){
        finalGraph->adjList[i].visit = 0;
    }

    //inputting into our LL queue the source of our BFS search and then printing it//
    head = nodeAllocate(querieSource);
    printf("%s", head->vertex);
    printf(" ");

    //marking the source vertex as VISITED//
    for(int i = 0; i < numVertices; i++){
        if(strcmp(finalGraph->adjList[i].vertexName,querieSource) == 0){
            finalGraph->adjList[i].visit = 1;
        }
    }

    //while our LL queue exists//
    while(head){
        node* hold = head;
        if(head->next){
            head = head->next;
        }else{
            head = 0;
        }
        int i = 0;
        //traversing through our list until we reach the popped vertex//
        while(strcmp(finalGraph->adjList[i].vertexName, hold->vertex) != 0){
            i++;
        }
        //traverse through the connected vertexes of our popped vertex, enqueue it if it is NOT visited, print it, and mark it as visited//
        for(node* ptr =finalGraph->adjList[i].head; ptr; ptr = ptr->next){
            int j = 0;
            while(strcmp(finalGraph->adjList[j].vertexName, ptr->vertex) != 0){
                j++;
            }
            if(finalGraph->adjList[j].visit == 0){
                head = enqueue(head, ptr->vertex);
                printf("%s", ptr->vertex);
                printf(" ");
            }
            //marking the input vertex as VISITED//
            for(int i = 0; i < numVertices; i++){
                if(strcmp(finalGraph->adjList[i].vertexName, ptr->vertex) == 0){
                    finalGraph->adjList[i].visit = 1;
                }
            }
        }
        free(hold);
    }
    printf("\n");
}       

int main (int argc, char* argv[argc +1]){
    FILE* fp = fopen(argv[1], "r");
    graph* finalGraph = makeGraph(fp);
    //string to input our source vertex and our adjacent vertex//
    char source[21];
    char adj[21];
 
    while(fscanf(fp, "%s %s\n", source, adj) != EOF){
        inputEdge(finalGraph, source, adj);
        inputEdge(finalGraph, adj, source);
    }
    char sourceVertex[21];
    FILE* fp2 = fopen(argv[2], "r");
    while(fscanf(fp2, "%s\n", sourceVertex)!= EOF){
        BFS(finalGraph, sourceVertex);
    }
    fclose(fp);
    fclose(fp2);
    freeMemory(finalGraph);
    return EXIT_SUCCESS;
}