#ifndef MAP_NODE_H
# define MAP_NODE_H

# include <stdbool.h>
# include "../ft_printf/libft/header/libft.h"

typedef struct s_ant t_ant;

typedef struct  s_map_node
{
    char *                  name;
    unsigned int            x;
    unsigned int            y;
    struct s_ant *          occupant;
    int                     link_amount;
    struct s_map_node **    map_links;
    struct s_list   *       map_links_ll;
    bool start;
    bool end;
    bool locked;
    bool reachable;
    int distance;
}               t_map_node;

typedef struct  s_link
{
    char * from;
    char * to;
}               t_link;

typedef struct  s_ant
{
    unsigned int id;
    struct s_map_node * location;
    bool finished;

}               t_ant;

typedef struct  s_game_info
{
    int ant_amount;
    struct s_ant * ants;

    int room_amount;
    struct s_map_node ** rooms;
    t_list * tmp_room_storage;

    int link_amount;
    t_link ** links;
    t_list * tmp_link_storage;
}               t_game_info;

typedef enum    e_line_type
{
    ANTS,
    COMMENT,
    START,
    END,
    ROOM,
    LINK,
    UNKNOWN
}               t_line_type;

// Read and parse file
t_game_info * read_and_parse_file(char * file);

int link_rooms(t_game_info * info);

#endif
