.DEFAULT_GOAL	= all
NAME			= containers

# ******************* SETTINGS (NAMESPACE, STD VERSION) ********************** #

NAMESPACE		?= std 
CXXSTD			= c++11

ifeq ($(NAMESPACE),ft)
    NAMESPACE_FLAGS = -DNAMESPACE=ft -DNAMESPACE_NAME=$(shell echo '"ft"')
else
    NAMESPACE_FLAGS = -DNAMESPACE=std -DNAMESPACE_NAME=$(shell echo '"std"')
endif

# ****************************** COMPILE FLAGS ******************************* #

CXX				= c++
CXXSTD_FLAG		= -std=$(CXXSTD)
CXXFLAGS		= $(CXXSTD_FLAG) -MMD -MP -Wall -Wextra -Werror
CXXFLAGS		+= $(NAMESPACE_FLAGS) $(SANITIZE_FLAGS)
# SANITIZE_FLAGS  = -fsanitize=address

CPPFLAGS		= -I./include -I./include/ft -I./include/utils
LDFLAGS			=
LDLIBS			=

# ******************************** FILE LIST ********************************* #

TEST_FILE   	= \
					pair \
					equal \
					lexicographical_compare \
					enable_if \
					is_integral \
					iterator_traits \
					reverse_iterator
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
