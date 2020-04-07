NAME = polyvec.a

# directories
SRC_D = src
OBJ_D = obj
INC_D = inc

# C source and header files
SRC =	$(SRC_D)/vector.c													\
		$(SRC_D)/types/fifobuffer.c											\
		$(SRC_D)/types/lifobuffer.c											\
		$(SRC_D)/types/ringbuffer.c											\
		$(SRC_D)/types/buffer.c												\
		$(SRC_D)/types/ringbuffer_mt.c										\

INC =	$(INC_D)/vector.h													\
		$(INC_D)/types/fifobuffer.h											\
		$(INC_D)/types/lifobuffer.h											\
		$(INC_D)/types/ringbuffer.h											\
		$(INC_D)/types/ringbuffer_mt.h										\
		$(INC_D)/types/buffer.h												\

OBJ :=	$(SRC:$(SRC_D)/%.c=$(OBJ_D)/%.o)

# outputting
CC_LOG=./.cc.log
CC_ERROR=./.cc.error

NO_COLOR=\x1b[0m
OK_COLOR=\x1b[32;01m
ERROR_COLOR=\x1b[31;01m
WARN_COLOR=\x1b[33;01m

OK_STRING=$(OK_COLOR)[OK]$(NO_COLOR)
ERROR_STRING=$(ERROR_COLOR)[ERRORS]$(NO_COLOR)
WARN_STRING=$(WARN_COLOR)[WARNINGS]$(NO_COLOR)

ECHO=printf
CAT=cat

# set DBG to debugger of choice (or nothing)
# DBG = lldb

# compiler and linker
CC = clang

# compile flags
CC_FLAGS = -Werror -Wextra -Wall

# flags for criterion tests
CRIT_FLAGS =  --full-stats --verbose

# debugging or optimization flags
ifeq ($(DEBUG),1)
    CC_FLAGS += -g -fsanitize=address -DDEBUG
    #export  LSAN_OPTIONS=verbosity=1:log_threads=1
else
    CC_FLAGS += -O3 -march=native
endif

# make commands
all: $(NAME)

$(NAME): $(OBJ_D) $(OBJ) $(INC)
	@$(ECHO) "Linking $(NAME)..."
	@ar -rcs $(NAME) $(OBJ) 2>$(CC_LOG) || touch $(CC_ERROR)
	@if test -e $(CC_ERROR); then											\
		$(ECHO) "$(ERROR_STRING)\n" && $(CAT) $(CC_LOG);					\
	elif test -s $(CC_LOG); then											\
		$(ECHO) "$(WARN_STRING)\n" && $(CAT) $(CC_LOG);						\
	else																	\
		$(ECHO) "$(OK_STRING)\n";											\
	fi
	@$(RM) -f $(CC_LOG) $(CC_ERROR)

$(OBJ_D):
	@mkdir -p $(OBJ_D)
	@mkdir -p $(OBJ_D)/types

$(OBJ): $(OBJ_D)/%.o: $(SRC_D)/%.c
	@$(ECHO) "Compiling $<..."
	@$(CC) $(CC_FLAGS) -I$(INC_D) -c $< -o $@ 2>$(CC_LOG)	\
		|| touch $(CC_ERROR)
	@if test -e $(CC_ERROR); then											\
		$(ECHO) "$(ERROR_STRING)\n" && $(CAT) $(CC_LOG);					\
	elif test -s $(CC_LOG); then											\
		$(ECHO) "$(WARN_STRING)\n" && $(CAT) $(CC_LOG);						\
	else																	\
		$(ECHO) "$(OK_STRING)\n";											\
	fi
	@$(RM) -f $(CC_LOG) $(CC_ERROR)

clean:
	@$(RM) $(OBJ)
	@$(RM) -r $(OBJ_D)

buffer_crit_test: TEST='buffer_crit_t'
buffer_crit_test: $(NAME)
	@$(ECHO) "Compiling $(TEST).c..." 2>$(CC_LOG) || touch $(CC_ERROR)
	@$(CC) $(CC_FLAGS) -I$(INC_D) -lcriterion -o $(TEST) tests/$(TEST).c $(NAME)
	@if test -e $(CC_ERROR); then                                           \
        $(ECHO) "$(ERROR_STRING)\n" && $(CAT) $(CC_LOG);					\
    elif test -s $(CC_LOG); then                                            \
        $(ECHO) "$(WARN_STRING)\n" && $(CAT) $(CC_LOG);                     \
    else                                                                    \
        $(ECHO) "$(OK_STRING)\n";                                           \
    fi
	@$(ECHO) "Running $(TEST)...\n"
	@time $(DBG) ./$(TEST) $(CRIT_FLAGS) && $(RM) -f $(TEST) && $(RM) -rf $(TEST).dSYM 2>$(CC_LOG) || touch $(CC_ERROR)
	@if test -e $(CC_ERROR); then                                           \
		$(ECHO) "Completed $(TEST): $(ERROR_STRING)\n" && $(CAT) $(CC_LOG);		\
    elif test -s $(CC_LOG); then											\
		$(ECHO) "Completed $(TEST): $(WARN_STRING)\n" && $(CAT) $(CC_LOG);		\
    else                                                                    \
		$(ECHO) "Completed $(TEST): $(OK_STRING)\n";								\
    fi
	@$(RM) -f $(CC_LOG) $(CC_ERROR)

ringbuffer_crit_test: TEST='ringbuffer_crit_t'
ringbuffer_crit_test: $(NAME)
	@$(ECHO) "Compiling $(TEST).c..." 2>$(CC_LOG) || touch $(CC_ERROR)
	@$(CC) $(CC_FLAGS) -I$(INC_D) -lcriterion -o $(TEST) tests/$(TEST).c $(NAME)
	@if test -e $(CC_ERROR); then                                           \
        $(ECHO) "$(ERROR_STRING)\n" && $(CAT) $(CC_LOG);					\
    elif test -s $(CC_LOG); then                                            \
        $(ECHO) "$(WARN_STRING)\n" && $(CAT) $(CC_LOG);                     \
    else                                                                    \
        $(ECHO) "$(OK_STRING)\n";                                           \
    fi
	@$(ECHO) "Running $(TEST)...\n"
	@time $(DBG) ./$(TEST) $(CRIT_FLAGS) && $(RM) -f $(TEST) && $(RM) -rf $(TEST).dSYM 2>$(CC_LOG) || touch $(CC_ERROR)
	@if test -e $(CC_ERROR); then                                           \
		$(ECHO) "Completed $(TEST): $(ERROR_STRING)\n" && $(CAT) $(CC_LOG);		\
    elif test -s $(CC_LOG); then											\
		$(ECHO) "Completed $(TEST): $(WARN_STRING)\n" && $(CAT) $(CC_LOG);		\
    else                                                                    \
		$(ECHO) "Completed $(TEST): $(OK_STRING)\n";								\
    fi
	@$(RM) -f $(CC_LOG) $(CC_ERROR)

ringbuffer_mt_crit_test: TEST='ringbuffer_mt_crit_t'
ringbuffer_mt_crit_test: $(NAME)
	@$(ECHO) "Compiling $(TEST).c..." 2>$(CC_LOG) || touch $(CC_ERROR)
	@$(CC) $(CC_FLAGS) -I$(INC_D) -lcriterion -o $(TEST) tests/$(TEST).c $(NAME)
	@if test -e $(CC_ERROR); then                                           \
        $(ECHO) "$(ERROR_STRING)\n" && $(CAT) $(CC_LOG);					\
    elif test -s $(CC_LOG); then                                            \
        $(ECHO) "$(WARN_STRING)\n" && $(CAT) $(CC_LOG);                     \
    else                                                                    \
        $(ECHO) "$(OK_STRING)\n";                                           \
    fi
	@$(ECHO) "Running $(TEST)...\n"
	@time $(DBG) ./$(TEST) $(CRIT_FLAGS) && $(RM) -f $(TEST) && $(RM) -rf $(TEST).dSYM 2>$(CC_LOG) || touch $(CC_ERROR)
	@if test -e $(CC_ERROR); then                                           \
		$(ECHO) "Completed $(TEST): $(ERROR_STRING)\n" && $(CAT) $(CC_LOG);		\
    elif test -s $(CC_LOG); then											\
		$(ECHO) "Completed $(TEST): $(WARN_STRING)\n" && $(CAT) $(CC_LOG);		\
    else                                                                    \
		$(ECHO) "Completed $(TEST): $(OK_STRING)\n";								\
    fi
	@$(RM) -f $(CC_LOG) $(CC_ERROR)

buffer_test: TEST='buffer_t'
buffer_test: $(NAME)
	@$(ECHO) "Compiling $(TEST).c..." 2>$(CC_LOG) || touch $(CC_ERROR)
	@$(CC) $(CC_FLAGS) -I$(INC_D) -o $(TEST) tests/$(TEST).c $(NAME)
	@if test -e $(CC_ERROR); then                                           \
        $(ECHO) "$(ERROR_STRING)\n" && $(CAT) $(CC_LOG);					\
    elif test -s $(CC_LOG); then                                            \
        $(ECHO) "$(WARN_STRING)\n" && $(CAT) $(CC_LOG);                     \
    else                                                                    \
        $(ECHO) "$(OK_STRING)\n";                                           \
    fi
	@$(ECHO) "Running $(TEST)...\n"
	@time $(DBG) ./$(TEST) && $(RM) -f $(TEST) && $(RM) -rf $(TEST).dSYM 2>$(CC_LOG) || touch $(CC_ERROR)
	@if test -e $(CC_ERROR); then                                           \
		$(ECHO) "Completed $(TEST): $(ERROR_STRING)\n" && $(CAT) $(CC_LOG);		\
    elif test -s $(CC_LOG); then											\
		$(ECHO) "Completed $(TEST): $(WARN_STRING)\n" && $(CAT) $(CC_LOG);		\
    else                                                                    \
		$(ECHO) "Completed $(TEST): $(OK_STRING)\n";								\
    fi
	@$(RM) -f $(CC_LOG) $(CC_ERROR)

fifobuffer_test: TEST='fifobuffer_t'
fifobuffer_test: $(NAME)
	@$(ECHO) "Compiling $(TEST).c..." 2>$(CC_LOG) || touch $(CC_ERROR)
	@$(CC) $(CC_FLAGS) -I$(INC_D) -o $(TEST) tests/$(TEST).c $(NAME)
	@if test -e $(CC_ERROR); then                                           \
        $(ECHO) "$(ERROR_STRING)\n" && $(CAT) $(CC_LOG);					\
    elif test -s $(CC_LOG); then                                            \
        $(ECHO) "$(WARN_STRING)\n" && $(CAT) $(CC_LOG);                     \
    else                                                                    \
        $(ECHO) "$(OK_STRING)\n";                                           \
    fi
	@$(ECHO) "Running $(TEST)...\n"
	@time $(DBG) ./$(TEST) && $(RM) -f $(TEST) && $(RM) -rf $(TEST).dSYM 2>$(CC_LOG) || touch $(CC_ERROR)
	@if test -e $(CC_ERROR); then                                           \
		$(ECHO) "Completed $(TEST): $(ERROR_STRING)\n" && $(CAT) $(CC_LOG);		\
    elif test -s $(CC_LOG); then											\
		$(ECHO) "Completed $(TEST): $(WARN_STRING)\n" && $(CAT) $(CC_LOG);		\
    else                                                                    \
		$(ECHO) "Completed $(TEST): $(OK_STRING)\n";								\
    fi
	@$(RM) -f $(CC_LOG) $(CC_ERROR)

lifobuffer_test: TEST='lifobuffer_t'
lifobuffer_test: $(NAME)
	@$(ECHO) "Compiling $(TEST).c..." 2>$(CC_LOG) || touch $(CC_ERROR)
	@$(CC) $(CC_FLAGS) -I$(INC_D) -o $(TEST) tests/$(TEST).c $(NAME)
	@if test -e $(CC_ERROR); then                                           \
        $(ECHO) "$(ERROR_STRING)\n" && $(CAT) $(CC_LOG);					\
    elif test -s $(CC_LOG); then                                            \
        $(ECHO) "$(WARN_STRING)\n" && $(CAT) $(CC_LOG);                     \
    else                                                                    \
        $(ECHO) "$(OK_STRING)\n";                                           \
    fi
	@$(ECHO) "Running $(TEST)...\n"
	@time $(DBG) ./$(TEST) && $(RM) -f $(TEST) && $(RM) -rf $(TEST).dSYM 2>$(CC_LOG) || touch $(CC_ERROR)
	@if test -e $(CC_ERROR); then                                           \
		$(ECHO) "Completed $(TEST): $(ERROR_STRING)\n" && $(CAT) $(CC_LOG);		\
    elif test -s $(CC_LOG); then											\
		$(ECHO) "Completed $(TEST): $(WARN_STRING)\n" && $(CAT) $(CC_LOG);		\
    else                                                                    \
		$(ECHO) "Completed $(TEST): $(OK_STRING)\n";								\
    fi
	@$(RM) -f $(CC_LOG) $(CC_ERROR)

test: $(NAME)
	@make buffer_crit_test
	#@make fifobuffer_crit_test
	#@make lifobuffer_crit_test
	@make ringbuffer_crit_test
	@make ringbuffer_mt_crit_test

fclean: clean
	@$(RM) $(NAME)
	@$(RM) -rf *.dSYM
	@$(RM) -f fifobuffer_t

re: fclean all

.PHONY = all clean fclean re test
