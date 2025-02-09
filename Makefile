NAME = philo

SOURCES = main.c init_data.c \

OBJ = $(SOURCES:.c=.o)
CC = cc
FLAGS = -Wall -Wextra -Werror

GREEN = \033[0;32m
RESET = \033[0m

all: $(NAME)

%.o: %.c philo.h
	$(CC) $(FLAGS) -c $< -o $@