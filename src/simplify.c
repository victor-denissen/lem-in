#include "../ft_printf/libft/header/libft.h"
#include "../ft_printf/inc/ft_printf.h"
#include "../inc/map_node.h"
#include "../inc/utils.h"
#include <stdbool.h>

int    dead_end(t_map_node * room)
{
    if (room->start || room->end)
        return 0;
    else if (room->link_amount <= 1)
        return 1;
    else
        return 0;
}

void    simp(t_list ** head, int * room_amount)
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
            if (dead_end(room))
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

int non_locked_links(t_map_node * room)
{
    int retval = 0;;
    for (int i = 0; i < room->link_amount; ++i)
        if (!room->map_links[i]->locked)
            retval++;
    return retval;
}

void    lock_dead_ends(t_map_node ** rooms, int room_amount)
{
    bool change = true;

    while (change)
    {
        change = false;
        for (int i = 0; i < room_amount; i++)
        {
            t_map_node * room = rooms[i];
            if (room->end || room->start)
                continue;
            if (room->locked)
                continue;
            if (non_locked_links(room) < 2)
            {
                room->locked = true;
                change = true;
            }
        }
    }
}

void    mark_reachable(t_list ** rooms, t_map_node * room)
{
    if (room->reachable)
        return ;
    room->reachable = true;
    t_list * links = room->map_links_ll;
    for (; links; links = links->next)
    {
        t_map_node * link= links->content;
        t_map_node * to = find_room_ll(*rooms, link->name);
        mark_reachable(rooms, to);
    }
}

int remove_unreachable(t_list ** head, int * room_amount)
{
    t_map_node * start = get_start_ll(*head);
    mark_reachable(head, start);

    t_list * node = * head;
    while ( node)
    {
        t_map_node * room = node->content;
        node = node->next;
        if (!room->reachable)
        {
            remove_room_ll(head, room_amount, room->name);
            continue;
        }
    }
    return 0;
}

int simplify(t_list * rooms, int * room_amount)
{
    ft_printf("Initial rooms: %i\n", *room_amount);
    simp(&rooms, room_amount);
    ft_printf("Removed dead ends: %i\n", *room_amount);
    remove_unreachable(&rooms,room_amount);
    ft_printf("Removed unreachable: %i\n", *room_amount);
    return 0;
}
