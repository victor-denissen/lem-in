define pllr
    set $node = $arg0
    while $node != 0x0
        print *((t_map_node *)($node->content))
        set $node = $node->next
    end
end
