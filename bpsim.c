//
//  main.c
//  Branch Predictor Simulator
//  Project 2
//  Created by Huey Padua on 11/19/17.
//  Copyright © 2017 Huey Padua. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void initialize(int **array, int M);
int newPow(int n, int exp);

typedef struct {
    long address;
    char outcome;
    int branchVal;
}data;

int main(int argc, char * argv[]) {
    
    //command line args, used to index gshare table
    int M = atoi(argv[1]); //number of PC bits
    int N = atoi(argv[2]); //global history register bits
    char *filename = argv[3];
    /*
     //paramaters passed into simulator
     char *filename= "/Users/hueypadua/Desktop/mcf_trace.txt";
     int M=8; // # of lower part of the program branches bits
     int N=5; //# of bits in global branch history register
     */
    FILE *ifp;
    char ch;
    char outcome;
    long address;
    
    //counters for predictions/number of branches
    int mis_pred=0;
    int cor_pred=0;
    int branches=0;

    ifp=fopen(filename, "r");
    if(ifp==NULL) {
        printf("File doesn't exist\n");
        return 1;
    }
    //global buffer history initialized to 0;
    int GBH=0;
    //initializing array to hold pow(2, M), set to weakly taken 2
    int *array;
    array = malloc(sizeof(int)*(newPow(2, M)));
    for(int i=0; i<newPow(2, M); i++) {
        array[i]=2;
    }
    data gtable; // holds each elements PC address and branch value
    //loop through tracefile until there is no more content
    while((ch=fgetc(ifp))!=EOF) {
        
        //reading trace file, each line includes <PC address> <outcome>
        fscanf(ifp, "%lx %c", &address, &outcome);
        gtable.address=address;
        gtable.outcome=outcome;
        //When outcome is read as taken or not taken, branch value is set to 1 or 0
        /**if-else statement for extra credit
        int comparison is more time-efficient than string comparison*/
        if(gtable.outcome=='t') {
            gtable.branchVal=1;
        }
        else if(gtable.outcome=='n') {
            gtable.branchVal=0;
        }
        else {
            printf("Invalid prediction\n");
            exit(1);
        }
        long trimmedPC=gtable.address>>2; //removed first 2 bits of PC address
        int mgbh=GBH<<(M-N);  //extended the N-bit gbh t M bits
        int mpc=trimmedPC&((1<<M)-1); // first M bits from the [PC without the first 2 bits
        //printf("%d\n", mpc);
        int index = mgbh^mpc; // accessing the element in the array
        //printf("%d\n", index);
        // initialize state to current element in the array
        int state=array[index];
        //predicts if the element in the table is taken
        if((gtable.branchVal<<1)==(state&2)) {
            cor_pred++;
        }
        //update state, new value is taken
        if(gtable.branchVal==1) {
            //not strongly taken
            if(state!=3)
                state++;
            if(state==2||state==3) {
                //if taken, set the most significant bit in GBH
                GBH|=newPow(2,N-1);
            }
        }
        //new branch value is not taken
        else {
            //update state if not in strongly not taken
            if(state!=0) {
                state--;
            }
            else if(state==1||state==2) {
                mis_pred++;
            }
        }
        //Adding outcome that has been parsed to the GBH
        GBH>>=1; // discard least significant bit
        GBH&=(newPow(2, N-1)-1); //clearing the most significant bit in N-bit
        //update element in array to state
        array[index]=state;
        branches++;
    }
    float corrRatio = (float)cor_pred/branches*100;
    int missRate = branches - cor_pred;
    float misprediction= 100-corrRatio;
    
    //Printing output
    /*printf("Configurations: %s, M=%d, N=%d\n", filename, M, N);
    printf("Number of branch mispredictions: %d\n", missRate);
    printf("Misprediction rate = %.2f%%\n", misprediction);
    */
    //project output requirement
    printf("%d %d %.2f%%\n", M, N, misprediction);
    
    free(array);
    fclose(ifp);
    return 0;
}

//new power function to support type double
int newPow(int n, int exp) {
    int res=1;
    for(; exp>0; exp--)
        res*=n;
    
    return res;
}

