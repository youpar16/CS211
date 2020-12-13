#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//our counts for cache hit, cache miss, memory read, memory write, and age count//
int cacheHitL1 = 0, cacheHitL2 = 0, cacheMissL1 = 0, cacheMissL2 = 0, memoryRead = 0, memoryWrite = 0, ageCountL1 = 0, ageCountL2 = 0;

//defining the struct for our cache which will be represented as a 2d array//
typedef struct cacheSet{
    //the valid bit which will either be 0 or 1//
    int valid;
    //the tag big//
    unsigned long long tagL1;
    unsigned long long tagL2;
    //storing the setIndex and the numOfBlocks//
    long long setL1;
    long long setL2;
    long unsigned int blockL1;
    long unsigned int blockL2;
    //age that will be useful in eviction//
    int age;
}cacheSet;

//boolean method to check if an inserted value is a power of 2//
int powerOfTwo(long unsigned int input){
    //if input is 0, it cannot be a power of 2 and according to bitwise operation, a value & value - 1 will return 0 if it is a power of 2//
    if((input != 0) && ((input &(input - 1)) == 0)){
        return 1;
    }else{
        return 0;
    }
}

//boolean method to check if the tag bits exist in the cache//
int checkforTagL1(cacheSet** cache, unsigned long long tagBits, long unsigned int numOfBlocks, unsigned long long setIndex, char cachePolicy[]){
    //simply loop through the rows of the given set index in the cache checking that the valid bit is not 0, and that the tag bit is equail to each other//
    for(int i = 0; i < numOfBlocks; i++){
        if(cache[setIndex][i].valid == 1){
            if(cache[setIndex][i].tagL1 == tagBits){
            //if the cache policy is a LRU, reading the tag will mean it is the most recently used, so we must update its age to the most recent one//
                if(strcmp(cachePolicy, "lru") == 0){
                    cache[setIndex][i].age = ageCountL1;
                    ageCountL1++;
                }
                return 1;
            }      
        }
    }
    return 0;
}


//method to initialize all components of the cache to 0//
void alltoZero(cacheSet** cache, long unsigned int numOfBlocks, long unsigned int numOfSets){
    for(int i = 0; i < numOfSets; i++){
        for(int j = 0; j < numOfBlocks; j++){
            cache[i][j].valid = 0;
            cache[i][j].tagL1 = 0;
            cache[i][j].tagL2 = 0;
            cache[i][j].setL1 = 0;
            cache[i][j].setL2 = 0;
            cache[i][j].blockL1 = 0;
            cache[i][j].blockL2 = 0;
            cache[i][j].age = 0;
        }
    }
}

//method to load an address to the cache//
void insertToCacheL2(cacheSet** cacheL1, cacheSet** cacheL2, long long tagBitsL1, long long tagBitsL2, long long setIndexL1, long long setIndexL2, long unsigned int numOfBlocksL1, long unsigned int numOfBlocksL2){
    //the count value we will use to hold place and the age while we find which block to evict//
    int holdAge, holdPlace;
    //first thing we should do is determine if there is an empty block in the set for us to put this address into//
    for(int i = 0; i < numOfBlocksL2; i++){
        //going through the columns of the row of the setIndex, if there is a valid bit which is 0 (which means nothing is there), we will fill that row w our address//
        if(cacheL2[setIndexL2][i].valid == 0){
            cacheL2[setIndexL2][i].valid = 1;
            cacheL2[setIndexL2][i].tagL1 = tagBitsL1;
            cacheL2[setIndexL2][i].tagL2 = tagBitsL2;
            cacheL2[setIndexL2][i].setL1 = setIndexL1;
            cacheL2[setIndexL2][i].setL2 = setIndexL2;
            cacheL2[setIndexL2][i].blockL1 = numOfBlocksL1;
            cacheL2[setIndexL2][i].blockL2 = numOfBlocksL2;
            cacheL2[setIndexL2][i].age = ageCountL2;
            ageCountL2++;
            return;
        }
    }
    //if it did not go through the return, it means there were no free blocks in the set for the address to be inserted. therefore, we must evict one and replace it//
    //first initialize the holdAge and holdPlace to the first block
    holdPlace = 0;
    holdAge = cacheL2[setIndexL2][0].age;
    //we must find the block that has the smallest age//
    for(int i = 0; i <numOfBlocksL2; i++){
        //for the blocks that are not the first one, update the holdPlace and holdAge, if the age is smaller (meaning it was input earlier)//
            if(i != 0){
                if(cacheL2[setIndexL2][i].age < holdAge && cacheL2[setIndexL2][i].valid == 1){
                    holdPlace = i;
                    holdAge = cacheL2[setIndexL2][i].age;
                }
            }   
    }
    //now we can update that block with the new address//
    cacheL2[setIndexL2][holdPlace].tagL1 = tagBitsL1;
    cacheL2[setIndexL2][holdPlace].tagL2 = tagBitsL2;
    cacheL2[setIndexL2][holdPlace].setL1 = setIndexL1;
    cacheL2[setIndexL2][holdPlace].setL2 = setIndexL2;
    cacheL2[setIndexL2][holdPlace].blockL1 = numOfBlocksL1;
    cacheL2[setIndexL2][holdPlace].blockL2 = numOfBlocksL2;
    cacheL2[setIndexL2][holdPlace].age = ageCountL2;
    ageCountL2++;
    return;
}

