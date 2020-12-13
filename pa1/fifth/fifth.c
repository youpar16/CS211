#include<stdio.h>
#include<stdlib.h>
//traverse through the matrix and print (tab in between each node and a new line for a new line)//
void printMagic(int** magicSquare, int size){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            printf("%d\t", magicSquare[i][j]);
        }
        printf("\n");
    }
}

//allocates space for a matrix and fills it up with input (went over in class)//
int** readMatrix(FILE* fp, int* size1){
    //for sizeXsize matrix//
    int size;
    //read the size of the matrix//
    fscanf(fp, "%d\n", &size);
    *size1 = size;

    //allocating space for the size*size matrix//
    int **matrix = malloc(size * sizeof(int*));
    for(int i = 0; i < size; i++){
        matrix[i] = malloc(size * sizeof(int));
    }

    //inputting contents of the matrix//
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            fscanf(fp, "%d\t", &matrix[i][j]);
        }
        fscanf(fp, "\n");
    }
    return matrix;
}
//allocates memory for a matrix (learned in class)//
int** allocateMatrix(int size){
    //allocating the initial array of pointers//
    int **matrix = malloc(size * sizeof(int*));
    //allocating the array of ints//
    for(int i = 0; i < size; i++){
        matrix[i] = malloc(size * sizeof(int));
    }
    return matrix;
}

//returns the smaller matrix made after pivoting//
int** makeMatrix(int** smallMatrix, int** matrix, int size, int i){
    int smallRow = 0;
    int smallColumn = 0;
    //looping through the second row(row index 1) of the matrix to set to the smallMatrix//
        for(int row = 1; row < size; row++){
            //looping through the columns
            for(int column= 0; column < size; column++){
                //if the column that we are currently on is the column that our pivot entry is from SKIP//
                if(column == i){
                    continue;
                }else{
                    //otherwise set corresponding index of small matrix to corresponding index of the input matrix//
                    smallMatrix[smallRow][smallColumn] = matrix[row][column];
                    //then go to the next column//
                    smallColumn++;
                    //this checks if we reached the end of the column in the smallMatrix//
                    if(smallColumn == size-1){
                        //moves on to the next row and back to first column//
                        smallRow++;
                        smallColumn = 0;
                    }
                }
            }
        }    
    return smallMatrix;
}

//freeing the memory allocated to the matrix (learned in class)//
void freeMatrix(int** matrix, int size){
    for(int i = 0; i < size; i++){
        free(matrix[i]);
    }
    free(matrix);
}

//recursively calculates the determinate//
int determinant (int** matrix, int size){
    //the calculated determinant we will be returning//
    int answer = 0;
    //the sign that will change to add/subtract to calculate our determinant//
    int alternate = 1;
    //the matrix is 1x1//
    if(size == 1){
        answer = matrix[0][0];
        return answer;
    //the base case of our recursive method when our matrix is not 2x2//    
    }if(size == 2){
        answer = matrix[0][0]*matrix[1][1] - matrix[0][1]*matrix[1][0];
        return answer;
    //if our matrix is larger than 2x2
    }else{
        //loop through the entire first row of the matrix//
        for(int i = 0; i < size; i++){
            //initialize the smaller matrix of size (size-1)x(size-1)//
            int** smallMatrix = allocateMatrix(size-1);
            smallMatrix = makeMatrix(smallMatrix, matrix, size, i);
            //recursively alternate between add and subtract of the index multiplied by the determinant of the smaller matrix made from the pivot index//
            answer += alternate * matrix[0][i] * determinant(smallMatrix, size-1);
            //frees memory from our matrix after recursion//
            freeMatrix(smallMatrix, size-1);
            //alternates between positive and negative for add/subtract //
            alternate = -alternate;
        }
    }
    return answer;
}


int main(int argc, char* argv[argc+1]){
    FILE* fp = fopen(argv[1], "r");
    int size1;
    //read and allocate memory for matrix//
    int** mat = readMatrix(fp, &size1);
    //recursive method to find determinant//
    int answer = determinant(mat, size1);
    //print our answer//
    printf("%d\n", answer);
    //free our matrix that we allocated memory for//
    freeMatrix(mat, size1);
    //close our file//
    fclose(fp);
    return EXIT_SUCCESS;
}