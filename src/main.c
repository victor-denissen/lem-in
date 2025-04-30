#include "../inc/map_node.h"
#include "../ft_printf/inc/ft_printf.h"
#include "../inc/utils.h"
#include "../inc/free.h"
#include "../inc/prep.h"
#include "../inc/master.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int prep(t_game_info *info)
{
    info->room_amount = ft_lstsize(info->tmp_room_storage);
    simplify(info->tmp_room_storage, &info->room_amount);


    info->rooms = (t_map_node **)ft_list_to_array(  info->tmp_room_storage,
                                                    &info->room_amount);

    for (int i = 0; i < info->room_amount; ++i)
    {
        t_map_node * room = info->rooms[i];
        room->map_links = (t_map_node **)ft_list_to_array(  room->map_links_ll,
                                                           &room->link_amount);
    }
    set_all_distance(info->rooms, info->room_amount);
    return 0;
}

int main(int argc, char **argv)
{
    // Read and process file
        // Get amount of ants
        // Read room info
        // Get room links
    // simplify map
        // Filter out dead ends
    // solution algorithm
        // Check how many input roads there are into end
        // check how many output roads there are in start
            // thats how many ants can flow at once
        // find that amount of roads with least distance


    clock_t start = clock();
    t_game_info * info;

    if (argc > 2)
        return ft_putendl_fd("Wrong amount of arguments", 2), 1;
    if (argc == 2)
        info = read_and_parse_file(argv[1]);
    else
        info = read_and_parse_file("maps/test.txt");
    if (!info)
        return 1;

    prep(info);

    solve(info);

    free_game_info(info);

    clock_t end = clock();
    double total_cycles = end - start;
    printf("time for completion: %lfs\n", total_cycles / CLOCKS_PER_SEC);

    return 0;
}
