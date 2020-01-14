#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "header.h"
#include <assert.h>

//*****************************
int startTime;
//simboliai
const char S_player = '#';
const char S_ghost = 'G';
const char S_emptySlot =' ';
const char S_wall = '*';
const char S_food = '.';
//****************************
//zemelapio didumas
const int MapX = 15;
const int MapY = 30;
//------------------------------------------
char Grid[15][30]= {         "*****************************",
                             "*                           *",
                             "*                           *",
                             "*                           *",
                             "*                           *",
                             "*                           *",
                             "*                           *",
                             "*                           *",
                             "*                           *",
                             "*                           *",
                             "*                           *",
                             "*                           *",
                             "*                           *",
                             "*****************************"
};
//-----------------------------------------
void logTime() {
    int exTime = time(0) -startTime;
    if((lFile = fopen("info.log", "a"))!=NULL){
        fprintf(lFile, "Execution time: %ds\n", exTime);
    }
    fclose(lFile);
}

void logStart() {
    if((lFile = fopen("info.log", "a"))!=NULL){
        fputs("Program started!\n", lFile);
    }
    fclose(lFile);
}
//-----------------------------------------
void createArray(Results **stats, int n){
    *(stats)=(Results*)malloc(n*sizeof(Results));
}
//-----------------------------------------
void resultsSave(Results *stats,int GameSpeed, int pts, int n){
        stats->GameMode=GameSpeed;
        stats->points=pts;
}
//-----------------------------------------
void printResults(Results *stats, int n){
    printf("         Scoreboard\n");
    printf("-----------------------------------\n");
    for (int i=0; i<n; i++){
        if(stats[i].GameMode=='h'){
            printf("%d Player has scored %d at level: %s\n", i+1,stats[i].points, "hard");
        }else if(stats[i].GameMode=='m'){
            printf("%d Player has scored %d at level: %s\n", i+1,stats[i].points, "medium");
        }else{
            printf("%d Player has scored %d at level: %s\n", i+1,stats[i].points, "easy");
        }
    }
    printf("-----------------------------------\n");
}
//-----------------------------------------
void saveToFile(Results *s, int n){
    if(createFile()){
        fwrite(&n,sizeof(int),1,pFile);
        fwrite(s,sizeof(Results),n,pFile);
        fclose(pFile);
    }
}
//-----------------------------------------
void uploadFromFile(Results **s, int *n){
    if(openFile()){
        fread(n,sizeof(int),1,pFile);
        (*s)=(Results*)malloc(*n*sizeof(Results));
        fread(*s,sizeof(Results),(*n),pFile);
        fclose(pFile);
    }
}
//------------------------------------------
bool createFile(){
    if((pFile=fopen("gameData.bin","wb+"))==NULL){
        printf("File can`t be created\n");
        return false;
    }else{
        return true;
    }
}
//------------------------------------------
bool openFile(){
    if((pFile=fopen("gameData.bin","rb"))==NULL){
        return false;
    }else{
        return true;
    }
}

