NAME      := lem_in

SRC_DIRS  := src get_next_line
OBJ_DIR   := obj
HDR_DIRS  := inc get_next_line ft_printf/inc ft_printf/libft/header

SRCS      := $(shell find $(SRC_DIRS) -name '*.c')
OBJS      := $(SRCS:%.c=$(OBJ_DIR)/%.o)

CC        := cc
CFLAGS    := -Wall -Wextra -Werror $(foreach dir,$(HDR_DIRS),-I$(dir))
DFLAGS    := -g

# Default rule
all: $(NAME)

# Link final binary
$(NAME): ft_printf/libftprintf.a $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) ft_printf/libftprintf.a ft_printf/libft/libft.a $(DFLAGS)

# Compile .c to .o and ensure directory exists
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ $(DFLAGS)

# Build ft_printf library
ft_printf/libftprintf.a:
	$(MAKE) -C ft_printf

debug:
	$(MAKE) -C ft_printf debug
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) ft_printf/libftprintf.a ft_printf/libft/libft.a $(DFLAGS)

# Clean object files
clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C ft_printf clean

# Clean everything
fclean: clean
	rm -f $(NAME)
	$(MAKE) -C ft_printf fclean

# Rebuild all
re: fclean all

# Debug info
print:
	@echo "Sources: $(SRCS)"
	@echo "Objects: $(OBJS)"

