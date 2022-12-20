#include "../include/ist_operations.h"


SCORELIST list;



int FindLastGame ( char *PLID , char *fname )
{
    struct dirent **filelist;
    int n_entries, found;
    char dirname[20] ;

    sprintf ( dirname , "/server/source/GAMES/%s/" ,PLID ) ;
    n_entries = scandir ( dirname , &filelist , 0 , alphasort ) ;
    found=0;
    
    if ( n_entries <= 0 )
        return ( 0 ) ;
    else
    {  
        while ( n_entries--)
        {
            if ( filelist[n_entries]->d_name[0] != '.' )
            {
                sprintf ( fname , "server/source/GAMES/%s/%s" ,PLID , filelist[n_entries]->d_name ) ;
                found = 1;
            }
            free( filelist[n_entries] ) ;
            if( found )
                break;
        }
        free( filelist ) ;
    }
    return ( found ) ;
}







void FindTopScores()
{
    struct dirent ** filelist;
    int n_entries , i_file;
    char fname[100];
    FILE * fp ;

    list = (SCORELIST)malloc(sizeof(struct score_list));

    n_entries = scandir( "server/source/SCORES/", &filelist, 0, alphasort );

    printf("n_entries = %d\n", n_entries);

    i_file = 0;
    if ( n_entries < 0 )
    {
        return ( 0 ) ;
    }
    else
    {
        while ( n_entries--)
        {
            if ( filelist[n_entries]->d_name[0] != '.' )
            {
                sprintf( fname , "server/source/SCORES/%s" , filelist[n_entries]->d_name ) ;
                fp = fopen ( fname , "r" ) ;
                if ( fp != NULL)
                {
                    fscanf( fp , "%d %s %s %d %d" ,
                        &list->score[i_file], list->PLID[i_file] , list->word[i_file] ,&list->n_succ[i_file] ,
                        &list->n_tot[i_file] ) ;
                    printf("%03d %s %s %d %d\n",list->score[i_file], list->PLID[i_file], list->word[i_file], list->n_succ[i_file], list->n_tot[i_file]);
                    fclose( fp ) ;
                    ++i_file;
                }
            }
        free( filelist[n_entries] ) ;
        if ( i_file==10)
            break;
    }
    free(filelist) ;
    }
    list->n_scores = i_file ;
}




void create_scoreboard_file(){
    FILE * fp ;
    char file_line[100];
    int i = 0;
    fp = fopen("server/source/Fname.txt", "w");
    if ( fp == NULL){  
            printf("Failed to create file\n");
            exit(EXIT_FAILURE);
    }
    else{ 
        while(i<10){
            sprintf(file_line,"%03d %s %s %d %d\n", list->score[i], list->PLID[i], list->word[i], list->n_succ[i], list->n_tot[i]);
            fputs(file_line,fp);    
            i++;
        }
        fclose(fp);
    }
}











