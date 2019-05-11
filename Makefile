# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ffoissey <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/05 18:25:23 by ffoissey          #+#    #+#              #
#    Updated: 2019/02/27 18:59:31 by ffoissey         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
FILE = minishell tool_minishell echo env env_extend cd list list_extend        \
	   expansion process_out
LIB = libft/libft.a
SRCS = $(patsubst %,srcs/ft_%.c,$(FILE))
OBJS = $(patsubst %,objs/ft_%.o,$(FILE))
DIR_OBJS = objs/
INCLUDES += -I includes/
INCLUDES += -I libft/includes/
INCLUDES += -I libft/includes/ft_printf
INCLUDES += -I libft/includes/get_next_line
HEAD = includes/minishell.h
CFLAGS = -Wall -Werror -Wextra
CC = clang

all: $(NAME)

$(NAME): $(DIR_OBJS) $(OBJS) $(LIB)
	$(CC) $(CFLAGS) $(OBJS) $(LIB) $(INCLUDES) -o $@

$(DIR_OBJS):
	mkdir objs

$(OBJS): objs/%.o: srcs/%.c $(HEAD)
	$(CC) $(CFLAGS) -c $< $(INCLUDES) -o $@

$(LIB): FORCE
	make -C libft/

clean:
	make clean -C libft/
	rm -Rf $(OBJS) $(DIR_OBJS)

fclean: clean
	make fclean -C libft/
	rm -f $(NAME)

re: fclean all

FORCE:

.PHONY: clean fclean re all
