.DEFAULT_GOAL	= all
NAME			= containers

# ================================= SETTINGS ================================= #

NAMESPACE		?= std
CXXSTD			= c++11

ifeq ($(NAMESPACE),ft)
    NAMESPACE_FLAGS = -DNAMESPACE=ft
else
    NAMESPACE_FLAGS = -DNAMESPACE=std
endif

MAKEFLAGS += --no-print-directory

# ================================ COMPILER ================================== #

CXX				= c++
CXXSTD_FLAG		= -std=$(CXXSTD)
CXXFLAGS		= $(CXXSTD_FLAG) -MMD -MP -Wall -Wextra -Werror
CXXFLAGS		+= $(NAMESPACE_FLAGS) $(SANITIZE_FLAGS)
# SANITIZE_FLAGS  = -fsanitize=address

CPPFLAGS		= -I./include -I./include/ft -I./test/include
LDFLAGS			=
LDLIBS			=

# ============================= DIRECTORIES ================================== #

SRC_DIR      	:= src
TEST_DIR    	:= test
OBJ_DIR      	:= obj
BIN_DIR      	:= bin

# ============================ COMMON SOURCES ================================ #

BASE_SRCS 		:= $(wildcard $(SRC_DIR)/*.cpp)
BASE_OBJS 		:= $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(BASE_SRCS))

# ============================= TEST SOURCES ================================= #

TEST_UTILS_SRCS  := $(wildcard $(TEST_DIR)/utils/*.cpp)
TEST_VECTOR_SRCS := $(wildcard $(TEST_DIR)/vector/*.cpp)
TEST_MAP_SRCS    := $(wildcard $(TEST_DIR)/map/*.cpp)
TEST_STACK_SRCS  := $(wildcard $(TEST_DIR)/stack/*.cpp)

TEST_SRCS := \
	$(if $(TEST_UTILS),$(TEST_UTILS_SRCS)) \
	$(if $(TEST_VECTOR),$(TEST_VECTOR_SRCS)) \
	$(if $(TEST_MAP),$(TEST_MAP_SRCS)) \
	$(if $(TEST_STACK),$(TEST_STACK_SRCS))

# ================================== TARGETS ================================= #

all:
	@$(MAKE) build TEST_NAME=all TEST_UTILS=1 TEST_VECTOR=1 TEST_MAP=1 TEST_STACK=1
	@$(MAKE) run TEST_NAME=all

test_utils:
	@$(MAKE) build TEST_NAME=utils TEST_UTILS=1
	@$(MAKE) run TEST_NAME=utils

test_vector:
	@$(MAKE) build TEST_NAME=vector TEST_VECTOR=1
	@$(MAKE) run TEST_NAME=vector

test_map:
	@$(MAKE) build TEST_NAME=map TEST_MAP=1
	@$(MAKE) run TEST_NAME=map

test_stack:
	@$(MAKE) build TEST_NAME=stack TEST_STACK=1
	@$(MAKE) run TEST_NAME=stack

# ============================== BUILD / RUN ================================= #

build:
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) \
		$(addprefix -D, \
			TEST_UTILS=$(or $(TEST_UTILS),0) \
			TEST_VECTOR=$(or $(TEST_VECTOR),0) \
			TEST_MAP=$(or $(TEST_MAP),0) \
			TEST_STACK=$(or $(TEST_STACK),0)) \
		$(BASE_SRCS) $(TEST_SRCS) \
		$(CPPFLAGS) $(LDFLAGS) $(LDLIBS) \
		-o $(BIN_DIR)/$(NAME)_$(NAMESPACE)_$(TEST_NAME)

run:
	@./$(BIN_DIR)/$(NAME)_$(NAMESPACE)_$(TEST_NAME)
	@$(MAKE) clean -s

# ======================= OBJECT BUILD RULE ======================= #

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(BIN_DIR)
	rm -f $(NAME)_*

re: fclean all

-include $(OBJS:.o=.d)

.PHONY: all clean fclean re \
		test_utils test_vector test_map test_stack build run
	
