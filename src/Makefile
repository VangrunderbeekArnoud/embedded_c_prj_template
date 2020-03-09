# @file Makefile
# @brief project GNU-Make Makefile
# @code{.ebp}
# @author Arnoud Vangrunderbeek
# rules:
# @encode

# CREATE MISSING DIRS
dir_guard				= @mkdir -p $(@D);

RCM_RELEASE_NAME		= RCMsx
RCM_CONFIG_FILE			= config.cfg

APP_PATH				= app
COMMON_PATH				= common

APP_OBJS				= app.o
COMMON_OBJS				= log.o version.o argparse.o config.o timers.o

OBJS_RCM				= $(addprefix $(APP_PATH)/, $(APP_OBJS))\
					  	  $(addprefix $(COMMON_PATH)/, $(COMMON_OBJS))

OBJS_NO_PATH			= $(APP_OBJS) $(COMMON_OBJS)
VPATH 					= $(APP_PATH):$(COMMON_PATH)

MAJ_VER					= 00
MIN_VER					= 01
DEV_VER					= $$(cat $(BUILD_NUMBER_FILE))
BUILD_NUMBER_FILE		= .build_number
BUILD_NUMBER			= $$(cat $(BUILD_NUMBER_FILE) | xargs printf "%03d")
BUILD_DATE				= $$(date +'%Y.%m.%d')
GIT_HASH				= $$(git log -1 --pretty=format:"%H")
GIT_BRANCH				= $$(git log -1 --pretty=format:"%D" | awk '{print $$3}' | sed 's/.$$//')

BIN_PATH				= ../bin
LIB_PATH				= ../lib
TARGET_PATH				= $(BIN_PATH)/target
ARM_PATH 				= $(TARGET_PATH)/rcm
LNX_PATH 				= $(TARGET_PATH)/linux
ARM_PATH_RELEASE		= $(ARM_PATH)/release
ARM_PATH_DEBUG			= $(ARM_PATH)/debug
LNX_PATH_RELEASE		= $(LNX_PATH)/release
LNX_PATH_DEBUG			= $(LNX_PATH)/debug

ARM_LIB_PATH			= $(LIB_PATH)/armv7l
ARM_RELEASE_RCM			= $(ARM_PATH_RELEASE)/$(RCM_RELEASE_NAME)
ARM_RELEASE_OBJS_PATH	= $(ARM_PATH_RELEASE)/objs
ARM_RELEASE_DEPS_PATH	= $(ARM_PATH_RELEASE)/deps
ARM_RELEASE_RCM_CONFIG	= $(ARM_PATH_RELEASE)/$(RCM_CONFIG_FILE)
ARM_DEBUG_RCM			= $(ARM_PATH_DEBUG)/$(RCM_RELEASE_NAME)
ARM_DEBUG_OBJS_PATH		= $(ARM_PATH_DEBUG)/objs
ARM_DEBUG_DEPS_PATH		= $(ARM_PATH_DEBUG)/deps
ARM_DEBUG_RCM_CONFIG	= $(ARM_PATH_DEBUG)/$(RCM_CONFIG_FILE)

LNX_LIB_PATH			= $(LIB_PATH)/i686
LNX_RELEASE_RCM			= $(LNX_PATH_RELEASE)/$(RCM_RELEASE_NAME)
LNX_RELEASE_OBJS_PATH	= $(LNX_PATH_RELEASE)/objs
LNX_RELEASE_DEPS_PATH	= $(LNX_PATH_RELEASE)/deps
LNX_RELEASE_RCM_CONFIG	= $(LNX_PATH_RELEASE)/$(RCM_CONFIG_FILE)
LNX_DEBUG_RCM			= $(LNX_PATH_DEBUG)/$(RCM_RELEASE_NAME)
LNX_DEBUG_OBJS_PATH		= $(LNX_PATH_DEBUG)/objs
LNX_DEBUG_DEPS_PATH		= $(LNX_PATH_DEBUG)/deps
LNX_DEBUG_RCM_CONFIG	= $(LNX_PATH_DEBUG)/$(RCM_CONFIG_FILE)

ARM_COMPILER			= arm-linux-gcc
ARM_LINKER				= arm-linux-gcc
LNX_COMPILER			= gcc
LNX_LINKER				= gcc

LIBS					= -lpthread -lrt

CFLAGS					= -c -Wall -D_GNU_SOURCE
CFLAGS_DEBUG			= -g -ggdb
CFLAGS_RCM				= -DMAJ_VER=$(MAJ_VER) -DMIN_VER=$(MIN_VER) -DDEV_VER=$(DEV_VER) 
CFLAGS_RCM				+= -DBUILD_DATE=$(BUILD_DATE) -DBUILD_NUMBER=$(DEV_VER)
CFLAGS_RCM				+= -DGIT_HASH=$(GIT_HASH) -DGIT_BRANCH=$(GIT_BRANCH)

