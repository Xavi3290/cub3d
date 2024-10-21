# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: xroca-pe <xroca-pe@student.42barcel>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/21 18:43:43 by xroca-pe          #+#    #+#              #
#    Updated: 2024/10/21 20:33:33 by xroca-pe         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d
CC = cc
CFLAGS = -Wall -Werror -Wextra -I$(MLX_DIR)/include -fsanitize=address -g
RM = rm -f
MLX_DIR = MLX42
MLX = $(MLX_DIR)/libmlx42.a
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
LIBRARY = $(LIBFT) $(MLX) -ldl -lglfw -pthread -lm
LIBFT_MAKE = make --no-print-directory -C $(LIBFT_DIR)/

GREEN=\033[32m
LGREEN=\033[1;92m
ORANGE=\033[33m
RED = \033[1;91m
NONE=\033[0m

BUILD_DIR = build
SRC = src/main.c 

OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)
DEPS = $(OBJ:.o=.d)

all: libft mlx $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBRARY)
	@echo "\n$(LGREEN)Executable $(NAME) created ✔$(NONE)\n"

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -MMD -c $< -o $@
	@echo "$(GREEN)Compiled $< ✔$(NONE)"

-include $(DEPS)

libft:
	@echo "$(ORANGE)Compiling libft...$(NONE)"
	@$(LIBFT_MAKE)

mlx:
	@echo "$(ORANGE)Compiling MLX42...$(NONE)"
	@make -C $(MLX_DIR)

clean:
	@$(RM) -r $(BUILD_DIR)
	@$(LIBFT_MAKE) clean
	@make -C $(MLX_DIR) clean
	@echo "$(RED)Deleted object files ✔$(NONE)"

fclean: clean
	@$(RM) $(NAME)
	@$(LIBFT_MAKE) fclean
	@make -C $(MLX_DIR) clean
	@echo "$(RED)Executable $(NAME) deleted ✔$(NONE)"

re: fclean all

.PHONY: all clean fclean re libft mlx