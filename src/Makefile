# @file Makefile
# @brief project GNU-Make Makefile
# @code{.ebp}
# rules:
# @encode

# CREATE MISSING DIRS
dir_guard			= @mkdir -p $(@D)

# MODULE SOURCE/OBJS DIRS
COMMON_DIR			= common
APP_DIR				= app

# OBJECTS
OBJS_COMMON			= log.o version.o argparse.o
OBJS_APP				= app.o

OBJS 					= $(addprefix $(COMMON_DIR)/, $(OBJS_COMMON))\
						  $(addprefix $(APP_DIR)/, $(OBJS_APP))

OBJS_NO_PATH		= $(OBJS_COMMON) $(OBJS_APP)

# LIBRARIES
LIBS					= -lpthread
LIBS_PATH			= /usr/lib/gcc/i686-redhat-linux/9/include

# OBJECTS ROOT PATH
OBJS_PATH			= ../build/objs

# DEPS FILES PATH
DEPS_PATH			= ../build/deps

# CONFIG FILES
APP_CONFIG_FILE	= app.cfg

# RELEASE NAME
RELEASE_NAME		= main

# VERSION SPECIFICS
MAJ_VER=0
MIN_VER=0
DEV_VER=1
BUILD_NUMBER=$$(date +'%y%m%d%H%M')
BUILD_DATE=$$(date +'%Y.%m.%d')

# TARGETS
TARGET_REL_PATH	= ../build/release
TARGET_DEB_PATH 	= ../build/debug
TARGET_RELEASE		= $(TARGET_REL_PATH)/$(RELEASE_NAME)
TARGET_DEBUG		= $(TARGET_DEB_PATH)/$(RELEASE_NAME)
CONF_FILES_RELEASE= $(addprefix $(TARGET_REL_PATH)/, $(APP_CONFIG_FILE))
CONF_FILES_DEBUG	= $(addprefix $(TARGET_DEB_PATH)/, $(APP_CONFIG_FILE))

# SRC SEARCH PATHS
VPATH = $(COMMON_DIR):$(APP_DIR):

# TOOLS
COMPILER				= gcc
LINKER				= gcc

# CFLAGS
CFLAGS				= -c -Wall -D_GNU_SOURCE
CFLAGS				+= -DMAJ_VER=$(MAJ_VER) -DMIN_VER=$(MIN_VER) -DDEV_VER=$(DEV_VER) 
CFLAGS				+= -DBUILD_DATE=$(BUILD_DATE) -DBUILD_NUMBER=$(BUILD_NUMBER)


# RULES FOR GENERATING DEPENDENCIES
$(DEPS_PATH)/%.d: %.c
	@echo Make deps for $<
	$(dir_guard)
	@set -e; rm -f $@; \
	$(COMPILER) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OBJS_PATH)/\1.o $@ : Makefile ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# COMPILER RULES
$(OBJS_PATH)/$(COMMON_DIR)/%.o: $(DEPS_PATH)/%.d
	$(dir_guard)
	$(COMPILER) $(CFLAGS) $(COMMON_DIR)/$*.c -o $@

$(OBJS_PATH)/$(APP_DIR)/%.o: $(DEPS_PATH)/%.d
	$(dir_guard)
	$(COMPILER) $(CFLAGS) $(APP_DIR)/$*.c -o $@

# LINKER RULES
$(TARGET_RELEASE): $(addprefix $(OBJS_PATH)/, $(OBJS))
	$(dir_guard)
	$(LINKER) $^ $(LIBS) -o $@

$(TARGET_DEBUG): $(addprefix $(OBJS_PATH)/, $(OBJS))
	$(dir_guard)
	$(LINKER) $^ $(LIBS) -o $@

# CONFIG FILES
$(TARGET_REL_PATH)/%.cfg: $(APP_DIR)/$(APP_CONFIG_FILE)
	cp -f $^ $@

$(TARGET_DEB_PATH)/%.cfg: $(APP_DIR)/$(APP_CONFIG_FILE)
	cp -f $^ $@

# PULL DEPENDENCIES
-include $(addprefix $(DEPS_PATH)/, $(OBJS_NO_PATH:.o=.d))
.SECONDARY: $(OBJS_NO_PATH:.o=.d)

# MAIN RULES
all: $(TARGET_RELEASE) $(CONFIG_FILES_RELEASE)

debug: CFLAGS += -DDEBUG -g
debug: $(TARGET_DEBUG) $(CONFIG_FILES)

clean:
	rm -R -f $(OBJS_PATH)
	rm -R -f $(TARGET_PATH)
	rm -R -f $(DEPS_PATH)





