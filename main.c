#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    struct mapEntry **mapData;
    int numElements;
};

int main(void){
    FILE *infp;
    infp = fopen("C:\\Users\\Tyler\\Desktop\\Bratley's_Algorithm_Project\\input.txt","r");
    struct map * inputMap = parse(infp);

    for(int i=0;i<4;i++)
    printf("\nINPUT: %d,%s,%d,%d,%d\n",
    inputMap->mapData[i]->key,
    inputMap->mapData[i]->name,
    inputMap->mapData[i]->arrivalTime,
    inputMap->mapData[i]->deadline,
    inputMap->mapData[i]->executionTime);

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

    int current_Variable=-1;
    int currInput=0;

    struct map *tempMap;
    struct mapEntry *currMapEntry;

    tempMap = malloc(sizeof(struct map));

    char numInputStr[MAX_FIELD_LENGTH];
    int numInputStr_Index = 0;
    currChar = fgetc(infp);
    while(currChar != '\n'){
        numInputStr[numInputStr_Index]=currChar;
        numInputStr_Index++;
        currChar = fgetc(infp);
    }
    numInputStr[numInputStr_Index] = '\0';
    tempMap->numElements = atoi(numInputStr);
    //printf("NUMINPUTS: %d\n", tempMap->numElements);

    tempMap->mapData = malloc(tempMap->numElements * sizeof(struct mapEntry*));


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
                //printf("\tReading entry [%c]\n", currChar);

                currMapEntry = malloc(sizeof(struct mapEntry));

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
                    //printf("\tReading Variable Name [%c]\n", currChar);
                    readingVariable_Name = 1;
                }
            }
                while (readingVariable_Name){
                    char Variable_Name[MAX_FIELD_LENGTH]; // Assuming MAX_FIELD_LENGTH is a constant defining the maximum length of value fields
                    int Variable_Name_Index = 0;
                    currChar = fgetc(infp);

                    while(currChar != '\"'){
                        //printf("\t\tReading Character in Variable Name [%c]\n", currChar);
                        Variable_Name[Variable_Name_Index] = currChar;
                        Variable_Name_Index++;
                        currChar = fgetc(infp);
                    }
                    Variable_Name[Variable_Name_Index] = '\0';
                    //printf("Current Value Field: %s\n", Variable_Name);

                    if (strcmp(Variable_Name,"ID")==0) current_Variable=0;
                    else if(strcmp(Variable_Name,"name")==0) current_Variable=1;
                    else if(strcmp(Variable_Name,"arrivalTime")==0) current_Variable=2;
                    else if(strcmp(Variable_Name,"deadlineTime")==0) current_Variable=3;
                    else if(strcmp(Variable_Name,"executionTime")==0) current_Variable=4;
                    else {
                        printf("Invalid Variable Name");
                        return NULL;
                    }

                    readingVariable_Name=0;
                }

                while(!readingVariable_Value){
                    currChar = fgetc(infp);
                    if(currChar == ':'){
                        //printf("\tReading Variable Value [%c]\n", currChar);
                        fgetc(infp);
                        readingVariable_Value = 1;
                    }
                }
                while (readingVariable_Value){
                    char *Variable_Value = malloc(MAX_FIELD_LENGTH*sizeof(char));
                    //char Variable_Value[MAX_FIELD_LENGTH]; // Assuming MAX_FIELD_LENGTH is a constant defining the maximum length of value fields
                    int Variable_Value_Index = 0;
                    currChar = fgetc(infp);
                    if(currChar=='\"')
                        currChar = fgetc(infp);
                    while(currChar != '\"' && currChar != ',' && currChar != '\n'){
                        //printf("\t\tReading Character in Variable Name [%c]\n", currChar);
                        Variable_Value[Variable_Value_Index] = currChar;
                        Variable_Value_Index++;
                        currChar = fgetc(infp);
                    }
                    Variable_Value[Variable_Value_Index] = '\0';
                    //printf("Current Value Field: %s\n", Variable_Value);

                    switch(current_Variable){
                        case 0:
                            //printf("\nINNNNNN\n");
                            currMapEntry->key = atoi(Variable_Value);
                            break;
                        case 1:
                            //currMapEntry->name = malloc(MAX_FIELD_LENGTH*sizeof(char));
                            currMapEntry->name = Variable_Value;
                            break;
                        case 2:
                            currMapEntry->arrivalTime = atoi(Variable_Value);
                            break;
                        case 3:
                            currMapEntry->deadline = atoi(Variable_Value);
                            break;
                        case 4:
                            currMapEntry->executionTime = atoi(Variable_Value);
                            break;
                    }

                    //Delay so the current character is always the comma
                    if(currChar=='\"')
                        currChar = fgetc(infp);
                    readingVariable_Value=0;
                }
                if(currChar!=','){
                    while (currChar!='}'){
                        currChar = fgetc(infp);
                    }
                    tempMap->mapData[currInput] = currMapEntry;
                    currInput++;
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