//method to load an address to the cache//
void insertToCacheL1(cacheSet** cacheL1, cacheSet** cacheL2, long long tagBitsL1, long long tagBitsL2, long long setIndexL1, long long setIndexL2, long unsigned int numOfBlocksL1, long unsigned int numOfBlocksL2){
    //the count value we will use to hold place and the age while we find which block to evict//
    int holdAge, holdPlace;
    //first thing we should do is determine if there is an empty block in the set for us to put this address into//
    for(int i = 0; i < numOfBlocksL1; i++){
        //going through the columns of the row of the setIndex, if there is a valid bit which is 0 (which means nothing is there), we will fill that row w our address//
        if(cacheL1[setIndexL1][i].valid == 0){
            cacheL1[setIndexL1][i].valid = 1;
            cacheL1[setIndexL1][i].tagL1 = tagBitsL1;
            cacheL1[setIndexL1][i].tagL2 = tagBitsL2;
            cacheL1[setIndexL1][i].setL1 = setIndexL1;
            cacheL1[setIndexL1][i].setL2 = setIndexL2;
            cacheL1[setIndexL1][i].blockL1 = numOfBlocksL1;
            cacheL1[setIndexL1][i].blockL2 = numOfBlocksL2;
            cacheL1[setIndexL1][i].age = ageCountL1;
            ageCountL1++;
            return;
        }
    }
    //if it did not go through the return, it means there were no free blocks in the set for the address to be inserted. therefore, we must evict one and replace it//
    //first initialize the holdAge and holdPlace to the first block
    holdPlace = 0;
    holdAge = cacheL1[setIndexL1][0].age;
    //we must find the block that has the smallest age//
    for(int i = 0; i <numOfBlocksL1; i++){
        //for the blocks that are not the first one, update the holdPlace and holdAge, if the age is smaller (meaning it was input earlier)//
            if(i != 0){
                if(cacheL1[setIndexL1][i].age < holdAge && cacheL1[setIndexL1][i].valid == 1){
                    holdPlace = i;
                    holdAge = cacheL1[setIndexL1][i].age;
                }
            }   
    }
    //now that the block with the smallest age is found, we must evict that block in to cache L2//
    insertToCacheL2(cacheL1, cacheL2, cacheL1[setIndexL1][holdPlace].tagL1, cacheL1[setIndexL1][holdPlace].tagL2, cacheL1[setIndexL1][holdPlace].setL1, cacheL1[setIndexL1][holdPlace].setL2, cacheL1[setIndexL1][holdPlace].blockL1, cacheL1[setIndexL1][holdPlace].blockL2);
    //now we can update that block with the new address//
    cacheL1[setIndexL1][holdPlace].tagL1 = tagBitsL1;
    cacheL1[setIndexL1][holdPlace].tagL2 = tagBitsL2;
    cacheL1[setIndexL1][holdPlace].setL1 = setIndexL1;
    cacheL1[setIndexL1][holdPlace].setL2 = setIndexL2;
    cacheL1[setIndexL1][holdPlace].blockL1 = numOfBlocksL1;
    cacheL1[setIndexL1][holdPlace].blockL2 = numOfBlocksL2;
    cacheL1[setIndexL1][holdPlace].age = ageCountL1;
    ageCountL1++;
    return;
}

