.DEFAULT_GOAL	= all

NAME			= containers
CC				= c++
CFLAGS			= -Wall -Wextra -Werror -std=c++98
CPPFLAGS		=
LDFLAGS			= 
LDLIBS			= 
FILE			= main
SRCS 			= $(addprefix $(PREFIX), $(addsuffix $(SUFFIX).cpp, $(FILE)))
OBJS 			= $(SRCS:.c=.o)

# ********************************* MAKE RULES ******************************* #

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS) $(LDLIBS)

clean :
	make rm_obj
	BONUS=1 make rm_obj
	make clean

fclean :
	rm -f $(NAME) 
	make clean
	make fclean

re : 
	make fclean
	make all

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY : all clean fclean re bonus
