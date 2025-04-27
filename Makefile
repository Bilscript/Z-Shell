# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/27 12:26:04 by bhamani           #+#    #+#              #
#    Updated: 2025/04/27 12:26:04 by bhamani          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = Z-Shell

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror

SRC_DIR = src
OBJ_DIR = object

SRC     = \
	src/main.c \
	src/built-in/ft_cd.c \
	src/built-in/ft_echo.c \
	src/built-in/ft_env.c \
	src/built-in/ft_export.c \
	src/built-in/ft_pwd.c \
	src/env/env_utils.c \
	src/env/env_utils2.c \
	src/exec/exec.c \
	src/exec/handle_heredoc.c \
	src/exec/handle_pipe.c \
	src/exec/handle_redir.c \
	src/exec/pipe_gestion.c \
	src/exec/pipe_utils.c \
	src/parsing/free_function.c \
	src/parsing/parse_token.c \
	src/parsing/parsing.c \
	src/parsing/parsing_handler.c \
	src/parsing/token_lexer.c \
	src/parsing/token_lexer_utils.c \
	src/parsing/token_list.c \
	src/parsing/token_utils.c \
	src/parsing/token_utils2.c

OBJ     = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Libft
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Includes
INC     = -Isrc -I$(LIBFT_DIR)

# Libraries
LIBS    = -lreadline

# Rules

all: $(LIBFT) $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LIBS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re