COLOR_NO				= \e[0m
COLOR_OK				= \e[1;32m
COLOR_ERROR				= \e[31m
COLOR_WARNING			= \e[33m
STRING_OK				= $(COLOR_OK)[OK]$(COLOR_NO)
STRING_ERROR			= $(COLOR_ERROR)[ERROR]$(COLOR_NO)
STRING_WARNING			= $(COLOR_WARNING)[WARNING]$(COLOR_NO)

CCLOG 					= .cc.log
CCERROR 				= .cc.error

log_status:= @if test -e $(CCERROR); \
	then echo -e "\t\t$(STRING_ERROR)\n" && cat $(CCLOG) && rm -f $(CCLOG) $(CCERROR); echo -e "\n"; \
	elif test -s $(CCLOG); then echo -e "\t\t$(STRING_WARNING)\n" && cat $(CCLOG) && rm -f $(CCLOG); echo -e "\n"; \
	else echo -e "\t\t$(STRING_OK)" && rm -f $(CCLOG); fi;

increment_build_number:= @if ! test -e $(CCERROR); \
	then echo $$(($$(cat $(BUILD_NUMBER_FILE))+1)) > $(BUILD_NUMBER_FILE); fi;

append_build_number:= @if ! test -e $(CCERROR); \
	then mv $(ARM_RELEASE_RCM) $(ARM_RELEASE_RCM)_b$(BUILD_NUMBER); fi;

# RULES FOR GENERATING DEPENDENCIES
$(ARM_RELEASE_DEPS_PATH)/%.d: %.c
	$(dir_guard)
	@printf "generating dependency file %50s" "$@"
	@set -e; $(ARM_COMPILER) -MM $(CFLAGS) $(CFLAGS_RCM) $< > $@.$$$$
	@echo -e "\t\t$(STRING_OK)"

$(ARM_DEBUG_DEPS_PATH)/%.d: %.c
	$(dir_guard)
	@printf "generating dependency file %50s\t" "$@"
	@set -e; $(ARM_COMPILER) -MM $(CFLAGS) $(CFLAGS_DEBUG) $(CFLAGS_RCM) $< > $@.$$$$
	@echo -e "\t$(STRING_OK)"

$(LNX_RELEASE_DEPS_PATH)/%.d: %.c
	$(dir_guard)
	@printf "generating dependency file %50s" "$@"
	@set -e; $(LNX_COMPILER) -MM $(CFLAGS) $(CFLAGS_RCM) $< > $@.$$$$
	@echo -e "\t\t$(STRING_OK)"

$(LNX_DEBUG_DEPS_PATH)/%.d: %.c
	$(dir_guard)
	@printf "generating dependency file %50s" "$@"
	@set -e; $(LNX_COMPILER) -MM $(CFLAGS) $(CFLAGS_DEBUG) $(CFLAGS_RCM) $< > $@.$$$$
	@echo -e "\t\t$(STRING_OK)"


# COMPILER RULES
$(ARM_RELEASE_OBJS_PATH)/$(APP_PATH)/%.o: $(ARM_RELEASE_DEPS_PATH)/%.d
	$(dir_guard)
	@printf "generating object file     %50s" "$@"
	@set -e; $(ARM_COMPILER) $(CFLAGS) $(CFLAGS_RCM) $(APP_PATH)/$*.c -o $@ 2> $(CCLOG) || touch $(CCERROR)
	$(log_status)

$(ARM_RELEASE_OBJS_PATH)/$(COMMON_PATH)/%.o: $(ARM_RELEASE_DEPS_PATH)/%.d
	$(dir_guard)
	@printf "generating object file     %50s" "$@"
	@set -e; $(ARM_COMPILER) $(CFLAGS) $(CFLAGS_RCM) $(COMMON_PATH)/$*.c -o $@ 2> $(CCLOG) || touch $(CCERROR)
	$(log_status)

$(ARM_DEBUG_OBJS_PATH)/$(APP_PATH)/%.o: $(ARM_DEBUG_DEPS_PATH)/%.d
	$(dir_guard)
	@printf "generating object file     %50s" "$@"
	@set -e; $(ARM_COMPILER) $(CFLAGS) $(CFLAGS_RCM) $(CFLAGS_DEBUG) $(APP_PATH)/$*.c -o $@ 2> $(CCLOG) || touch $(CCERROR)
	$(log_status)

$(ARM_DEBUG_OBJS_PATH)/$(COMMON_PATH)/%.o: $(ARM_DEBUG_DEPS_PATH)/%.d
	$(dir_guard)
	@printf "generating object file     %50s" "$@"
	@set -e; $(ARM_COMPILER) $(CFLAGS) $(CFLAGS_RCM) $(CFLAGS_DEBUG) $(COMMON_PATH)/$*.c -o $@ 2> $(CCLOG) || touch $(CCERROR)
	$(log_status)

$(LNX_RELEASE_OBJS_PATH)/$(APP_PATH)/%.o: $(LNX_RELEASE_DEPS_PATH)/%.d
	$(dir_guard)
	@printf "generating object file     %50s" "$@"
	@set -e; $(LNX_COMPILER) $(CFLAGS) $(CFLAGS_RCM) $(APP_PATH)/$*.c -o $@ 2> $(CCLOG) || touch $(CCERROR)
	$(log_status)

$(LNX_RELEASE_OBJS_PATH)/$(COMMON_PATH)/%.o: $(LNX_RELEASE_DEPS_PATH)/%.d
	$(dir_guard)
	@printf "generating object file     %50s" "$@"
	@set -e; $(LNX_COMPILER) $(CFLAGS) $(CFLAGS_RCM) $(COMMON_PATH)/$*.c -o $@ 2> $(CCLOG) || touch $(CCERROR)
	$(log_status)

$(LNX_DEBUG_OBJS_PATH)/$(APP_PATH)/%.o: $(LNX_DEBUG_DEPS_PATH)/%.d
	$(dir_guard)
	@printf "generating object file     %50s" "$@"
	@set -e; $(LNX_COMPILER) $(CFLAGS) $(CFLAGS_RCM) $(CFLAGS_DEBUG) $(APP_PATH)/$*.c -o $@ 2> $(CCLOG) || touch $(CCERROR)
	$(log_status)

$(LNX_DEBUG_OBJS_PATH)/$(COMMON_PATH)/%.o: $(LNX_DEBUG_DEPS_PATH)/%.d
	$(dir_guard)
	@printf "generating object file     %50s" "$@"
	@set -e; $(LNX_COMPILER) $(CFLAGS) $(CFLAGS_RCM) $(CFLAGS_DEBUG) $(COMMON_PATH)/$*.c -o $@ 2> $(CCLOG) || touch $(CCERROR)
	$(log_status)

# LINKER RULES
$(ARM_RELEASE_RCM): $(addprefix $(ARM_RELEASE_OBJS_PATH)/, $(OBJS_RCM))
	$(dir_guard)
	@printf "generating target file     %50s" "$@"
	@set -e; $(ARM_LINKER) $^ $(LIBS) -o $@ 2> $(CCLOG) || touch $(CCERROR)
	$(append_build_number)
	$(increment_build_number)
	@cp $(RCM_CONFIG_FILE) $(ARM_PATH_RELEASE)
	$(log_status)

$(ARM_DEBUG_RCM): $(addprefix $(ARM_DEBUG_OBJS_PATH)/, $(OBJS_RCM))
	$(dir_guard)
	@printf "generating target file     %50s" "$@"
	@set -e; $(ARM_LINKER) $^ $(LIBS) -o $@ 2> $(CCLOG) || touch $(CCERROR)
	@cp $(RCM_CONFIG_FILE) $(ARM_PATH_DEBUG)
	$(log_status)

$(LNX_RELEASE_RCM): $(addprefix $(LNX_RELEASE_OBJS_PATH)/, $(OBJS_RCM))
	$(dir_guard)
	@printf "generating target file     %50s" "$@"
	@set -e; $(LNX_LINKER) $^ $(LIBS) -o $@ 2> $(CCLOG) || touch $(CCERROR)
	@cp $(RCM_CONFIG_FILE) $(LNX_PATH_RELEASE)
	$(log_status)

$(LNX_DEBUG_RCM): $(addprefix $(LNX_DEBUG_OBJS_PATH)/, $(OBJS_RCM))
	$(dir_guard)
	@printf "generating target file     %50s" "$@"
	@set -e; $(LNX_LINKER) $^ $(LIBS) -o $@ 2> $(CCLOG) || touch $(CCERROR)
	@cp $(RCM_CONFIG_FILE) $(LNX_PATH_DEBUG)
	$(log_status)

.SECONDARY: $(OBJS_NO_PATH:.o=.d)

.DEFAULT_GOAL:= arm_release_rcm

arm_release_rcm: $(ARM_RELEASE_RCM)

arm_debug_rcm: $(ARM_DEBUG_RCM)

lnx_release_rcm: $(LNX_RELEASE_RCM)

lnx_debug_rcm: $(LNX_DEBUG_RCM)

all: arm_release_rcm arm_debug_rcm lnx_release_rcm lnx_debug_rcm

clean:
	rm -R -f $(TARGET_PATH)

