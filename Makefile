NAME = ft_traceroute

CC = gcc
CFLAGS = -Wall -Wextra -Werror #-fsanitize=address -g

SRC = $(wildcard src/*.c)
SRC_LIST = $(notdir $(SRC))

OBJ_LIST = $(SRC_LIST:%.c=%.o)
OBJ_DIR = obj/
OBJ = $(addprefix obj/, $(OBJ_LIST))

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
