#include "../ft_printf/libft/header/libft.h"
#include "../ft_printf/inc/ft_printf.h"
#include "../inc/map_node.h"
#include "../inc/utils.h"

void    set_distance(t_map_node * room, int distance)
{
    /* ft_printf("trying to set %s: %i\n",room->name, distance); */
    if (room->distance == -1 || distance < room->distance)
    {
        room->distance = distance;
        /* ft_printf("setting %s: %i\n",room->name, distance); */
    }
    else
        return;

    for (int i = 0; i < room->link_amount; ++i)
    {
        t_map_node * link = room->map_links[i];
        set_distance(link, distance + 1);
    }
}

int set_all_distance(t_map_node ** rooms, int max)
{
    t_map_node * end = get_end(rooms, max);
    set_distance(end, 0);

    return 0;
}
