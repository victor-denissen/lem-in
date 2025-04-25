#include "../inc/map_node.h"
#include "../ft_printf/inc/ft_printf.h"
#include "../inc/utils.h"
#include "../inc/free.h"
#include "../inc/simplify.h"
#include "../inc/distance.h"
int main()
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


    t_game_info * info = read_and_parse_file("flow-one.txt");
    /* t_game_info * info = read_and_parse_file("test.txt"); */
    ft_printf("current nu of rooms: %i\n", ft_lstsize(info->tmp_room_storage));
    simplify(info->tmp_room_storage, &info->room_amount);
    ft_printf("after simplify: %i\n", ft_lstsize(info->tmp_room_storage));
    set_all_distance(info->tmp_room_storage);
    print_info(info);
    free_game_info(info);

    return 0;
}
