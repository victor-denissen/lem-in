#include "../inc/map_node.h"
#include "../inc/utils.h"
#include "../ft_printf/libft/header/libft.h"

int tmp(char * r_name, char * from, char * to, t_game_info * info, t_list **l)
{
    if (ft_strncmp(r_name, from, ft_strlen(r_name)) == 0)
    {
        if (ft_strncmp(r_name, to, ft_strlen(r_name)) == 0)
            return  ft_putendl_fd("Cant link to self", 2), 1;
        t_map_node * r_to_link = find_room_ll(info->tmp_room_storage, to);
        if (!r_to_link)
            return 1;
        ft_lstadd_back(l, ft_lstnew(r_to_link));
    }
    return 0;
}

int insert_all_link(t_map_node * room, t_game_info * info)
{ // TODO check for failures
    t_link ** links = info->links;
    int link_amount = info->link_amount;
    t_list * list = NULL;
    for (int i = 0; i < link_amount; ++i)
    {
        t_link * cur = links[i];
        if (tmp(room->name, cur->to, cur->from, info, &list))
        {
        }
        if (tmp(room->name, cur->from, cur->to, info, &list))
        {
        }
    }
    room->map_links_ll = list;
    room->link_amount = ft_lstsize(list);
    return (0);
}

int link_rooms(t_game_info * info)
{
    t_list * node = info->tmp_room_storage;
    while (node)
    {
        t_map_node * room = node->content;
        insert_all_link(room, info);
        node = node->next;
    }
    return 0;
}