//boolean method to check if the tag bits exist in the cache//
int checkforTagL2(cacheSet** cacheL1, cacheSet** cacheL2, unsigned long long tagBits, long unsigned int numOfBlocks, unsigned long long setIndex, char cachePolicy[]){
    //simply loop through the rows of the given set index in the cache checking that the valid bit is not 0, and that the tag bit is equail to each other//
    for(int i = 0; i < numOfBlocks; i++){
        if(cacheL2[setIndex][i].valid == 1){
            if(cacheL2[setIndex][i].tagL2 == tagBits){
                //if it does exist, delete it by making the valid bit 0//
                cacheL2[setIndex][i].valid = 0;
                return 1;
            }      
        }
    }
    return 0;
}

//traverse through the cache to free//
void freeMemory(cacheSet** cache, long unsigned int numOfSets){
    for(int i = 0; i < numOfSets; i++){
        free(cache[i]);
    }
    free(cache);
}

int main(int argc, char* argv[argc+1]){
    //checking to see that there are the right number of inputs//
    if(argc != 9){
        printf("error\n");
        return EXIT_SUCCESS;    
    }
    //value for blockSize is shared by both L1 and L2, cache size is entered seperately L1 as the 2nd argument and L2 as the 6th argument//
    long unsigned int cacheSizeL1, cacheSizeL2, blockSize;
    cacheSizeL1 = atoi(argv[1]);
    cacheSizeL2 = atoi(argv[5]);
    blockSize = atoi(argv[4]);
    int powerCheckL1, powerCheckL2, powerCheck;
    //cache size for L1 and L2 as well as the block size should all be powers of two//
    powerCheckL1 = powerOfTwo(cacheSizeL1);
    powerCheckL2 = powerOfTwo(cacheSizeL2);
    powerCheck = powerOfTwo(blockSize);
    //if any of them are not powers of two, return error//
    if(powerCheckL1 == 0 || powerCheckL2 == 0 || powerCheck == 0){
        printf("error\n");
        return EXIT_SUCCESS;
    }
    //indicator of the cachepolicy labeled as either "fifo" or "lru", inserted as the 4th argument for L1 and 8th argument for L2//
    char cachePolicyL1[50];
    char cachePolicyL2[50];
    strcpy(cachePolicyL1, argv[3]);
    strcpy(cachePolicyL2, argv[7]);
    //determining the associativity to 1 of 3 things, inserted as the 3rd argument for L1 and 7th argument for L2//
    //will be used to determine the number of blocks, as well as the number of sets//
    unsigned long long numOfBlocksL1, numOfSetsL1, numOfBlocksL2, numOfSetsL2;
    //if the first letter of the associativity argument is a d, it is a direct mapped cache//
    if(argv[2][0] == 'd'){
        numOfBlocksL1 = 1;
        //number of sets would equal the total cache size divided by the blockSize//
        numOfSetsL1 = cacheSizeL1/blockSize;
    //if there exists a 'o' as the 4th letter and there is a ':' in the 6th letter, it is a n way associate cache//
    }else if(argv[2][3] == 'o' && argv[2][5] == ':'){
        //scan in the n value for our n way associated cache and set it as our blockNum//
        sscanf(argv[2], "assoc:%llx", &numOfBlocksL1);
        int hold = numOfBlocksL1 * blockSize;
        //number of sets would equal the total cache size divided by the blockSize//
        numOfSetsL1 = cacheSizeL1/hold;
    //if else, it is a fully associated cache//
    }else{
        //in a fully associated cache, there is 1 set and the number of blocks in the set would be the total cache size divided by the block size//
        numOfSetsL1 = 1;
        numOfBlocksL1 = cacheSizeL1/blockSize;
    }
    //if the first letter of the associativity argument is a d, it is a direct mapped cache//
    if(argv[6][0] == 'd'){
        numOfBlocksL2 = 1;
        //number of sets would equal the total cache size divided by the blockSize//
        numOfSetsL2 = cacheSizeL2/blockSize;
    //if there exists a 'o' as the 4th letter and there is a ':' in the 6th letter, it is a n way associate cache//
    }else if(argv[6][3] == 'o' && argv[6][5] == ':'){
        //scan in the n value for our n way associated cache and set it as our blockNum//
        sscanf(argv[6], "assoc:%llx", &numOfBlocksL2);
        int hold = numOfBlocksL2 * blockSize;
        //number of sets would equal the total cache size divided by the blockSize//
        numOfSetsL2 = cacheSizeL2/hold;
    //if else, it is a fully associated cache//
    }else{
        //in a fully associated cache, there is 1 set and the number of blocks in the set would be the total cache size divided by the block size//
        numOfSetsL2 = 1;
        numOfBlocksL2 = cacheSizeL2/blockSize;
    }
    //now that we have inserted the set value and the block value for both L1 and L2 caches, we can calulate the set bits we need to obtain from our address as well as our block bits//
    unsigned long long blockBits, setBitsL1, setBitsL2;
    blockBits = log2(blockSize);
    setBitsL1 = log2(numOfSetsL1);
    setBitsL2 = log2(numOfSetsL2);
    //now that all the values for the components of the cache is set, we can initialize the L1 and L2 cache//
    cacheSet** cacheL1 = (cacheSet**)malloc(numOfSetsL1 * sizeof(cacheSet*));
    //now we must set according to how many blocks there are in the set//
    for(int i = 0; i < numOfSetsL1; i++){
        cacheL1[i] = (cacheSet*)malloc((numOfBlocksL1) * sizeof(cacheSet));
    }
    cacheSet** cacheL2 = (cacheSet**)malloc(numOfSetsL2 * sizeof(cacheSet*));
    //now we must set according to how many blocks there are in the set//
    for(int i = 0; i < numOfSetsL2; i++){
        cacheL2[i] = (cacheSet*)malloc((numOfBlocksL2) * sizeof(cacheSet));
    }
    //initialize all components of the cache to 0//
    alltoZero(cacheL1, numOfBlocksL1, numOfSetsL1);
    alltoZero(cacheL2, numOfBlocksL2, numOfSetsL2);
    //now we can open the input file and scan for addresses//
    FILE* fp = fopen(argv[8], "r");
    if(fp == NULL){
        printf("error\n");
        return EXIT_SUCCESS;
    }
    unsigned long long addressInput, address, setIndexL1, setIndexL2, tagBitsL1, tagBitsL2;
    //will determine whether we read or write//
    char operation;
    //boolean int to see if the tag bit was found in the cache or not//
    int tagCheckL1, tagCheckL2;
    //scanning the input file for our addresses//
    while(fscanf(fp, "%c %llx\n", &operation, &addressInput)!= EOF){
        //we must first get rid of the block value as we are not worried about inputting data//
        address = addressInput>>blockBits;
        //then we must evaluate the set index (method explained in recitation/piazza)//
        setIndexL1 = address & ((1<<setBitsL1)-1);
        setIndexL2 = address & ((1<<setBitsL2)-1);
        //now we can identify the tag which is just the reamining value after we take out the block bits and the set bits (right now we already took off the block bits so we just need to take off the set bits)//
        tagBitsL1 = address>>setBitsL1;
        tagBitsL2 = address>>setBitsL2;
        //now we must determine if it is a write command or a read command//
        //if the input operation is a write//
        if(operation == 'W'){
            //we must write to memory
            memoryWrite++;
        }
        //check the cache for if the given address is already existent or not//
        tagCheckL1 = checkforTagL1(cacheL1, tagBitsL1, numOfBlocksL1, setIndexL1, cachePolicyL1);
        //if it does exist//
        if(tagCheckL1 == 1){
            //increment cache hit//
            cacheHitL1++;
        //if it does not exist//
        }else{
            //increment cache miss for L1 and then check if it exists in cache L2//
            cacheMissL1++;
            tagCheckL2 = checkforTagL2(cacheL1, cacheL2, tagBitsL2, numOfBlocksL2, setIndexL2, cachePolicyL2);
            //if it does exist in cache L2//
            if(tagCheckL2 == 1){
                //increment cache hit for L2 and then add it to our cache L1//
                cacheHitL2++;
                insertToCacheL1(cacheL1, cacheL2, tagBitsL1, tagBitsL2, setIndexL1, setIndexL2, numOfBlocksL1, numOfBlocksL2);
            //if it does not exist in cache L2//
            }else{
                //increment cache miss L2, read from memory and then add to our cache L1//
                cacheMissL2++;
                memoryRead++;
                insertToCacheL1(cacheL1, cacheL2, tagBitsL1, tagBitsL2, setIndexL1, setIndexL2, numOfBlocksL1, numOfBlocksL2);
            }
        }
    }
     //print out our results, free the memory we allocated for the cache, close the printer and end the program//
    printf("memread:%d\nmemwrite:%d\nl1cachehit:%d\nl1cachemiss:%d\nl2cachehit:%d\nl2cachemiss:%d\n", memoryRead, memoryWrite, cacheHitL1, cacheMissL1, cacheHitL2, cacheMissL2);
    freeMemory(cacheL1, numOfSetsL1);
    freeMemory(cacheL2, numOfSetsL2);
    fclose(fp);
    return EXIT_SUCCESS;
}
