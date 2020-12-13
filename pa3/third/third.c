#include<stdio.h>
#include<stdlib.h>
#include<math.h>

//converting our int to binary
void convertU(long num, long bits, long binary[]){
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

//reading a signed binary representation//
int readBinaryS(long binary[], long bits){
    int answer = 0;
    int count = 0;
    for(int i = bits-1; i >= 0; i--){
        if(i != 0){
            answer += binary[i]* pow(2,count);
            count++;
        }else{
            answer += binary[i]* pow(2,count) * -1;
        }
    }
    return answer;
}

int readBinaryU(long binary[], long bits){
    int answer = 0;
    int count = 0;
    for(int i = bits-1; i >= 0; i--){
        answer += binary[i]* pow(2,count);
        count++;
    }
    return answer;
}

//in order to get the 2's complement we must inverse the binary form and then add 1//
void add(long binary[], long bits){
    //boolean holder to see if adding one will cause an overflow (2)//
    int overflow = 0;
    long index = bits -1;
    //if the last binary is 0, just change it to 1 and return//
    if(binary[index] == 0){
        binary[index] = 1;
        return;
    }
    //if it is 1 however//
    if(binary[index] == 1){
        //mark overflow as true//
        overflow = 1;
        //while overflow is true//
        while(overflow == 1){
            //change the 1 to a 0//
            if(binary[index] == 1){
                binary[index] = 0;
                //check the next binary//
                index--;
            //if it is a 0//
            }else{
                //change it to a 1//
                binary[index] = 1;
                //mark the boolean as false//
                overflow = 0;
            }
        }
    }
}

//converting our int to binary
void convertS(long num, long bits, long binary[]){
    int negative = 0;
    if(num < 0){
        num = num * -1;
        negative = 1;
    }
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
    //if the input value was negative//
    if(negative == 1){
        //swap all of the binary digits//
        for(int i = 0; i < bits; i++){
            if(binary[i] == 0){
                binary[i] = 1;
            }else{
                binary[i] = 0;
            }
        }
        //add 1//
        add(binary, bits);
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
    char source;
    char destination;
    //while pointer is not at the end of the file//
    while(fscanf(fp, "%ld %ld %c %c\n", &num, &bits, &source, &destination) != EOF){
        long answer = 0;
        long binary[bits];
        if(source == 'u'){
            convertU(num, bits, binary);
            answer = readBinaryS(binary, bits);
            printf("%ld\n", answer);
        }else{
            convertS(num, bits, binary);
            answer = readBinaryU(binary, bits);
            printf("%ld\n", answer);
        }
    }
    fclose(fp);
    return EXIT_SUCCESS;
}