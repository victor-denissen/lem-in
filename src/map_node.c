#include "../inc/map_node.h"
#include "../inc/contructors.h"
#include "../ft_printf/libft/header/libft.h"
#include "../get_next_line//get_next_line.h"
#include "../ft_printf/inc/ft_printf.h"
/* #include "../inc/utils.h" */
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>

int is_room(char *line)
{
    char ** words = ft_split(line, ' ');
    if (!words)
        return -1;
    int i = 0;
    for (; words[i] != NULL; i++);
    if (--i != 2)
    {
        ft_free_char_array(words);
        return 0;
    }
    if (ft_string_is_num(words[1]) && ft_string_is_num(words[2]))
    {
        ft_free_char_array(words);
        return 1;
    }
    ft_free_char_array(words);
    return 0;
}

int is_link(char *line)
{
    char ** words = ft_split(line, '-');
    if (!words)
        return -1;
    int i = 0;
    for (; words[i] != NULL; i++);
    ft_free_char_array(words);
    if (--i != 1)
        return 0;
    return 1;

}

t_line_type get_line_type(char *line)
{
    if (ft_starts_with(line, "##start"))
        return START;
    if (ft_starts_with(line, "##end"))
        return END;
    if (ft_starts_with(line, "#"))
        return COMMENT;
    if (ft_string_is_num(line))
        return ANTS;
    if (is_room(line))
        return ROOM;
    if (is_link(line))
        return LINK;

    return UNKNOWN;
}

int generate_ants(t_game_info * info, int ants)
{
    if (info->ants)
    {
        ft_putendl_fd("Ants already exist", 2);
        return (1);
    }
    if (ants == 0)
    {
        ft_putendl_fd("No ants in colony", 2);
        return (1);
    }
    info->ant_amount = ants;
    info->ants = (t_ant *)malloc(ants * sizeof(t_ant));
    if (!info->ants)
        return 1;
    for (int i = 0; i < ants; ++i)
    {
        info->ants[i].id = i;
        info->ants[i].location = NULL;
        info->ants[i].finished = false;
    }
    return 0;
}

int generate_room_holder(t_game_info * info, char * line, int start, int end)
{
    t_map_node * new = map_node_con(line);
    if (!new)
        return 1;
    if (start == 1)
        new->start = start;
    else
        new->start = 0;
    if (end == 1)
        new->end = end;
    else
        new->end = 0;
    t_list * new_holder = (t_list *)malloc(sizeof(t_list));
    if (!new_holder)
    {
        free(new);
        return 1;
    }
    new_holder->content = new;
    new_holder->next = NULL;
    if (!info->tmp_room_storage)
        info->tmp_room_storage = new_holder;
    else
    {
        t_list * current =  info->tmp_room_storage;
        while (current->next != NULL)
            current = current->next;
        current->next = new_holder;
    }
    return 0;
}

int generate_link_holder(t_game_info * info, char * line)
{
    char ** words = ft_split(line, '-');
    if (!words)
        return 1;

    t_link * new_link = link_con(words[0], words[1]);
    if (!new_link)
    {
        // TODO failure
    }
    t_list * new_node = ft_lstnew(new_link);
    if (!new_node)
    {
        // TODO failure
    }


    ft_lstadd_back(&info->tmp_link_storage, new_node);

    ft_free_char_array(words);
    return 0;
}

int     parse_line(char * line, t_game_info * info)
{
    static int start = 0;
    static int end = 0;
    t_line_type type = get_line_type(line);
    if (start == 1 || end == 1)
        if (type != ROOM)
        {
            ft_putendl_fd("Modifier not on room", 2);
            return 1;
        }
    switch (type)
    {
        case ANTS:
            if (generate_ants(info, ft_atoi(line)))
                return 1;
            break;
        case COMMENT:
            break;
        case START:
            if (start++ > 0) return 1;
            break;
        case END:
            if (end++ > 0)return 1;
            break;
        case ROOM:
            if (generate_room_holder(info, line, start, end))
                return 1;
            if (start)
                start++;
            if (end)
                end++;
            break;
        case LINK:
            if (generate_link_holder(info, line))
                return 1;
            break;

        default:
            ft_putendl_fd("Error: unknown line type", 2);
            return 1;
    }
    return 0;
}

t_game_info * read_and_parse_file(char * file)
{
    int fd = open(file, O_RDONLY);
    if (fd == -1)
        return ft_printf("Could not open %s\n", file), NULL;

    t_game_info * info = game_info_con();
    if (!info)
        return NULL;

    char * line = get_next_line(fd, 0);
    while (line)
    {
        line[ft_strlen(line) - 1] = '\0';
        parse_line(line, info);  // TODO check for failure
        free(line);
        line = get_next_line(fd, 0);
    }
    /* info->rooms = (t_map_node **)ft_list_to_array(info->tmp_room_storage, */
    /*         &info->room_amount); */
    info->links = (t_link **)ft_list_to_array(info->tmp_link_storage,
            &info->link_amount);
    // TODO Check failure
    link_rooms(info);
    return info;
}
