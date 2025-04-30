#ifndef FREE_H
#define FREE_H

#include "map_node.h"
#include "master.h"
/* declarations */

void    free_game_info(t_game_info * info);
void    free_map_node(void * room_src);
void    free_path(void * to_free);

#endif /* FREE_H */

