#include <stdio.h>
#include <stdlib.h>


struct map *parse(FILE *);

struct eventNode{
    struct eventNode ** children;
    struct eventNode * parent;
    int taskNum;
    int timeEnum;

    int arrivalTime;
    int deadline;
    int executionTime;
};

//map input data

struct mapEntry{
    int key;
    char * name;
    int arrivalTime;
    int deadline;
    int executionTime;

};

struct map{
    struct mapEntry *mapData;
    int numElements;
};

int main(void){
    printf("In\n");
    FILE *infp;
    infp = fopen("C:\\Users\\Tyler\\Desktop\\Bratley's_Algorithm_Project\\input.txt","r");
    struct map * inputMap = parse(infp);
    return 0;
}

struct map *parse(FILE *infp){
    printf("In parse\n");
    int numInputCases=0;
    int currNumInputs;
    char currChar;

    int readingFile = 0;
    int readingEntry=0;
    int readingEntryValues=0;

    while (!readingFile){
        if ((currChar = fgetc(infp))=='[')
            readingFile=1;
        printf("%c\n",currChar);
    }
/*
    while(readingFile){
        currChar = fgetc(infp);
        printf("%c ",currChar);
        if(currChar =='{')
            readingEntry = 1;

        if(readingEntry && currChar=='"'){
            if(!readingEntryValues){
                readingEntryValues = 1;
                char * tempSTR;
                int numChar = 0;
                while(currChar!='"'){
                    //Read in value
                }
                //string case statement to select parameter to be set
            }
            else
                readingEntryValues = 0;
        }

    }*/
}

//Recursive function to generate children
int generateChildren(struct eventNode *parent, int numChildren, int level){
    int child;
    struct eventNode* currChild;
    int childFinish;

    for(child=0 ; child<numChildren ; child++){
        currChild = malloc(sizeof(struct eventNode));
        childFinish = currChild->executionTime;

        if(parent->timeEnum < currChild->arrivalTime)
            childFinish += (currChild->arrivalTime);
        else
            childFinish += (parent->timeEnum);
        
        if(childFinish > currChild->deadline)
            return 0;
        else
            generateChildren(currChild,level,level+1);
    }
}
