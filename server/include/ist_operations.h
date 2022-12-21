#ifndef IST_OPERATIONS_H
#define IST_OPERATIONS_H

#include "server.h"
#include "../../common/network_common.h"
#include <dirent.h>


#define MAX_SCORES 100

typedef struct score_list{

        int score[10];
        char PLID[10][7];
        char word[10][31];
        int n_succ[10];
        int n_tot[10];
        int n_scores;

}*SCORELIST;

int FindLastGame( char *PLID , char *fname );
int FindTopScores();
char *create_scoreboard_file();
void put_empty_line(int line_size, FILE *fp);

#endif