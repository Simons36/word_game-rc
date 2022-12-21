#include "../include/ist_operations.h"


SCORELIST list;



int FindLastGame (char *PLID, char *fname){
    struct dirent **filelist;
    int n_entries, found;
    char dirname[50] ;

    sprintf(dirname, "/server/source/GAMES/%s/", PLID);
    n_entries = scandir(dirname, &filelist,0, alphasort);
    found = 0;
    
    if(n_entries <= 0)
        return 0;
    else{  
        while(n_entries--){
            if(filelist[n_entries]->d_name[0] != '.'){
                sprintf(fname, "server/source/GAMES/%s/%s", PLID, filelist[n_entries]->d_name);
                found = 1;
            }
            free(filelist[n_entries]);
            if(found)
                break;
        }
        free(filelist) ;
    }
    return(found) ;
}







int FindTopScores(){
    struct dirent ** filelist;
    int n_entries , i_file;
    char fname[277];
    FILE * fp ;

    list = (SCORELIST)malloc(sizeof(struct score_list));

    n_entries = scandir("server/source/SCORES/", &filelist, 0, alphasort);

    printf("n_entries = %d\n", n_entries);

    i_file = 0;
    if (n_entries < 0){
        return 0;
    }
    else{
        while(n_entries--){

            if (filelist[n_entries]->d_name[0] != '.'){
                sprintf(fname , "server/source/SCORES/%s", filelist[n_entries]->d_name);
                fp = fopen(fname, "r");

                if(fp != NULL){
                    fscanf(fp , "%d %s %s %d %d" ,
                        &list->score[i_file], list->PLID[i_file] , list->word[i_file] ,&list->n_succ[i_file] ,
                        &list->n_tot[i_file]);
                    printf("%03d %s %s %d %d\n",list->score[i_file], list->PLID[i_file], list->word[i_file], list->n_succ[i_file], list->n_tot[i_file]);
                    fclose(fp);
                    ++i_file;
                }
            }

            free(filelist[n_entries]);
            if(i_file == 10) break;
        }
        free(filelist);
    }

    list->n_scores = i_file;
    return i_file;
}




char *create_scoreboard_file(){
    char *path = (char*)malloc(sizeof(char) * 50);
    strcpy(path, "server/source/scoreboard/scoreboard.txt");
    FILE * fp ;
    char file_line[100] = "";
    int i = 0;
    fp = fopen(path, "w");
    if(fp == NULL){  
        printf("Failed to create file\n");
        exit(EXIT_FAILURE);
    }
    else{
        char top_line[90];
        sprintf(top_line, "-------------------------------- TOP %d SCORES --------------------------------\n", list->n_scores);
        fputs(top_line, fp);

        int line_size = strlen(top_line) - 1;

        put_empty_line(line_size, fp);

        strcpy(top_line,"    SCORE PLAYER     WORD                             GOOD TRIALS  TOTAL TRIALS\n");
        fputs(top_line, fp);

        put_empty_line(line_size, fp);

        printf("%d\n", list->n_scores);
        while(i < list->n_scores){
            file_line[0] = '\0';
            if(i != 10 - 1 ){
                strcat(file_line, " ");
            }
            printf("i3uibfew\n");
            char temp_string[90] = "";
            char temp_word[41] = "";
            strcpy(temp_word, list->word[i]);
            for(int k = 0; k < 40 - strlen(list->word[i]); k++) strcat(temp_word, " ");

            int plid;
            sscanf(list->PLID[i], "%d", &plid);

            sprintf(temp_string,"%d - %03d  %06d  %s%d             %d\n",
                                                                        i + 1,
                                                                        list->score[i], 
                                                                        plid, 
                                                                        temp_word, 
                                                                        list->n_succ[i], 
                                                                        list->n_tot[i]);

            strcat(file_line, temp_string);
            printf("%s", file_line);
            fputs(file_line,fp);    
            i++;
        }
        fclose(fp);
    }
    printf("path: %s\n", path);
    return path;
}

void put_empty_line(int line_size, FILE *fp){
    char top_line[90] = "";

    for(int k = 0; k < line_size; k++) strcat(top_line, " ");
    strcat(top_line, "\n");

    fputs(top_line, fp);
}











