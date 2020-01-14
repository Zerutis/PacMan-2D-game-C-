#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "header.h"
#include <time.h>

int startTime;

int main(){
    startTime=time(NULL);
    logStart();
    atexit(logTime);
    char GameSpeed;
    Results *stats=NULL;
    int n, score;
    if(!openFile()){
        assert(!openFile());
        n=0;
    }else{
        uploadFromFile(&stats,&n);
    }
    printf("Keyboard arrows are for control.\n");
    printf("Collect the dots that ghosts 'G' drops.\n");
    while(1){
        printf("Select the game level hard-medium-easy (h/m/e): ");
        if((scanf("%c",&GameSpeed)==1) && ((GameSpeed=='h') || (GameSpeed=='m') || (GameSpeed=='e')) && (getchar()=='\n')){
            if(GameSpeed=='h'){
                gameLoop(15,stats,n,&score);
            }else if(GameSpeed=='m'){
                gameLoop(30,stats,n,&score);
            }else if(GameSpeed=='e'){
                gameLoop(60,stats,n,&score);
            }
            if (n==0){
                n++;
                createArray(&stats,n);
            }else{
                n++;
                stats=(Results*)realloc(stats,n*sizeof(Results));
            }
            resultsSave(&(stats[n-1]),GameSpeed,score,n);
            *stats=sortTheStats(stats,n);
            if(n>3){
                assert(stats[1].points>stats[2].points);
            }
            printResults(stats,n);
            if(IsRestart()!=1){
                break;
            }
            printf("-------------------------------------------------\n");

        }else{
            while (getchar() != '\n');
            printf("-------------------------------------------------\n");
            printf("      Error: not one letter or even a letter.\n");
            printf("                    Try again\n");
            printf("-------------------------------------------------\n");
        }
    }
    saveToFile(stats,n);
    free(stats);
    return 0;
}



