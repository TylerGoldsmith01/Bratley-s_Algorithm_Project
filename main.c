#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define inputPath "C:\\Users\\Tyler\\Desktop\\Bratley's_Algorithm_Project\\Inputs\\input.txt"
//#define inputPath "C:\\Users\\Tyler\\Desktop\\Bratley's_Algorithm_Project\\Inputs\\input_large_shortestPath.txt"
//#define inputPath "C:\\Users\\Tyler\\Desktop\\Bratley's_Algorithm_Project\\Inputs\\input_large_shortestPath_fail.txt"
//#define inputPath "C:\\Users\\Tyler\\Desktop\\Bratley's_Algorithm_Project\\Inputs\\input_large_longestPath.txt"
//#define inputPath "C:\\Users\\Tyler\\Desktop\\Bratley's_Algorithm_Project\\Inputs\\input_large_longestPath_fail.txt"

#define MAX_FIELD_LENGTH 20

struct map *parse(FILE *);
struct eventNode *getInputFromMap(struct map *,int);
int generateChildren(struct eventNode *,struct map *,int,FILE *);
void printpath(struct eventNode *, int, FILE *);
int freeNode(struct eventNode *, int );

//struct to hold the event nodes which hold schedule information at points on the tree
struct eventNode{
    //struct eventNode ** children;
    struct eventNode * parent;
    char * name;
    int taskNum;
    int timeEnum;

    int arrivalTime;
    int deadline;
    int executionTime;
};

//map input data stored in (key,value) format and used to generate event nodes
struct mapEntry{
    int key;
    
    char * name;
    int arrivalTime;
    int deadline;
    int executionTime;

};

//Structure to hold all map input entries
struct map{
    struct mapEntry **mapData;
    int numElements;
};

int main(void){
    FILE *infp;
    infp = fopen(inputPath,"r");

    FILE *outfp;
    outfp = fopen("C:\\Users\\Tyler\\Desktop\\Bratley's_Algorithm_Project\\output.txt","w");
    struct map * inputMap = parse(infp);

    //Print input data to console
    for(int i=0;i<inputMap->numElements;i++)
        fprintf(outfp,"\nINPUT: %d,%s,%d,%d,%d\n",
    inputMap->mapData[i]->key,
    inputMap->mapData[i]->name,
    inputMap->mapData[i]->arrivalTime,
    inputMap->mapData[i]->deadline,
    inputMap->mapData[i]->executionTime);
    

   //Root node only cares about time enumerated
   struct eventNode *root = malloc(sizeof(struct eventNode));
   root->timeEnum = 0;

    //Function to recursively generate the Bratley's tree
    int result =  generateChildren(root,inputMap,0,outfp);
    if (result)
        printf("Found a successfull schedule");
    else
        printf("Could not generate a schedule from the given input events");

    //Free input data
    for(int i=0;i<inputMap->numElements;i++){
        free(inputMap->mapData[i]->name);
        free(inputMap->mapData[i]);
    }
    free(inputMap);
    fclose(infp);
    fclose(outfp);
    return 0;
}

//Parses the json files and creates map entry for each input from file
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
            printf("Reading Input \n",currChar);
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
                printf("Done Reading Input File\n\n");
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

//Simple function to use the key passed to the function to retrieve values from the tree
//And store them as a node to be placed ont he tree
struct eventNode *getInputFromMap(struct map *inputMap, int key){
    struct eventNode * tempNode = malloc(sizeof(struct eventNode));
    struct mapEntry * currmapEntry;
    //Search for the input key to select the proper element on the map
    for (int i=0;i<inputMap->numElements;i++){
        if(inputMap->mapData[i]->key == key)
            currmapEntry = inputMap->mapData[i];
    }
    tempNode->arrivalTime= currmapEntry->arrivalTime;
    tempNode->executionTime = currmapEntry->executionTime;
    tempNode->deadline = currmapEntry->deadline;
    tempNode->name = currmapEntry->name;
    tempNode->taskNum = currmapEntry->key;
    return tempNode;
}

//Climbs the tree, and prints the parent path up to root to indicate the location of node on tree
void printpath(struct eventNode *node, int level, FILE *outfp){
    struct eventNode *parent = node->parent;
    fprintf(outfp, "Current Path = [%s]<-", node->name);
    for(int l = level; l>0;l--){
        fprintf(outfp,"%s<-", parent->name);
        parent = parent->parent;
    }
}

