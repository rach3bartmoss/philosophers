NAME = philo

SRC_DIR = srcs
INC_DIR = includes
OBJ_DIR = objs

SOURCES = main.c init_data.c utils.c routines.c clean_up.c \
			routines_helper.c monitor.c main_helper.c utils2.c \

SRC = $(addprefix $(SRC_DIR)/, $(SOURCES))
OBJ = $(addprefix $(OBJ_DIR)/, $(SOURCES:.c=.o))
CC = cc
FLAGS = -g -Wall -Wextra -Werror -pthread -I $(INC_DIR)
GREEN = \033[0;32m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)
	@echo "$(GREEN)Compilation successful! Created $(NAME)$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/philo.h | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(GREEN)Cleaned object files$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(GREEN)Cleaned executable$(RESET)"

re: fclean all

.PHONY: all clean fclean re
