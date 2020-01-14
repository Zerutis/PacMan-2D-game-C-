#include "header.h"
//-----------------------------------------
Results sortTheStats(Results *stats, int n){
    Results sort;
    for (int i=0; i<n-1; i++){
        for(int j=i+1; j<n; j++){
            if(stats[i].points<stats[j].points){
                sort=stats[i];
                stats[i]=stats[j];
                stats[j]=sort;
            }
        }
    }
    return *stats;
}
