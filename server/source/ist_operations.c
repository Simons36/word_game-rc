#include "../include/ist_operations.h"

int FindLastGame ( char *PLID , char *fname )
{
    struct dirent **filelist;
    int n_entries, found;
    char dirname[50] ;

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
                sprintf ( fname , "/server/source/GAMES/%s/%s" ,PLID , filelist[n_entries]->d_name ) ;
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
