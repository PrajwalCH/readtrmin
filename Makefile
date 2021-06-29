DEBUG = 0
<<<<<<< HEAD
CFLAGS = -DNDEBUG -MMD -O2
=======
CFLAGS = -MMD -std=c11

>>>>>>> 6a7ea83e1ec9e152f635f1988331c15589970c74
ifeq ($(DEBUG), 1)
	CFLAGS += -g -Wall -Werror -Wextra -DDEBUG
else
	CFLAGS += -DNDEBUG -O2
endif

LIB_FOLDER = ./lib
LIB_SRCS = ./src/readtrmin_new.c ./src/util/str_util.c
LIB_OBJS = $(LIB_SRCS:%.c=%.o)
LIB_DEPS = $(LIB_OBJS:%.o=%.d)
LIB_NAME = libreadtrmin.so
LIB_FILE = $(LIB_FOLDER)/$(LIB_NAME)

all: readtrmin install

.PHONY: clean_all
clean_all: clean uninstall

.PHONY: readtrmin
readtrmin: make_lib_folder $(LIB_FILE)

.PHONY: make_lib_folder
make_lib_folder:
	if [ ! -d $(LIB_FOLDER) ]; then mkdir lib; fi

$(LIB_FILE): $(LIB_OBJS)
	$(CC) -shared -fPIC -o $@ $^

<<<<<<< HEAD
-include ($(LIB_DEPS))

=======
>>>>>>> 6a7ea83e1ec9e152f635f1988331c15589970c74
%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

-include ($(LIB_DEPS))

.PHONY: install
install:
	cp $(LIB_FILE) $(LD_LIBRARY_PATH)

.PHONY: uninstall
uninstall:
	$(RM) $(LD_LIBRARY_PATH)/$(LIB_NAME)

.PHONY: clean
clean:
	$(RM) $(LIB_OBJS) $(LIB_DEPS) $(LIB_FILE)
	$(RM) -r $(LIB_FOLDER)
