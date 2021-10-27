#MAKEFLAGS += --silent

NAME = minishell
CC = gcc
CFLAGS = -g3 -lreadline
INC_DIR = includes/
INC =

SRC_DIR = src/
SRC =	main.c

OBJ_DIR = .obj/
OBJ = $(SRC:%.c=$(OBJ_DIR)%.o)

.PHONY : all clean fclean re exec

all: $(NAME) exec

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -I $(INC_DIR) -lm

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(addprefix $(INC_DIR),$(INC))
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

exec:
	./$(NAME)

norm:
	norminette $(SRC_DIR)$(SRC) $(INC_DIR)$(INC)

clean:
	/bin/rm -rf $(OBJ_DIR)

fclean: clean
	/bin/rm -f $(NAME)

re: 
	$(MAKE) fclean
	$(MAKE) all