//Ar norite pradėti iš naujo
//------------------------------------------
bool IsRestart(){
    char input;
    while(1){
        printf("Play again (y/n)? ");
        if((scanf("%c", &input)==1) && ((input=='y') || (input=='n')) && (getchar()=='\n')){
            if(input=='y'){
                return true;
            }else if(input=='n'){
                return false;
            }
        }else{
            while(getchar() != '\n')
                ;
            printf("-------------------------------------------------\n");
            printf("Error: Input not valid!\n");
            printf("Try again\n");
            printf("-------------------------------------------------\n");
            }
        }
}
//-----------------------------------------
void deleteFile(){
    remove("gameData.bin");
}
// Ar poziciją į kurią norite judėti yra laisva
//-----------------------------------------
bool isEmptyPosition(int x, int y){
    return ((x>=0) && (y>=0) && (x<MapX) && (y<MapY));
}
// Pastuma žaidėją į pasirinktą kryptį
//------------------------------------------
bool movePlayer(Player *p, int x, int y){
    if (!isEmptyPosition(x,y)){
        return false;
    }

    char newPosition = Grid[x][y];
    if (newPosition == S_food){
            collectDot(p);
            p->points++;
    }else if (newPosition != S_emptySlot){
        return false;
    }

    if (isEmptyPosition(x,y)){
        Grid[p->x][p->y]=S_emptySlot;
    }
    p->x=x; p->y=y;
    Grid[p->x][p->y]=S_player;
    return true;
}
// Pastuma vaiduoklį link žaidėjo
//------------------------------------------
bool moveGhost(Ghost *g, int x, int y){
    if (!isEmptyPosition(x,y)){
        return false;
    }

    char newPosition = Grid[x][y];

    if(newPosition==S_player){
        ;
    }else if(newPosition==S_food){
        ;
    }else if (newPosition != S_emptySlot){
        return false;
    }

    if (isEmptyPosition(x,y)){
        Grid[g->x][g->y]=S_emptySlot;
    }
    dropDot(g);
    g->x=x; g->y=y;
    Grid[g->x][g->y]=S_ghost;
    return true;
}
// Žaidėjo ir vaiduoklio susidurimo atvejis
//------------------------------------------
bool collision(Player *p, Ghost *g){
   if(((p->x)==(g->x))&&((p->y)==(g->y))){
        return false;
   }else{
        return true;
   }
}
//------------------------------------------
bool saveGame(){
    if(GetAsyncKeyState(0x53)){
        return false;
    }else{
        return true;
    }
}
//------------------------------------------
bool endGame(){
    if(GetAsyncKeyState(0x51)){
        return false;
    }else{
        return true;
    }
}
// Apskaičiuoja ar X koordinačių skirtumas didesnis
// už Y koordinačių skirtumą (tarp vaiduoklio ir žaidėjo)
//------------------------------------------
bool CheckIfXDifferenceBigger(Ghost *g, Player *p){
    if (abs((p->x) - (g->x)) > abs ((p->y) - (g->y))){
        return true;
    }
    return false;
}
// Vaiduoklio judėjimo principas
//-------------------------------------------------
void GhostAI(Ghost *g, Player *p){
    if (((g->x) < (p->x)) && (CheckIfXDifferenceBigger(g,p))){
        moveGhost(g,(g->x)+1,g->y);
    }else if (((g->x) > (p->x)) && (CheckIfXDifferenceBigger(g, p))){
        moveGhost(g,(g->x)-1,g->y);
    }else if (g->y < p->y){
        moveGhost(g,g->x,(g->y)+1);
    }else if (g->y > p->y){
        moveGhost(g,g->x,(g->y)-1);
    }
}
//-----------------------------------------
void dropDot(Ghost *g){
    if(Grid[g->x][g->y]==S_emptySlot){
        Grid[g->x][g->y]=S_food;
    }
}
//-----------------------------------------
void collectDot(Player *p){
    if(Grid[p->x][p->y]==S_food){
        Grid[p->x][p->y]=S_player;
    }
}
//-----------------------------------------
void showMap(){
    for (int x=0; x<MapX; x++){
            printf("%s\n", Grid[x]);
    }
}
//------------------------------------------
void cleanMap(){
    for(int x=1; x<MapX-2; x++){
        for(int y=1; y<MapY-2; y++){
            if(Grid[x][y]!=S_emptySlot){
                Grid[x][y]=S_emptySlot;
            }
        }
    }
}
void showPlayer(Player gamer){
    printf("\nPlayerX: %d\n", gamer.x);
    printf("PlayerY: %d\n", gamer.y);
    printf("Points: %d\n", gamer.points);
}
//-----------------------------------------
void gameLoop(int GameSpeed, Results *stats, int n, int *points){
    Player gamer;
    Ghost buu;
    int GhostSpeed=1; // ghost speed
    Player *p=&gamer;
    Ghost *g=&buu;
    gamer.x=-1; gamer.y=-1; gamer.dir=RIGHT; gamer.points=0;
    buu.x=1; buu.y=1;
    movePlayer(p,6,12);
    assert(movePlayer(p, 4,7));

    while(true){
        system("cls");
        showMap();
        showPlayer(gamer);
        //************************************
        //Vaiduoklio judejimas kas antra ejima
        if((GhostSpeed) && (gamer.dir!=0)){
            GhostAI(g,p);
            GhostSpeed--;
        }else{
            GhostSpeed++;
        }
        //************************************
        if (GetAsyncKeyState(0x26)){
            p->dir=UP;
        }
        else if (GetAsyncKeyState(0x28)){
            p->dir=DOWN;
        }
        else if (GetAsyncKeyState(0x25)){
            p->dir=LEFT;
        }
        else if (GetAsyncKeyState(0x27)){
            p->dir=RIGHT;
        }
        //************************************
        switch(p->dir){
            case UP:
                movePlayer(p,gamer.x-1,gamer.y);
                break;
            case DOWN:
                movePlayer(p,gamer.x+1,gamer.y);
                break;
            case LEFT:
                movePlayer(p,gamer.x,gamer.y-1);
                break;
            case RIGHT:
                movePlayer(p,gamer.x,gamer.y+1);
                break;
            default:
                p->dir=0;
        }
        Sleep(GameSpeed);
        //************************************
        //Iseinant is zaidimo
        if((!endGame()) || (!collision(p,g))){
            system("cls");
            cleanMap();
            *points=p->points;
            break;
        }
    }
}
//------------------------------------------
