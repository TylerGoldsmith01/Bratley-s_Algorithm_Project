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
    fclose(infp);
    return 0;
}

struct map *parse(FILE *infp){
    int numInputCases=0;
    int currNumInputs;
    char currChar;

    int readingFile = 0;
    int readingEntry=0;
    int readingValueField=0;

    //Loop to wait for the start of the reading frame in the file
    while (!readingFile){
        if ((currChar = fgetc(infp))=='['){
            readingFile=1;
            printf("Reading Input [%c]\n",currChar);
        }
    }

    while(readingFile){
        currChar = fgetc(infp);

        //Check for the start of individual entries
        if(currChar =='{'){
            printf("\tReading entry [%c]\n", currChar);
            readingEntry = 1;

            while(readingEntry){
                currChar = fgetc(infp);
                //printf("%c ", currChar);

                if(currChar == '\"'){
                    printf("\tReading Value Field [%c]\n", currChar);
                    char *currValueField;
                    int currValueFieldindex = 0;

                    readingValueField = 1;

                    //Iterate through the character names
                    while((currChar = fgetc(infp)) != '\"'){
                        printf("\t\tReading Character in Value Field [%c]\n", currChar);
                        currValueField[currValueFieldindex] = currChar;
                        currValueFieldindex++;
                    }
                    currValueField[currValueFieldindex] = '\0';
                    printf("Current Value Field: %s\n", currValueField);
                    readingValueField = 0;
                if(currChar=='}')
                    readingEntry=0;
                }
            }
        if(currChar ==']')
            readingFile = 0;
        }
    }
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
