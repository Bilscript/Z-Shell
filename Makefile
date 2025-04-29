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
	src/built-in/ft_unset.c \
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

INCLUDES = ./headers

# Libft
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

# Configuration pour Mac ARM64
READLINE_DIR = /opt/homebrew/opt/readline
ARCH_FLAGS = -arch arm64

CFLAGS  = -Wall -Wextra -Werror $(ARCH_FLAGS) -I$(READLINE_DIR)/include -I./headers
LDFLAGS = -L$(READLINE_DIR)/lib $(ARCH_FLAGS)
LIBS    = -lreadline

# Compilation rules

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LDFLAGS) $(LIBS) -o $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR) CFLAGS="$(ARCH_FLAGS)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
