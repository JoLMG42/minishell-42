NAME	:= minishell
CC		:= cc
CFLAGS	:= -Wall -Wextra -Werror -I ./includes -g3
LIBS =	-lreadline libft/libft.a
SRCS	=	srcs/parsing/expander.c \
			srcs/parsing/parsing.c \
			srcs/utils/ft_split_quotes.c \
			srcs/utils/env_utils.c \
			srcs/exec/exec.c \
			srcs/exec/exec_utils.c \
			srcs/exec/exec_utils2.c \
			srcs/exec/redir.c \
			srcs/exec/simple_cmd.c \
			srcs/builtins/ft_pwd.c \
			srcs/builtins/ft_env.c \
			srcs/main.c \
			srcs/exec/pipe_exec.c \

RM		:= rm -f


OBJS	:= $(SRCS:.c=.o)

.c.o:
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

CLR_RMV		:= \033[0m
RED		    := \033[1;31m
GREEN		:= \033[1;32m
YELLOW		:= \033[1;33m


${NAME}:	libftr ${OBJS}
			echo "$(GREEN)Compilation ${CLR_RMV}of ${YELLOW}$(NAME) ${CLR_RMV}..."
			${CC} ${CFLAGS} -o ${NAME} ${OBJS} $(LIBS)
			echo "$(GREEN)$(NAME) created[0m ✔️"

all:		${NAME}

libftr:
			make -C libft

clean:
	make -C libft clean
	rm -f $(OBJS)
	echo "$(RED)Cleaned object files${CLR_RMV}"

fclean: clean
	make -C libft fclean
	rm -f $(NAME)
	echo "$(RED)Cleaned all build files${CLR_RMV}"

re: fclean all

.PHONY: all clean fclean re
