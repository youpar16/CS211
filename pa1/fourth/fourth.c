#include<stdio.h>
#include<stdlib.h>

//allocates memory for a matrix (learned in class)//
int** allocateMatrix(int size){
    //allocating the initial array of pointers//
    int **matrix = malloc(size * sizeof(int*));
    //allocating the array of ints//
    for(int i = 0; i < size; i++){
        matrix[i] = malloc(size * sizeof(int));
    }
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            matrix[i][j] = -1;
        }
    }
    return matrix;
}

//makes the magic square//
int** makeMagic(int size){
    //allocate the matrix//
    int** returnMatrix = allocateMatrix(size);
    //magic square of 1x1//
    if(size == 1){
        returnMatrix[0][0] = 1;
        return returnMatrix;
    }
    //initialize the place for '1' (first row middle column)//
    int i = 0;
    int j = (size/2);

    //holders for i and j in case matrix[i][j] already exist
    int holdi = 0;
    int holdj = 0;

    //loop to fill the entire matrix//
    for(int num = 1; i < size*size; num++){
        if(num > size*size){
            break;
        }
        //case where it has to loop from first row to the last row//
        if(i == -1){
            //loops from first row to last row//
            i = size - 1;
            //if it has to loop from last column to first colum//
            if(j == size){
                //loops over to first column
                j = 0;
            }
        //row is fine    
        }else{
            //if column has to loop over
            if(j == size){
                //loops over to first column
                j = 0;
            }
        //if something is already in that matrix slot
        }if(returnMatrix[i][j] > 0){
            //go back to where you came from and drop a row
            i = holdi + 1;
            j = holdj;
            //and then place num in that slot
            returnMatrix[i][j] = num;
            //before you move up and to the right, place your current slot to holdi and holdj
            holdi = i;
            holdj = j;
            i--;
            j++;
        //if nothing is there in that matrix slot
        }else{
            //just put the num in that slot
            returnMatrix[i][j] = num;
            //before you move up and to the right, place your current slot to holdi and holdj
            holdi = i;
            holdj = j;
            i--;
            j++;
        }
          
    }
    return returnMatrix;
}
//traverse through the matrix and print (tab in between each node and a new line for a new line)//
void printMagic(int** magicSquare, int size){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            printf("%d\t", magicSquare[i][j]);
        }
        printf("\n");
    }
}

//traverse through the matrix to free//
void freeMemory(int** magicSquare, int size){
    for(int i = 0; i < size; i++){
        free(magicSquare[i]);
    }
    free(magicSquare);
}

int main(int argc, char* argv[argc+1]){
    if(argc < 2){
        printf("insufficient arguments\n");
        return EXIT_SUCCESS;
    }
    //read the input number//
    int size = atoi(argv[1]);
    
    //if input number is an even number return error//
    if(size%2 == 0){
        printf("error\n");
    }else{
        int** magicSquare = makeMagic(size);
        printMagic(magicSquare, size);
        freeMemory(magicSquare, size);    

    }
    
    return EXIT_SUCCESS;
}