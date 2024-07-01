NAME	:= minishell
CC		:= gcc
CFLAGS	:= -Wall -Wextra -Werror -g3
LIBS = -lreadline
SRCS	:=
RM		:= rm -f


OBJS	:= $(SRCS:.c=.o)

.c.o:
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

CLR_RMV		:= \033[0m
RED		    := \033[1;31m
GREEN		:= \033[1;32m
YELLOW		:= \033[1;33m

${NAME}:	libft ${OBJS}
			echo "$(GREEN)Compilation ${CLR_RMV}of ${YELLOW}$(NAME) ${CLR_RMV}..."
			${CC} ${CFLAGS} -o ${NAME} ${OBJS} $(LIBS)
			echo "$(GREEN)$(NAME) created[0m ✔️"

all:		${NAME}


libft:
		make -C libft

clean:
	rm -f $(OBJS)
	echo "$(RED)Cleaned object files${CLR_RMV}"

fclean: clean
	rm -f $(NAME)
	echo "$(RED)Cleaned all build files${CLR_RMV}"

re: fclean all

.PHONY: all clean fclean re
