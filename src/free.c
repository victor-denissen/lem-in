#include "../inc/map_node.h"
#include "../ft_printf/libft/header/libft.h"

void    free_map_node(void * room_src)
{
    t_map_node * room = room_src;
    free(room->name);
    free(room->map_links);
    ft_lstclear(&room->map_links_ll, NULL);
    free(room);
}


void    free_link(void * link_src)
{
    t_link * link = link_src;
    free(link->from);
    free(link->to);
    free(link);
}


void    free_game_info(t_game_info * info)
{
    if (!info)
        return;
    if (info->ants)
    {
        free(info->ants);
        info->ants = NULL;
    }

    if (info->rooms)
        free(info->rooms);
    info->rooms = NULL;

    if (info->links)
        free(info->links);

    ft_lstclear(&info->tmp_link_storage, &free_link);
    ft_lstclear(&info->tmp_room_storage, &free_map_node);

    free(info);
}
