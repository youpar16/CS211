#include<stdio.h>
#include<stdlib.h>
//to prevent implicit declaration//
int primeCheck(int input);

int main(int argc, char* argv[argc+1]){
    if(argc < 2){
        printf("insufficient arguments\n");
        return EXIT_SUCCESS;
    }
    FILE* fp = fopen(argv[1], "r");
    //while pointer is not at the end of the file
    while(!feof(fp)){
        int prime;
        //set prime to the int that pointer is at
        fscanf(fp, "%d\n", &prime);
            //if the given integer is prime
            if (primeCheck(prime) == 1){
                //check if int+2 int -2 is also prime
                int twinPrime1 = prime + 2;
                int twinPrime2 = prime - 2;
                //if ONE of the two is prime
                if(primeCheck(twinPrime1)== 1 || primeCheck(twinPrime2) == 1){
                    //print yes
                    printf("yes\n");
                //if either are not prime 
                }else{
                    //print no
                    printf("no\n");
                }
            //if given integer is not prime
            }else{
                //print no
                printf("no\n");
            }
    }
    //close file
    fclose(fp);
    return EXIT_SUCCESS;
}

//method to check if the input number is prime//
int primeCheck(int input){
    //output of 0 will signify false which means it is NOT prime//
    //output of 1 will signify true which means it IS prime//
    int output = 0;
    //if the input is one or lower it is NOT prime//
    if(input <= 1){
        //therefore, return FALSE//
        return output;
    }else{
        //divide the input by all the numbers from 2 to input divided by 2//
        for(int i = 2; i < input / 2; i++){
            //if there is a number that divides into input with no remainder//
            if(input % i == 0){
                //return false sign signifying the input is NOT prime//
                return output;
            //if no number divides into input evenly, return true sinifiying the input IS prime//    
            }
        }
        output = 1;
        return output;
    }
    return 0;
}