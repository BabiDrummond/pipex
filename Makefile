NAME = pipex
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDE = -I./include/

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS_DIR = src/
SRCS = pipex.c \
		utils.c
OBJS_DIR = objs/
OBJS = $(SRCS:%.c=$(OBJS_DIR)%.o)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(OBJS) $(LIBFT) -o $(NAME)
	@echo -n "\033[0;32mGenerated pipex\n"

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
	@echo "\033[95mCompiling $(notdir $<)"

$(LIBFT):
	@make --no-print-directory -C $(LIBFT_DIR)

clean:
	@echo "\033[0;34mCleaning pipex objects"
	@rm -rf $(OBJS_DIR)
	@make --no-print-directory -C $(LIBFT_DIR) clean

fclean: clean
	@echo "\033[0;34mCleaning pipex"
	@rm -f $(NAME)
	@make --no-print-directory -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: $(LIBFT) clean fclean re