#include "../inc/map_node.h"

t_game_info * game_info_con()
{
    t_game_info * info = (t_game_info *)malloc(sizeof(t_game_info));
    if (!info)
        return NULL;
    info->ants = NULL;
    info->ant_amount = 0;

    info->rooms = NULL;
    info->room_amount = 0;
    info->tmp_room_storage = NULL;

    info->link_amount = 0;
    info->links = NULL;
    info->tmp_link_storage = NULL;
    return info;
}

t_map_node *    map_node_con(char * str)
{
    t_map_node * new = (t_map_node *)malloc(sizeof(t_map_node));

    if (!new)
        return NULL;

    char ** words = ft_split(str, ' ');
    if (0)  // TODO check if input valid
    {
        ft_free_char_array(words);
        free(new);
        return NULL;
    }
    new->name = ft_strdup(words[0]);
    new->x = ft_atoi(words[1]);
    new->y = ft_atoi(words[2]);
    new->occupant = NULL;
    new->map_links = NULL;
    new->end = false;
    new->start = false;
    new->distance = -1;
    ft_free_char_array(words);
    return new;
}

t_link * link_con(char * from, char * to)
{
    t_link * new = (t_link *)malloc(sizeof(t_link));
    if (!new)
        return NULL;
    new->from = ft_strdup(from);
    new->to = ft_strdup(to);
    return new;
}
