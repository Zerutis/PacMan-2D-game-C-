#ifndef HEADER_H
#define HEADER_H
#include <stdio.h>
#include <stdbool.h>

//****************************
//Judejimo kryptys
#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4
//****************************
//Rezultatu lentele
typedef struct{
    int points;
    char GameMode;
}Results;
//****************************
//zaidejo ir vaiduoklio strukturos
typedef struct{
    int x, y;
    int dir;
    int points;
}Player;

typedef struct{
    int x, y;
}Ghost;
//***************************
//Failo iniciavimas programoje
FILE* pFile;
FILE* lFile;

void logStart();
void logTime();
//***************************
//Rezultatai:
void createArray(Results **stats, int n);
void resultsSave(Results *stats,int GameSpeed, int pts, int n);
void printResults(Results *stats, int n);
Results sortTheStats(Results *stats, int n);
//--------------------
//Darbas su failu:
void uploadFromFile(Results **s, int *n);
void saveToFile(Results *s, int n);
void deleteFile();
//--------------------
//Tikrinimai
bool createFile();
bool openFile();
bool IsRestart();
bool isEmptyPosition(int x, int y);
bool movePlayer(Player *p, int x, int y);
bool moveGhost(Ghost *g, int x, int y);
bool CheckIfXDifferenceBigger(Ghost *g, Player *p);
bool collision(Player *p, Ghost *g);
bool saveGame();
bool endGame();
//--------------------
//Zaidimo logika
void GhostAI(Ghost *g,Player *p);
void chasePlayer(Ghost *g, int dir, int x, int y);
void showPlayer(Player gamer);
void showMap();
void cleanMap();
void dropDot(Ghost *g);
void collectDot(Player *p);
void pointsAndTime();
//--------------------
//Visas zaidimo ciklas
void gameLoop(int GameSpeed,Results *stats, int n, int* points);

#endif // header
