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

CC      = cc
CFLAGS  = -Wall -Wextra -Werror

SRC_DIR = src
OBJ_DIR = object

SRC     = \
	src/main.c \
	src/built-in/ft_cd.c \
	src/built-in/ft_echo.c \
	src/built-in/ft_unset.c \
	src/built-in/ft_exit.c \
	src/built-in/ft_env.c \
	src/built-in/ft_export.c \
	src/built-in/ft_pwd.c \
	src/env/env_utils.c \
	src/env/env_utils2.c \
	src/exec/exec.c \
	src/exec/exec_builtin.c \
	src/exec/exec_builtin_or_real.c \
	src/exec/exec_piped_commands.c\
	src/exec/is_directory.c \
	src/exec/handle_heredoc.c \
	src/exec/handle_heredoc2.c \
	src/exec/handle_pipe.c \
	src/exec/handle_redir.c \
	src/exec/handle_std.c \
	src/exec/pipe_gestion.c \
	src/exec/pipe_utils.c \
	src/parsing/free_function.c \
	src/parsing/parse_token.c \
	src/parsing/parsing.c \
	src/parsing/handle_dollars.c \
	src/parsing/parsing_handler.c \
	src/parsing/token_lexer.c \
	src/parsing/token_lexer_utils.c \
	src/parsing/token_list.c \
	src/parsing/token_utils.c \
	src/parsing/token_utils2.c \
	src/sig_management.c

OBJ     = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

INCLUDES = ./headers

# Libft
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

# Libraries
LIBS    = -lreadline

# Rules

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -I$(INCLUDES) $(LIBS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
