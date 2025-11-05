.DEFAULT_GOAL	= all
NAME			= containers

# ================================= SETTINGS ================================= #

NAMESPACE		?= ft
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

BASE_SRCS 		:= 
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

# =========================== TEST MAIN SOURCES ============================== #

UNIT_MAIN   := $(TEST_DIR)/main_unit_test.cpp
STRESS_MAIN := $(TEST_DIR)/main_stress_test.cpp

# ================================== TARGETS ================================= #

all:
	$(MAKE) unit_all
	$(MAKE) stress_all

# =========================== UNIT TEST TARGETS ============================== #

unit_all:
	$(MAKE) build TEST_MAIN=$(UNIT_MAIN) TEST_NAME=unit_all TEST_UTILS=1 TEST_VECTOR=1 TEST_MAP=1 TEST_STACK=1

unit_utils:
	$(MAKE) build TEST_MAIN=$(UNIT_MAIN) TEST_NAME=unit_utils TEST_UTILS=1

unit_vector:
	$(MAKE) build TEST_MAIN=$(UNIT_MAIN) TEST_NAME=unit_vector TEST_VECTOR=1

unit_map:
	$(MAKE) build TEST_MAIN=$(UNIT_MAIN) TEST_NAME=unit_map TEST_MAP=1

unit_stack:
	$(MAKE) build TEST_MAIN=$(UNIT_MAIN) TEST_NAME=unit_stack TEST_STACK=1

# ========================- STRESS TEST TARGETS ============================== #

stress_all:
	$(MAKE) build TEST_MAIN=$(STRESS_MAIN) TEST_NAME=stress_all TEST_VECTOR=1 TEST_MAP=1 TEST_STACK=1

stress_vector:
	$(MAKE) build TEST_MAIN=$(STRESS_MAIN) TEST_NAME=stress_vector TEST_VECTOR=1

stress_map:
	$(MAKE) build TEST_MAIN=$(STRESS_MAIN) TEST_NAME=stress_map TEST_MAP=1

stress_stack:
	$(MAKE) build TEST_MAIN=$(STRESS_MAIN) TEST_NAME=stress_stack TEST_STACK=1

# ================================= BUILD ==================================== #

build:
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) \
		$(addprefix -D, \
			TEST_UTILS=$(or $(TEST_UTILS),0) \
			TEST_VECTOR=$(or $(TEST_VECTOR),0) \
			TEST_MAP=$(or $(TEST_MAP),0) \
			TEST_STACK=$(or $(TEST_STACK),0)) \
		$(TEST_MAIN) $(TEST_SRCS) \
		$(CPPFLAGS) $(LDFLAGS) $(LDLIBS) \
		-o $(BIN_DIR)/$(NAME)_$(NAMESPACE)_$(TEST_NAME)

# ============================ OBJECT BUILD RULE ============================= #

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(BIN_DIR)
	rm -f $(NAME)_*

re: fclean all

-include $(OBJS:.o=.d)

.PHONY: all clean fclean re \
	unit_all unit_utils unit_vector unit_map unit_stack \
	stress_all stress_vector stress_map stress_stack \
	build
