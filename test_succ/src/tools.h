#ifndef _TOOLS_H
#define _TOOLS_H

/*  This method designed to calculate time_difference between two timeval       --usage
*
*/
long delta_time_ms(struct timeval t_pre, struct timeval t_aft){
    return ( (t_aft.tv_sec - t_pre.tv_sec)*1000 + (t_aft.tv_usec - t_pre.tv_usec)/1000 );
}


#endif
