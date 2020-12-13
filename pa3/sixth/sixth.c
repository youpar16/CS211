#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

void toBinary(char hex[], char binary[], int total, int count){
    int hold = (count-2)*4;
    char holderBinary[hold];

    for(int i = 2; i <= count; i++){
        switch (hex[i]) {
        case '0':
            if(i == 2){
                strcpy(holderBinary, "0000");
            }else{
                strcat(holderBinary, "0000");
            }
            break;
        case '1':
            if(i == 2){
                strcpy(holderBinary, "0001");
            }else{
                strcat(holderBinary, "0001");
            }
            break;
        case '2':
            if(i == 2){
                strcpy(holderBinary, "0010");
            }else{
                strcat(holderBinary, "0010");
            }
            break;
        case '3':
            if(i == 2){
                strcpy(holderBinary, "0011");
            }else{
                strcat(holderBinary, "0011");
            }
            break;
        case '4':
            if(i == 2){
                strcpy(holderBinary, "0100");
            }else{
                strcat(holderBinary, "0100");
            }
            break;
        case '5':
            if(i == 2){
                strcpy(holderBinary, "0101");
            }else{
                strcat(holderBinary, "0101");
            }
            break;
        case '6':
            if(i == 2){
                strcpy(holderBinary, "0110");
            }else{
                strcat(holderBinary, "0110");
            }
            break;
        case '7':
            if(i == 2){
                strcpy(holderBinary, "0111");
            }else{
                strcat(holderBinary, "0111");
            }
            break;
        case '8':
            if(i == 2){
                strcpy(holderBinary, "1000");
            }else{
                strcat(holderBinary, "1000");
            }
            break;
        case '9':
            if(i == 2){
                strcpy(holderBinary, "1001");
            }else{
                strcat(holderBinary, "1001");
            }
            break;
        case 'A':
        case 'a':
            if(i == 2){
                strcpy(holderBinary, "1010");
            }else{
                strcat(holderBinary, "1010");
            }
            break;
        case 'B':
        case 'b':
            if(i == 2){
                strcpy(holderBinary, "1011");
            }else{
                strcat(holderBinary, "1011");
            }
            break;
        case 'C':
        case 'c':
            if(i == 2){
                strcpy(holderBinary, "1100");
            }else{
                strcat(holderBinary, "1100");
            }
            break;
        case 'D':
        case 'd':
            if(i == 2){
                strcpy(holderBinary, "1101");
            }else{
                strcat(holderBinary, "1101");
            }
            break;
        case 'E':
        case 'e':
            if(i == 2){
                strcpy(holderBinary, "1110");
            }else{
                strcat(holderBinary, "1110");
            }
            break;
        case 'F':
        case 'f':
            if(i == 2){
                strcpy(holderBinary, "1111");
            }else{
                strcat(holderBinary, "1111");
            }
            break;
        }
    }
	
    if(total == hold){
        for(int i = 0; i < total; i++){
            binary[i] = holderBinary[i];
        }
    }
    if(total < hold){
        int x = hold-1;
        for(int i = total; i >0; i--){
            binary[i-1] = holderBinary[x];
            x--;
        }
    }

}

void denormal(char binary[], int total, int exponent, int fraction, int precision){
    double m;
    int power = -1;
    for(int i = 1 + exponent; i < total; i++){
        int hold = binary[i] - '0';
        m += pow(2, power) * hold;
        power--;
    }
    int bias = pow(2, exponent-1)-1;
    int E = 1-bias;

    double final = pow(2, E);
    if(binary[0] == '1'){
	printf("-");
    }
    printf("%.*lf\n", precision, m*final);
}

void normal(char binary[], int total, int exponent, int fraction, int precision){
    double m = 1;
    int power = -1;
    for(int i = 1 + exponent; i < total; i++){
        int hold = binary[i] - '0';
        m += pow(2, power) * hold;
        power--;
    }
    int bias = pow(2, exponent-1)-1;
    int exp = 0;
    int count = 0;
    for(int i = exponent; i > 0; i--){
        int holder = binary[i] - '0';
        exp += pow(2, count) * holder;
        count++;
    }
    int E = exp-bias;
    double final = pow(2, E);
    if(binary[0] == '1'){
	printf("-");
    }
    printf("%.*lf\n", precision, m*final);
}



int main(int argc, char* argv[argc+1]){
    if(argc < 2){
        printf("insufficient arguments\n");
        return EXIT_SUCCESS;
    }
    FILE* fp = fopen(argv[1], "r");
    int total;
    int exponent;
    int fraction;
    // long hex;
    char hex[100];
    for(int i = 0; i < 100; i++){
        hex[i] = 'z';
    }
    int precision;

    //while pointer is not at the end of the file//
    while(fscanf(fp,"%d %d %d %s %d", &total, &exponent, &fraction, hex, &precision) != EOF){
        // printf("%d %d %d %s %d\n", total, exponent, fraction, hex, precision);
	int isDenormal = 0;
        char binary[total];
        int count = -1;
        for(int i = 0; i < 100; i++){
            if(hex[i] != 'z'){
                count++;
            }
        }
        toBinary(hex, binary, total, count);
        for(int i = 1; i < exponent+1; i++){
            if(binary[i] == '1'){
                isDenormal = 1;
            }
        }
        if(isDenormal == 1){
            normal(binary, total, exponent, fraction, precision);
        }else{
            denormal(binary, total, exponent, fraction, precision);
        }
        
    }
    fclose(fp);
    return EXIT_SUCCESS;
}

