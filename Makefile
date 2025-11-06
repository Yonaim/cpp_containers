.DEFAULT_GOAL	= all
NAME			= containers

# ================================= SETTINGS ================================= #

NAMESPACE		?= ft
CXXSTD			:= c++98
CONTAINERS_PATH	= include/ft

ifeq ($(NAMESPACE),std)
    NAMESPACE_FLAG = -DSTD_MODE=1
else
    NAMESPACE_FLAG = 
endif

MAKEFLAGS += --no-print-directory

# ================================ COMPILER ================================== #

CXX				= c++
CXXSTDFLAG		= -std=$(CXXSTD)
CXXFLAGS		= $(CXXSTDFLAG) -MMD -MP -Wall -Wextra -Werror $(NAMESPACE_FLAG) $(SANITIZE_FLAG)
# SANITIZE_FLAG  = -fsanitize=address

CPPFLAGS		= -I./$(CONTAINERS_PATH) -I./test/include
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

# =========================== TEST MAIN SOURCES ============================== #

UNIT_MAIN   := $(TEST_DIR)/main_unit_test.cpp
STRESS_MAIN := $(TEST_DIR)/main_stress_test.cpp

# ============================= UNIT TEST SOURCES ============================ #

UNIT_UTILS_NAMES  := enable_if equal is_integral iterator_traits lexicographical_compare pair reverse_iterator
UNIT_VECTOR_NAMES := vector_basic vector_modifiers
UNIT_MAP_NAMES    := map_basic map_lookup_compare map_modifiers
UNIT_STACK_NAMES  := stack

UNIT_UTILS_SRCS  := $(addprefix $(TEST_DIR)/utils/, $(addsuffix .cpp, $(addprefix test_, $(UNIT_UTILS_NAMES))))
UNIT_VECTOR_SRCS := $(addprefix $(TEST_DIR)/vector/, $(addsuffix .cpp, $(addprefix test_, $(UNIT_VECTOR_NAMES))))
UNIT_MAP_SRCS    := $(addprefix $(TEST_DIR)/map/, $(addsuffix .cpp, $(addprefix test_, $(UNIT_MAP_NAMES))))
UNIT_STACK_SRCS  := $(addprefix $(TEST_DIR)/stack/, $(addsuffix .cpp, $(addprefix test_, $(UNIT_STACK_NAMES))))

UNIT_SRCS := \
	$(if $(TEST_UTILS),$(UNIT_UTILS_SRCS)) \
	$(if $(TEST_VECTOR),$(UNIT_VECTOR_SRCS)) \
	$(if $(TEST_MAP),$(UNIT_MAP_SRCS)) \
	$(if $(TEST_STACK),$(UNIT_STACK_SRCS))

# ============================ STRESS TEST SOURCES =========================== #

STRESS_SRCS := $(STRESS_MAIN)

# ================================== TARGETS ================================= #

all:
	$(MAKE) unit_all
	$(MAKE) stress_all

# ============================ HELPER RULES ================================== #

define run_build
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) \
		$(addprefix -D, \
			TEST_UTILS=$(or $(TEST_UTILS),0) \
			TEST_VECTOR=$(or $(TEST_VECTOR),0) \
			TEST_MAP=$(or $(TEST_MAP),0) \
			TEST_STACK=$(or $(TEST_STACK),0)) \
		$(1) $(2) \
		$(CPPFLAGS) $(LDFLAGS) $(LDLIBS) \
		-o $(BIN_DIR)/$(NAME)_$(NAMESPACE)_$(3)
endef

make_unit:
	$(call run_build,$(UNIT_MAIN),$(UNIT_SRCS),$(TEST_NAME))

make_stress:
	$(call run_build,,$(STRESS_SRCS),$(TEST_NAME))

# =========================== UNIT TEST TARGETS ============================== #

unit_all:
	$(MAKE) make_unit TEST_NAME=unit_all TEST_UTILS=1 TEST_VECTOR=1 TEST_MAP=1 TEST_STACK=1

unit_utils:
	$(MAKE) make_unit TEST_NAME=unit_utils TEST_UTILS=1

unit_vector:
	$(MAKE) make_unit TEST_NAME=unit_vector TEST_VECTOR=1

unit_map:
	$(MAKE) make_unit TEST_NAME=unit_map TEST_MAP=1

unit_stack:
	$(MAKE) make_unit TEST_NAME=unit_stack TEST_STACK=1

# ======================== STRESS TEST TARGETS =============================== #

stress_all:
	$(MAKE) make_stress TEST_NAME=stress_all TEST_VECTOR=1 TEST_MAP=1 TEST_STACK=1

stress_vector:
	$(MAKE) make_stress TEST_NAME=stress_vector TEST_VECTOR=1

stress_map:
	$(MAKE) make_stress TEST_NAME=stress_map TEST_MAP=1

stress_stack:
	$(MAKE) make_stress TEST_NAME=stress_stack TEST_STACK=1

# ============================ CLEAN / REBUILD =============================== #

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
	make_unit make_stress
