#include "../inc/map_node.h"
#include "../inc/utils.h"
#include "../inc/master.h"
#include "../inc/free.h"
#include "../ft_printf/inc/ft_printf.h"


int get_max_flow(t_map_node * start, t_map_node * end)
{
    int max_flow_start = start->link_amount;
    int max_flow_end = end->link_amount;
    if (max_flow_end < max_flow_start)
        return max_flow_end;
    return max_flow_start;
}

t_path * copy_path(t_path * src)
{
    // TODO check failure
    t_path * copy = (t_path *)malloc(sizeof(t_path));
    if (!copy)
        return NULL;
    copy->length = src->length;
    copy->path = NULL;
    for (t_list * node = src->path; node; node = node->next)
        ft_lstadd_back(&copy->path, ft_lstnew(node->content));
    return copy;
}

bool    room_in_list(t_list * list, t_map_node * room)
{
    while(list)
    {
        if (((t_map_node *)list->content) == room)
            return true;
        list = list->next;
    }
    return false;
}

bool    room_in_any_path(t_list *paths, t_map_node * room)
{
    if (!room || room->end)
        return false;
    while(paths)
    {
        t_path * path = paths->content;
        if (room_in_list(path->path, room))
            return true;
        paths = paths->next;
    }
    return false;
}

t_map_node * get_cur_shortest(t_list ** paths, t_map_node * room)
{
    t_map_node * current = NULL;
    for (int i = 0; i < room->link_amount; i++)
    {
        if (room_in_any_path(*paths, room->map_links[i]))
            continue;
        if (room->map_links[i]->locked)
            continue;
        if (!current || room->map_links[i]->distance < current->distance)
            current = room->map_links[i];
    }
    return current;
}

void    get_paths(t_list ** head, t_path * cur,t_map_node * room)
{
    t_path *src_copy = copy_path(cur);
    bool create = false;
    if (room->end)
    {
        ft_putendl_fd("\nFound full path:", 1);
        print_path(cur);
        return free_path(src_copy);
    }
    for(int i = 0; i < room->link_amount; ++i)
    {
        t_map_node * link = room->map_links[i];
        if (!room_in_list(cur->path, link))
        {
            if (!create)
            {
                create = true;
                ft_lstadd_back(&cur->path, ft_lstnew(link));
                cur->length++;
                get_paths(head, cur, link);
            }
            else
            {
                t_path * new = copy_path(src_copy);
                new->length++;
                ft_lstadd_back(&new->path, ft_lstnew(link));
                ft_lstadd_back(head, ft_lstnew(new));
                get_paths(head, new, link);
            }
        }
    }
    free_path(src_copy);
}

void    get_all_paths(t_list ** paths, t_map_node * start)
{
    t_path * null = (t_path *)malloc(sizeof(t_path));
    null->path = NULL;
    ft_lstadd_back(&null->path, ft_lstnew(start));
    null->length = 0;
    * paths = ft_lstnew(null);

    get_paths(paths, null, start);

}

void    get_shortest_paths(t_list ** paths, t_map_node * start, int max_flow, t_game_info * info)
{
    for (int i = 0; i < max_flow; ++i)
    {
        t_path * new = (t_path *)malloc(sizeof(t_path));
        new->path = NULL;
        t_map_node * cur = start;
        ft_lstadd_back(&new->path, ft_lstnew(cur));
        while (!cur->end)
        {
            cur = get_cur_shortest(paths, cur);
            if (!cur)
            {
                ft_lstclear(&new->path, NULL);
                cur = start;
                continue;
            }
            if (!cur->end && !cur->start)
                cur->locked = true;
            ft_lstadd_back(&new->path, ft_lstnew(cur));
        }
        lock_dead_ends(info->rooms,info->room_amount);
        ft_lstadd_back(paths, ft_lstnew(new));
    }
}

int solve(t_game_info * info)
{
    // Check in and ouptut flow, determine max flow
    t_map_node * start = get_start(info->rooms, info->room_amount);
    t_map_node * end = get_end(info->rooms, info->room_amount);
    int max_flow = get_max_flow(start, end);
    ft_printf("max_flow: %i\n", max_flow);


    t_list * paths = NULL;
    /* get_all_paths(&paths, start); */
    get_shortest_paths(&paths, start, max_flow, info);

    print_all_paths(paths);
    ft_lstclear(&paths, free_path);
    // find shortest path/s depending on max flow

    return 0;
}
