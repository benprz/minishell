#MAKEFLAGS += --silent

NAME = minishell
CC = gcc
CFLAGS = -g3  -fsanitize=address -Wextra -Wall #-Werror
INC_DIR = includes/
INC = minishell.h

SRC_DIR = src/
SRC =	main.c \
		parse_prompt.c \
		execute_command.c \
		utils/ft_split.c \
		utils/ft_strcmp.c \
		utils/ft_strjoin.c \
		utils/ft_strlen.c \
		utils/ft_get_path.c \
		utils/ft_strdup.c \
		utils/ft_free.c \
		utils/ft_substr.c \
		utils/ft_bzero.c \
		utils/ft_calloc.c \
		utils/ft_memset.c \
		utils/ft_strclen.c \
		utils/ft_tmp.c \
		utils/ft_strtrim.c \
		utils/ft_strndup.c \
		utils/ft_free_2d.c \
		utils/ft_strlen_2d.c \
		utils/ft_isalnum.c \
		utils/ft_isalpha.c \
		utils/ft_isdigit.c

OBJ_DIR = .obj/
OBJ = $(SRC:%.c=$(OBJ_DIR)%.o)

.PHONY : all clean fclean re exec

all: $(NAME) #exec

$(NAME): $(OBJ)
	$(CC) -lreadline $(CFLAGS) $(OBJ) -o $(NAME) -I $(INC_DIR) -lm

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(addprefix $(INC_DIR),$(INC))
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I $(INC_DIR) -c $< -o $@

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
