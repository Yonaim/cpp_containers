.DEFAULT_GOAL	= all

NAME			= containers
CXX				= c++
CXXFLAGS		= -std=c++98 -MMD -MP -Wall -Wextra -Werror -fsanitize=address -DNAMESPACE=ft
CPPFLAGS		= -I./include -I./include/ft -I./include/utils
LDFLAGS			=
LDLIBS			=

TEST_FILE   	= pair equal lexicographical_compare enable_if is_integral
TEST_PREFIX	 	= ./src/test/

FILE        	= utils/print main
PREFIX      	= ./src/

SRCS       		:= $(addprefix $(PREFIX),      $(addsuffix .cpp,$(FILE)))
TEST_SRCS  		:= $(addprefix $(TEST_PREFIX), $(addsuffix .cpp,$(TEST_FILE)))

OBJS       		:= $(SRCS:.cpp=.o) $(TEST_SRCS:.cpp=.o)
DEPS      		:= $(OBJS:.o=.d)

# ********************************* MAKE RULES ******************************* #

all : $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS) $(LDLIBS)

clean :
	rm -f $(OBJS) $(DEPS) 

fclean : clean
	rm -f $(NAME)

re : fclean all

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

-include $(DEPS)

.PHONY : all clean fclean re
