#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//our counts for cache hit, cache miss, memory read, memory write, and age count//
int cacheHit = 0, cacheMiss = 0, memoryRead = 0, memoryWrite = 0, ageCount = 0;

//defining the struct for our cache which will be represented as a 2d array//
typedef struct cacheSet{
    //the valid bit which will either be 0 or 1//
    int valid;
    //the tag big//
    unsigned long long tag;
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
int checkforTag(cacheSet** cache, unsigned long long tagBits, long unsigned int numOfBlocks, unsigned long long setIndex, char cachePolicy[]){
    //simply loop through the rows of the given set index in the cache checking that the valid bit is not 0, and that the tag bit is equail to each other//
    for(int i = 0; i < numOfBlocks; i++){
        if(cache[setIndex][i].valid == 1){
            if(cache[setIndex][i].tag == tagBits){
            //if the cache policy is a LRU, reading the tag will mean it is the most recently used, so we must update its age to the most recent one//
                if(strcmp(cachePolicy, "lru") == 0){
                    cache[setIndex][i].age = ageCount;
                    ageCount++;
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
            cache[i][j].tag = 0;
            cache[i][j].age = 0;
        }
    }
}

//method to load an address to the cache//
void insertToCache(cacheSet** cache, long long tagBits, long long setIndex, long unsigned int numOfBlocks){
    //the count value we will use to hold place and the age while we find which block to evict//
    int holdAge, holdPlace;
    //first thing we should do is determine if there is an empty block in the set for us to put this address into//
    for(int i = 0; i < numOfBlocks; i++){
        //going through the columns of the row of the setIndex, if there is a valid bit which is 0 (which means nothing is there), we will fill that row w our address//
        if(cache[setIndex][i].valid == 0){
            cache[setIndex][i].valid = 1;
            cache[setIndex][i].tag = tagBits;
            cache[setIndex][i].age = ageCount;
            // printf("INPUT AGE: %d\n", ageCount);
            ageCount++;
            return;
        }
    }
    //if it did not go through the return, it means there were no free blocks in the set for the address to be inserted. therefore, we must evict one and replace it//
    //first initialize the holdAge and holdPlace to the first block
    holdPlace = 0;
    holdAge = cache[setIndex][0].age;
    //we must find the block that has the smallest age//
    for(int i = 0; i <numOfBlocks; i++){
        //for the blocks that are not the first one, update the holdPlace and holdAge, if the age is smaller (meaning it was input earlier)//
            if(i != 0){
                if(cache[setIndex][i].age < holdAge && cache[setIndex][i].valid == 1){
                    holdPlace = i;
                    holdAge = cache[setIndex][i].age;
                }
            }   
    }
    //now that the block with the smallest age is found, we can update that block with the new address//
    cache[setIndex][holdPlace].tag = tagBits;
    cache[setIndex][holdPlace].age = ageCount;
    ageCount++;
    return;
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
    if(argc != 6){
        printf("error\n");
        return EXIT_SUCCESS;    
    }
    //values of the total size of the cache in bytes, and the size of the cache block in bytes//
    //both should be a power of 2//
    long unsigned int cacheSize, blockSize;
    //cachesize is the 2nd argument inserted while blocksize is the 5th argument inserted//
    cacheSize = atoi(argv[1]);
    blockSize = atoi(argv[4]);
    int powerCheck1, powerCheck2;
    powerCheck1 = powerOfTwo(cacheSize);
    powerCheck2 = powerOfTwo(blockSize);
    //if either of cacheSize or blockSize is NOT a power of two, print error and exit the program//
    if(powerCheck1 == 0 || powerCheck2 == 0){
        printf("error\n");
        return EXIT_SUCCESS;
    }
    //indicator of the cachepolicy labeled as either "fifo" or "lru", inserted as the 4th argument//
    char cachePolicy[50];
    strcpy(cachePolicy, argv[3]);
    //determining the associativity to 1 of 3 things, inserted as the 3rd argument//
    //will be used to determine the number of blocks, as well as the number of sets//
    unsigned long long numOfBlocks, numOfSets;
    //if the first letter of the associativity argument is a d, it is a direct mapped cache//
    if(argv[2][0] == 'd'){
        numOfBlocks = 1;
        //number of sets would equal the total cache size divided by the blockSize//
        numOfSets = cacheSize/blockSize;
    //if there exists a 'o' as the 4th letter and there is a ':' in the 6th letter, it is a n way associate cache//
    }else if(argv[2][3] == 'o' && argv[2][5] == ':'){
        //scan in the n value for our n way associated cache and set it as our blockNum//
        sscanf(argv[2], "assoc:%llx", &numOfBlocks);
        int hold = numOfBlocks * blockSize;
        //number of sets would equal the total cache size divided by the blockSize//
        numOfSets = cacheSize/hold;
    //if else, it is a fully associated cache//
    }else{
        //in a fully associated cache, there is 1 set and the number of blocks in the set would be the total cache size divided by the block size//
        numOfSets = 1;
        numOfBlocks = cacheSize/blockSize;
    }
    //now that we have inserted the set value and the block value, we can calulate the set bits we need to obtain from our address as well as our block bits//
    unsigned long long blockBits, setBits;
    blockBits = log2(blockSize);
    setBits = log2(numOfSets);
    //now that all the values for the components of the cache is set, we can initialize the cache//
    cacheSet** cache = (cacheSet**)malloc(numOfSets * sizeof(cacheSet*));
    //now we must set according to how many blocks there are in the set//
    for(int i = 0; i < numOfSets; i++){
        cache[i] = (cacheSet*)malloc((numOfBlocks) * sizeof(cacheSet));
    }
    //initialize all components of the cache to 0//
    alltoZero(cache, numOfBlocks, numOfSets);
    //now we can open the input file and scan for addresses//
    FILE* fp = fopen(argv[5], "r");
    if(fp == NULL){
        printf("error\n");
        return EXIT_SUCCESS;
    }
    unsigned long long addressInput, address, setIndex, tagBits;
    //will determine whether we read or write//
    char operation;
    //boolean int to see if the tag bit was found in the cache or not//
    int tagCheck;
    //scanning the input file for our addresses//
    while(fscanf(fp, "%c %llx\n", &operation, &addressInput)!= EOF){
        //we must first get rid of the block value as we are not worried about inputting data//
        address = addressInput>>blockBits;
        //then we must evaluate the set index (method explained in recitation/piazza)//
        setIndex = address & ((1<<setBits)-1);
        //now we can identify the tag which is just the reamining value after we take out the block bits and the set bits (right now we already took off the block bits so we just need to take off the set bits)//
        tagBits = address>>setBits;
        //now we must determine if it is a write command or a read command//
        //if the input operation is a write//
        if(operation == 'W'){
            //we must write to memory
            memoryWrite++;
        }
        //check the cache for if the given address is already existent or not//
        tagCheck = checkforTag(cache, tagBits, numOfBlocks, setIndex, cachePolicy);
        //if it does exist//
        if(tagCheck == 1){
            //increment cache hit//
            cacheHit++;
        //if it does not exist//
        }else{
            //increment cache miss and memory read while also inputting the address into our cache//
            cacheMiss++;
            memoryRead++;
            insertToCache(cache, tagBits, setIndex, numOfBlocks);
        }
    }
    //print out our results, free the memory we allocated for the cache, close the printer and end the program//
    printf("memread:%d\nmemwrite:%d\ncachehit:%d\ncachemiss:%d\n", memoryRead, memoryWrite, cacheHit, cacheMiss);
    freeMemory(cache, numOfSets);
    fclose(fp);
    return EXIT_SUCCESS;
}