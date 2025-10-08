.DEFAULT_GOAL	= all

NAME			= containers
CXX				= c++
CXXFLAGS		= -Wall -Wextra -Werror -std=c++98
CPPFLAGS		=
LDFLAGS			=
LDLIBS			=
FILE			= main
SRCS 			= $(addprefix $(PREFIX), $(addsuffix $(SUFFIX).cpp, $(FILE)))
OBJS 			= $(SRCS:.cpp=.o)

# ********************************* MAKE RULES ******************************* #

all : $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS) $(LDLIBS)

clean :
	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

.PHONY : all clean fclean re
