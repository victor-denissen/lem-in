#include "../ft_printf/libft/header/libft.h"
#include "../inc/map_node.h"
#include "../inc/utils.h"
#include <stdbool.h>

void    remove_dead_ends(t_list ** head, int * room_amount)
{
    bool change = true;

    while (change)
    {
        change = false;

        t_list * node = *head;
        t_list * prev = NULL;
        while (node)
        {
            t_map_node * room = node->content;
            if (room->start || room->end);
            else if (room->link_amount <= 1)
            {
                change = true;
                remove_room_ll(head, room_amount, room->name);
                node = prev->next;
                continue;
            }
            prev = node;
            node = node->next;
        }
    }
}

int simplify(t_list * rooms, int * room_amount)
{
    remove_dead_ends(&rooms, room_amount);
    return 0;
}
