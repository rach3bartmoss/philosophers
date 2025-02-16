NAME = philo

SOURCES = main.c init_data.c utils.c routines.c clean_up.c utils2.c \
			routines_helper.c \

OBJ = $(SOURCES:.c=.o)
CC = cc
FLAGS = -Wall -Wextra -Werror -pthread

GREEN = \033[0;32m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(OBJ) -o $(NAME)
	@echo "$(GREEN)Compilation successful! Created $(NAME)$(RESET)"

%.o: %.c philo.h
	$(CC) $(FLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@echo "$(GREEN)Cleaned object files$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(GREEN)Cleaned executable$(RESET)"

re: fclean all

.PHONY: all clean fclean re