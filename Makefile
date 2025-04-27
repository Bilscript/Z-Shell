NAME		:= Z-Shell
SRC_DIR		:= src
OBJ_DIR		:= obj
LIBFT_DIR	:= libft
LIBFT		:= $(LIBFT_DIR)/libft.a
CC			:= gcc
CFLAGS		:= -Wall -Wextra -Werror -I$(LIBFT_DIR) -I$(SRC_DIR)
#CFLAGS		:= -g -I$(LIBFT_DIR) -I$(SRC_DIR)
LDFLAGS		:= -lreadline

SRC			:= $(wildcard $(SRC_DIR)/*.c) # PAS LE DROIT A WILDCARD
OBJ			:= $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
 
# === Rules ===

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBFT) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re 