//Recursively free the final path discovered (all other nodes are freed when the path fails)
int freeNode(struct eventNode *node, int currLevel){
    struct eventNode *parent = node->parent;
    currLevel--;
    if(currLevel!=0)
        freeNode(parent,currLevel--);
    free(node);
}

//Recursive function to generate children
int generateChildren(struct eventNode *parent,struct map *inputMap,int level, FILE *outfp){

    //Array to figure out which children need to be created based on index
    int * childrenKeys = malloc(inputMap->numElements * sizeof(int));

    struct eventNode *parentUnderAnalysis;
    parentUnderAnalysis = parent;

    struct eventNode* currChild;
    int timeEnum;

    for(int i=0; i<inputMap->numElements;i++){
        childrenKeys[i]=1;
    }

    fprintf(outfp, "\nCurrent Parent path: ");

    //climb up the tree, and set the index of the parent node to 0 in the childrenKeys list
    //to indicate that this node will not be created as a child
    for(int parent=level; parent>0; parent--){
        childrenKeys[parentUnderAnalysis->taskNum] = 0;
        fprintf(outfp,"%s->",parentUnderAnalysis->name);
        parentUnderAnalysis = parentUnderAnalysis->parent;
    }
    fprintf(outfp,"\n");

    //Check the timing of all the children before creating any children to check if branch should be pruned
    int child;
    for(child=0 ; child<inputMap->numElements ; child++){
        if(childrenKeys[child]==1){
            int executionTime = inputMap->mapData[child]->executionTime;
            int arrivalTime = inputMap->mapData[child]->arrivalTime;
            int deadline = inputMap->mapData[child]->deadline;
            int timeToRun = executionTime;

            if(parent->timeEnum < arrivalTime){
                timeToRun += (arrivalTime);
            }else{
                timeToRun += (parent->timeEnum);
            }    
            //Check if this section of the schedule is feasible...\n otherwise break the line
            if(timeToRun <= deadline){
                fprintf(outfp,"Valid Branch found at Node %s\n", inputMap->mapData[child]->name);

                //Check if the algorithm has reached the bottom of the tree (if true schedule is complete)
                if(level == inputMap->numElements-1){
                    fprintf(outfp,"\n Found a schedule that works at \n");
                    fprintf(outfp,"%s<-",inputMap->mapData[child]->name);
                    parentUnderAnalysis = parent;

                    //Print final schedule then return up
                    for(int parent=level-1; parent>=0; parent--){
                        if(parent!=0)
                            fprintf(outfp,"%s<-",parentUnderAnalysis->name);
                        else
                            fprintf(outfp,"%s<-",parentUnderAnalysis->name);
                        parentUnderAnalysis = parentUnderAnalysis->parent;
                    }
                    //freeNode(currChild,level);
                    return 1;
                }
            }
            //Break case for an invalid branch to be pruned
            else{
                fprintf(outfp,"Invalid Branch found at node %s...Prune\n",inputMap->mapData[child]->name);
                fprintf(outfp,"\n\n");
                free(childrenKeys);
                free(parent);
                return 0;
            }
        }
    }
    fprintf(outfp,"\n\n");
    //If code makes it here all children are valid (otherwise it would return 0)
    //Rerun loop to recursively generate children
    for(child=0 ; child<inputMap->numElements ; child++){
        if(childrenKeys[child]==1){
            //Create the nodes from the input map starting from the lowest event
            fprintf(outfp,"Creating child from Key %d\n", child);
            currChild = getInputFromMap(inputMap,child);
            fprintf(outfp,"Created child %s from Key %d\n", currChild->name,child);
            currChild->parent = parent;

            //Calculate the time taken to execute this child
            int timeToRun = currChild->executionTime;

            if(parent->timeEnum < currChild->arrivalTime){
                timeToRun += (currChild->arrivalTime);
            }else{
                timeToRun += (parent->timeEnum);
            }
            currChild->timeEnum = timeToRun;   

            fprintf(outfp,"%s:\n\t", currChild->name);
            printpath(currChild,level, outfp);
            fprintf(outfp,"\n\tLayer %d\n\tTime to Run: %d\n\tDeadline: %d\n",level,currChild->timeEnum, currChild->deadline);
            
            int validBranch = generateChildren(currChild, inputMap, level+1,outfp);
            if(validBranch)
                return 1;
        }
    }
    return 0;
}
