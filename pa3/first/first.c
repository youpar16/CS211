#include<stdio.h>
#include<stdlib.h>
#include<math.h>
//converting our int to binary
void convert(long num, long bits, long binary[]){
    //keeping a count to make sure our binary has the appropriate # of bits//
    long count = bits-1;
    //initializing our input that will hold our binary values//
    long input;
    while(num > 0){
        //set input as the remainder when divided by 2//
        input = num % 2;
        //set num as num divided by 2//
        num = num / 2;
        //input into our array//
        binary[count] = input;
        //decrement our count//
        count--;
    }
    //for all of the bits we did not get to//
    for(long i = count; i >= 0; i--){
        //put in a 0//
        binary[i] = 0;
    }
}

int main(int argc, char* argv[argc+1]){
    if(argc < 2){
        printf("insufficient arguments\n");
        return EXIT_SUCCESS;
    }
    FILE* fp = fopen(argv[1], "r");
    long num;
    long bits;
    //while pointer is not at the end of the file//
    while(fscanf(fp, "%ld %ld\n", &num, &bits) != EOF){
        long binary[bits];
        //if our number is greater than our range for our bit size//
        if(num > pow(2, bits)-1){
            //set our num to the remainder when divided by 2^bits//
            long hold = pow(2, bits);
            num = num%hold;
        }
        convert(num, bits, binary);
        for(int i = 0; i < bits; i++){
            printf("%ld", binary[i]);
        }
        printf("\n");
    }
    fclose(fp);
    return EXIT_SUCCESS;
}
