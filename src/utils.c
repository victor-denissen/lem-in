#include "../inc/free.h"
#include "../ft_printf/inc/ft_printf.h"
#include "../inc/master.h"

void    print_ant_info(t_game_info * info)
{
    // Print ant info
    int ant_amount = info->ant_amount;
    ft_printf("ant amount: %i\n", ant_amount);
    for ( int i = 0; i < ant_amount; ++i)
    {
        t_ant ant = info->ants[i];
        if (ant.location)
            ft_printf("ant id: %i\nlocation: %s", ant.id, ant.location->name);
        else
            ft_printf("ant id: %i\nlocation: %p\n\n", ant.id, ant.location);
    }
}

void    print_room_info(t_map_node * room)
{
    ft_putendl_fd("---------------------", 1);
    ft_printf("room name: %s\ncoords: x=%i y=%i\ndistance: %i\n\n",
            room->name,
            room->x,
            room->y,
            room->distance
            );
    if (room->map_links_ll)
    {
        ft_putendl_fd("Linked to:", 1);
        for (t_list * node = room->map_links_ll;node; node = node->next)
            ft_printf("\t%s", ((t_map_node *)node->content)->name);
        ft_putendl_fd("", 1);
    }
    ft_putendl_fd("---------------------", 1);
}

void    print_room_array(t_game_info * info)
{
    //print map info
    for (int i = 0; i < info->room_amount; ++i)
        print_room_info(info->rooms[i]);
}

void    print_room_ll(t_game_info * info)
{
    //print map holder info
    t_list * m_holder = info->tmp_room_storage;
    while (m_holder)
    {
        t_map_node * room = m_holder->content;
        print_room_info(room);
        m_holder = m_holder->next;
    }

}

void    print_link_info(t_link * link)
{
    ft_putendl_fd("---------------------", 1);
        ft_printf("from: %s\nto: %s\n\n",
                link->from,
                link->to
                );
    ft_putendl_fd("---------------------", 1);
}

void    print_link_ll(t_game_info * info)
{
    //print link holder info
    t_list * l_holder = info->tmp_link_storage;
    while (l_holder)
    {
        print_link_info(l_holder->content);
        l_holder = l_holder->next;
    }
}

void    print_info(t_game_info * info)
{
    /* ft_putendl_fd("Ants", 1); */
    /* print_ant_info(info); */

    /* ft_putendl_fd("Room Arrays", 1); */
    /* print_room_array(info); */

    ft_putendl_fd("Room ll", 1);
    print_room_ll(info);

    /* ft_putendl_fd("link ll", 1); */
    /* print_link_ll(info); */
}

void    print_type(t_line_type type)
{
    if (type == ANTS)
        ft_putendl_fd("ANTS", 1);
    if (type == COMMENT)
        ft_putendl_fd("COMMENT", 1);
    if (type == START)
        ft_putendl_fd("START", 1);
    if (type == END)
        ft_putendl_fd("END", 1);
    if (type == ROOM)
        ft_putendl_fd("ROOM", 1);
    if (type == LINK)
        ft_putendl_fd("LINK", 1);
    if (type == UNKNOWN)
        ft_putendl_fd("UNKNOWN", 1);
}

void    remove_link_from_room(t_map_node * room)
{
    t_list * node = room->map_links_ll;

    while (node)
    {
        t_map_node * cur_room = node->content;
        t_list * cur_room_node = cur_room->map_links_ll;
        t_list * cur_room_prev = NULL;
        while (cur_room_node)
        {
            t_map_node * link_room = cur_room_node->content;
            if (ft_strncmp(link_room->name, room->name, ft_strlen(room->name)) == 0)
            {
                if (cur_room_prev)
                    cur_room_prev->next = cur_room_node->next;
                else
                    cur_room->map_links_ll = cur_room->map_links_ll->next;
                free(cur_room_node);
                cur_room_node = NULL;
                cur_room->link_amount--;
                break;
            }
            cur_room_prev = cur_room_node;
            cur_room_node = cur_room_node->next;
        }
        node = node->next;
    }
}

void    remove_room_ll(t_list ** head, int * room_amount, char *to_remove)
{
    t_list * node = *head;
    t_list * prev = NULL;
    t_list * l_to_remove = NULL;
    t_map_node * room = NULL;
    while (node)
    {
        room = node->content;
        if (ft_strncmp(to_remove, room->name, ft_strlen(to_remove)) == 0)
        {
            l_to_remove = node;
            break;
        }
        prev = node;
        node = node->next;
    }
    if (prev)
        prev->next = node->next;
    else
        *head = (*head)->next;
    remove_link_from_room(l_to_remove->content);
    free_map_node(l_to_remove->content);
    free(l_to_remove);
    (*room_amount)--;
}

t_map_node * find_room(t_game_info * info, char * name)
{
    t_map_node ** rooms = info->rooms;
    for (int i = 0; i < info->room_amount; ++i)
        if (ft_strncmp(name, rooms[i]->name, ft_strlen(name)) == 0)
            return rooms[i];
    ft_putendl_fd("Room not found", 2);
    return NULL;
}

t_map_node * find_room_ll(t_list * node, char * name)
{
    while (node)
    {
        t_map_node * room = node->content;
        if (ft_strncmp(name, room->name, ft_strlen(name)) == 0)
            return room;
        node = node->next;
    }
    ft_putendl_fd("Room not found", 2);
    return NULL;
}

t_map_node * get_end(t_map_node ** rooms, int max)
{
    for (int i = 0; i < max; ++i)
    {
        t_map_node * room = rooms[i];
        if (room->end)
            return room;
    }
    return NULL;
}

t_map_node * get_start(t_map_node ** rooms, int max)
{
    for (int i = 0; i < max; ++i)
    {
        t_map_node * room = rooms[i];
        if (room->start)
            return room;
    }
    return NULL;
}
t_map_node * get_end_ll(t_list * rooms)
{
    while (rooms)
    {
        t_map_node * room = rooms->content;
        if (room->end)
            return room;
        rooms = rooms->next;
    }
    return NULL;
}

t_map_node * get_start_ll(t_list * rooms)
{
    while (rooms)
    {
        t_map_node * room = rooms->content;
        if (room->start)
            return room;
        rooms = rooms->next;
    }
    return NULL;
}

void    print_path(t_path * path_src)
{
    t_list * path = path_src->path;
    for (int j = 0; path; path = path->next, j++)
    {
        t_map_node * room = path->content;
        ft_printf("room %i: %s\n", j, room->name);
    }

}

void    print_all_paths(t_list * paths)
{
    for (int i = 0;paths; paths = paths->next, i++)
    {
        ft_putendl_fd("------------------", 1);
        ft_printf("path %i\n\n", i);
        t_path * cont = paths->content;
        print_path(cont);
    }
}
