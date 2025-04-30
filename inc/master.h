#ifndef MASTER_H
#define MASTER_H

#include "map_node.h"

typedef struct  s_path
{
    t_list * path;
    int length;
}               t_path;

/* declarations */
int solve(t_game_info * info);

#endif /* MASTER_H */

