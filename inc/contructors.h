#ifndef CONTRUCTORS_H
#define CONTRUCTORS_H

#include "../inc/map_node.h"
/* declarations */
t_game_info * game_info_con();
t_map_node *    map_node_con(char * str);
t_link * link_con(char * from, char * to);

#endif /* CONTRUCTORS_H */

