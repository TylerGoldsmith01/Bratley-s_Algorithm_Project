#include <stdio.h>
#include <stdlib.h>
//#include <string.h>

#define MAX_FIELD_LENGTH 20

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
    int readingVariable_Name=0;
    int readingVariable_Value=0;

    struct map *tempMap;

    //Loop to wait for the start of the reading frame in the file
    while (!readingFile){
        currChar = fgetc(infp);
        if (currChar =='['){
            readingFile=1;
            printf("Reading Input [%c]\n",currChar);
        }
    }

    while(readingFile){
        //Wait until you get to an entry to read
        while (!readingEntry && readingFile){
            currChar = fgetc(infp);
            if(currChar =='{'){
                readingEntry = 1;
                printf("\tReading entry [%c]\n", currChar);
            } else if (currChar == ']'){
                readingFile=0;
                printf("Done Reading Input File");
            }
        }

        while(readingEntry){
            //Wait until you get to a value field to read
            while(!readingVariable_Name){
                currChar = fgetc(infp);
                if(currChar == '\"'){
                    printf("\tReading Variable Name [%c]\n", currChar);
                    readingVariable_Name = 1;
                }
            }
                while (readingVariable_Name){
                    char Variable_Name[MAX_FIELD_LENGTH]; // Assuming MAX_FIELD_LENGTH is a constant defining the maximum length of value fields
                    int Variable_Name_Index = 0;
                    currChar = fgetc(infp);

                    while(currChar != '\"'){
                        printf("\t\tReading Character in Variable Name [%c]\n", currChar);
                        Variable_Name[Variable_Name_Index] = currChar;
                        Variable_Name_Index++;
                        currChar = fgetc(infp);
                    }
                    Variable_Name[Variable_Name_Index] = '\0';
                    printf("Current Value Field: %s\n", Variable_Name);
                    readingVariable_Name=0;
                }

                while(!readingVariable_Value){
                    currChar = fgetc(infp);
                    if(currChar == ':'){
                        printf("\tReading Variable Value [%c]\n", currChar);
                        fgetc(infp);
                        readingVariable_Value = 1;
                    }
                }
                while (readingVariable_Value){
                    char Variable_Value[MAX_FIELD_LENGTH]; // Assuming MAX_FIELD_LENGTH is a constant defining the maximum length of value fields
                    int Variable_Value_Index = 0;
                    currChar = fgetc(infp);
                    if(currChar=='\"')
                        currChar = fgetc(infp);
                    while(currChar != '\"' && currChar != ',' && currChar != '\n'){
                        printf("\t\tReading Character in Variable Name [%c]\n", currChar);
                        Variable_Value[Variable_Value_Index] = currChar;
                        Variable_Value_Index++;
                        currChar = fgetc(infp);
                    }
                    Variable_Value[Variable_Value_Index] = '\0';
                    printf("Current Value Field: %s\n", Variable_Value);
                    //Delay so the current character is always the comma
                    if(currChar=='\"')
                        currChar = fgetc(infp);
                    readingVariable_Value=0;
                }
                if(currChar!=','){
                    while (currChar!='}'){
                        currChar = fgetc(infp);
                    }
                    readingEntry=0;
                }
        }
    }
    return tempMap;
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
