#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Artifact_struct {
    char name[20];
    int weight;
    int value;
} Artifact;


int totalWeight(Artifact* allArts, int numArts) {
    int tot = 0;
    for (int i = 0; i < numArts; ++i) {
        tot += allArts[i].weight;
    }
    return tot;
}


int totalValue(Artifact* allArts, int numArts) {

    // TODO: write the totalValue() function
    int tot = 0;
    for (int i = 0; i < numArts; ++i) {
        tot += allArts[i].value;
    }
    return tot;

    return 0; //modify this
}


int maxValueFromHere(Artifact* allArts, int numArts, int here, int weightAllow) {

    // TODO: write the maxValueFromHere() function

    if (here == numArts){
        return 0;
    }
    if(weightAllow < 0){
        return 0;
    }


    int incVal = 0;
    int excVal = 0;
    if(weightAllow >= allArts[here].weight){
        incVal = allArts[here].value + maxValueFromHere(allArts, numArts, here + 1, weightAllow - allArts[here].weight);
    }

    excVal = maxValueFromHere(allArts, numArts, here + 1, weightAllow);

    if(incVal > excVal){
        return incVal;
    }else{
        return excVal;
    }
}


int maxValueFromStart(Artifact* allArts, int numArts, int weightAllow) {
    return maxValueFromHere( allArts, numArts, 0, weightAllow);
}


int maxValueFromHereFast(Artifact* allArts, int numArts, int here, int weightAllow, int** memo) {

    // TODO: write the maxValueFromHereFast() function;
    //          begin by copy/pasting your code fully 
    //          functioning code from maxValueFromHere(),
    //          and make the required memoization additions 
    if(here == numArts){
        return 0;
    }
    if(weightAllow < 0){
        return 0;
    }

    if(memo[here][weightAllow] != -1){
        return memo[here][weightAllow];
    }

    int incVal = 0;
    int excVal = 0;

    if(weightAllow >= allArts[here].weight){
        incVal = allArts[here].value + maxValueFromHereFast(allArts, numArts, here + 1, weightAllow - allArts[here].weight, memo);
    }

    excVal = maxValueFromHereFast(allArts, numArts, here + 1, weightAllow, memo);

    if( incVal > excVal){
        memo[here][weightAllow] = incVal;
    } else { 
        memo[here][weightAllow] = excVal;
    }

    return memo[here][weightAllow];

}


int maxValueFromStartFast(Artifact* allArts, int numArts, int weightAllow) {
    int** memo = (int**)malloc((numArts+1)*sizeof(int*));
    for (int i = 0; i < numArts+1; ++i ) {
        memo[i] = (int*)malloc((weightAllow+1)*sizeof(int));
        for (int j = 0; j < weightAllow+1; ++j ) {
            memo[i][j] = -1;
        }
    } 
    int res = maxValueFromHereFast( allArts, numArts, 0, weightAllow,memo);

    //  TODO:   we are done with the heap-allocated 2D-array [memo] and 
    //          it is about to go out of scope when we return from this fuction, 
    //          so, free up ALL memory allocated for [memo] 
    for(int i = 0; i < numArts+1; i++){
        free(memo[i]);
    }
    free(memo);
    
    return res; 
}


//---------------------------------------------------------------------
// DO NOT MAKE ANY CHANGES BETWEEN THIS POINT AND THE END OF main()
//---------------------------------------------------------------------
// readCollectionFile() reads data from a file to build an array of 
//                      Artifact structs, which is an array pointer
//                      that is passed-by-pointer (i.e. double ptr);
//                      returns the size of the Artifact array
int readCollectionFile(int collectionNum, Artifact** allArtifacts) {
    int numArtifacts;
    char filename[20] = "collection";
    char appendThis[10];
    sprintf(appendThis,"%d.txt",collectionNum);
    strcat(filename,appendThis);

    FILE* myfile = fopen(filename,"r");
    if (myfile == NULL) {
        printf("File %s not found.\n",filename);
        return -1;
    }

    fscanf(myfile,"%d",&numArtifacts);
    *allArtifacts = (Artifact*)malloc(numArtifacts*sizeof(Artifact));
    for (int i = 0; i < numArtifacts; i++) {
        fscanf(myfile, "%s", (*allArtifacts)[i].name);
        fscanf(myfile, "%d", &(*allArtifacts)[i].weight);
        fscanf(myfile, "%d", &(*allArtifacts)[i].value);
    }
    fclose(myfile);

    return numArtifacts;
}

// a helpful function to print out all subitems of an Artifact
// using a neatly formatted, single-line output
void printArtifacts(Artifact* allArts, int numArts) {
    for (int i = 0; i < numArts; ++i) {
        printf("  %s has weight = %d and value = %d\n",allArts[i].name,allArts[i].weight,allArts[i].value);
    }
}


int main() {

    int numArtifacts; // number of Artifacts in the array built from a data file
    int weightLimit; // weight limit set by user-input

    // user selects which file to read data from (1-4)
    int collectionNum = 0; 
    while (collectionNum != 1 && collectionNum != 2 && collectionNum != 3 && collectionNum != 4) {
        printf("Enter the collection number (1, 2, 3, or 4):\n");
        scanf("%d",&collectionNum);
    } 
    
    // user enters weight limit of the backpack
    printf("Enter the backpack weight limit:\n");
    scanf("%d",&weightLimit);

    // building the Artifact array built from data in a collection#.txt file
    Artifact* allArtifacts;
    numArtifacts = readCollectionFile(collectionNum, &allArtifacts);

    // printing out the full list of artifacts, plus the total weight & value 
    printf("Full list of artifacts:\n");
    printArtifacts(allArtifacts,numArtifacts);
    printf("Total weight of artifacts: %d\n",totalWeight(allArtifacts,numArtifacts));
    printf("Total value of artifacts: %d\n",totalValue(allArtifacts,numArtifacts));
    
    // find and print the maximum possible value of any subset of 
    // Artifacts that stay under the weight limit of the backpack...
    // ... first, using the brute-force slow algorithm, which is too
    //     slow for larger collections, like collection4.txt    
    if (collectionNum == 1 || collectionNum == 2 || collectionNum == 3) {
        printf("Maximum possible value (slow mode) = $");
        printf("%d\n",maxValueFromStart( allArtifacts, numArtifacts, weightLimit));
    }  

    // ... then, using the fast algorithm (with memoization)
    printf("Maximum possible value (fast mode) = $");
    printf("%d\n",maxValueFromStartFast( allArtifacts, numArtifacts, weightLimit));
 
    // ... 
    printf("Artifacts to put in backpack (weight capacity = %d): ??? \n", weightLimit);    
    //---------------------------------------------------------------------
    // TODO: ADD TO main() here if you are on the last step of the coding  
    //              exercise, which is an extension to find and print the
    //              list of artifacts that should go into the backpack to
    //              maximize value while not going over the weight limit;
    //              the maximum value was already found above but extending
    //              that algorithm to also save the subset of artifacts may
    //              require an additional struct and modification to the 
    //              recursive funciton.
    //---------------------------------------------------------------------

    free(allArtifacts);
    return 0;
}
