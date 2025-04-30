#ifndef UTILS_H
# define UTILS_H

#include "map_node.h"
#include "master.h"

void    print_info(t_game_info * info);
void    print_ant_info(t_game_info * info);
void    print_room_array(t_game_info * info);
void    print_room_ll(t_game_info * info);
void    print_link_ll(t_game_info * info);
void    print_all_paths(t_list * paths);
void    print_path(t_path * path_src);

t_map_node * find_room(t_game_info * info, char * name);
t_map_node * find_room_ll(t_list * node, char * name);
void    remove_room_ll(t_list ** head, int * room_amount, char *to_remove);

t_map_node * get_end(t_map_node ** rooms, int max);
t_map_node * get_start(t_map_node ** rooms, int max);
t_map_node * get_start_ll(t_list * rooms);
t_map_node * get_end_ll(t_list * rooms);

int tmp_link_to_perm_link(t_game_info * info);
int tmp_room_to_perm_room(t_game_info * info);

void    print_type(t_line_type type);

#endif
