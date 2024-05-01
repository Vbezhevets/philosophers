NAME  = philo
CC    = cc
CFLAGS= -g -Wall -Wextra -Werror #-pthread # -fsanitize=thread
RM    = rm -rf

SRCSDIR     = srcs
SRCS  = \
			${SRCSDIR}/main.c \
			${SRCSDIR}/error.c \
			${SRCSDIR}/utils.c \
			${SRCSDIR}/life.c \

OBJSDIR     = ${SRCSDIR}/objs
OBJS        = $(SRCS:${SRCSDIR}/%.c=${OBJSDIR}/%.o)

all   : $(NAME)

$(NAME)     : $(OBJS)
		$(CC) $(CFLAGS) -o $@ $^

${OBJSDIR}/%.o : ${SRCSDIR}/%.c
		@mkdir -p $(dir $@)
		${CC} ${CFLAGS} -c $< -o $@ -I ./includes

${OBJSDIR_B}/%.o : ${SRCSDIR_B}/%.c
		@mkdir -p $(dir $@)
		${CC} ${CFLAGS} -c $< -o $@ -I ./includes

clean :
		$(RM) $(OBJSDIR)

fclean:
		$(RM) $(OBJSDIR) $(NAME)

re    : fclean all

.PHONY: all clean fclean bonus re