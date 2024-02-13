# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/22 10:08:41 by eralonso          #+#    #+#              #
#    Updated: 2024/02/13 17:27:45 by omoreno-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#<--------------------------------->COLORS<----------------------------------->#
DEF_COLOR	:=	\033[1;97m
WHITE_BOLD	:=	\033[1m
BLACK		:=	\033[1;90m
RED			:=	\033[1;91m
GREEN		:=	\033[1;92m
YELLOW		:=	\033[1;93m
BLUE		:=	\033[1;94m
PINK		:=	\033[1;95m
CIAN		:=	\033[1;96m

#<--------------------------------->NAME<------------------------------------>#
PRENAME		:=	"webserv"
NAME		:=	$(shell echo $(PRENAME) | tr ' ' '_')

#<--------------------------------->VARS<------------------------------------>#
NULL		:=
SPACE		:=	$(NULL) #

#<--------------------------------->ROOTS<----------------------------------->#
SRC_ROOT	:=	src/
OBJ_ROOT	:=	.objs/
DEP_ROOT	:=	.deps/
INC_ROOT	:=	inc/

#<-------------------------------->HEADERS<---------------------------------->#
HEADERS		:=	$(INC_ROOT)

#<---------------------------------->DIRS<----------------------------------->#
SRC_DIRS	:=	./:Tools/:ConfigParse/:Clients/:Headers/:Requests/:Response/\
				:Cgi/:Router/:ConfigResolver/:Events/:Server/
SRC_DIRS	:=	$(subst :,$(SPACE),$(SRC_DIRS))
SRC_DIRS	:=	$(addprefix $(SRC_ROOT),$(SRC_DIRS))
SRC_DIRS	:=	$(subst $(SPACE),:,$(SRC_DIRS))

#<--------------------------------->FILES<---------------------------------->#

FILES		:=	main Sockets WSSignals Receptionist\
				TreeSplit Utils SplitString FolderLs ConfigParser CGIService \
				Directives DirectivesParserInit DirectivesParser ActionMask \
				ServerInit Server ServerGetters ServerSetters ServerOnEvs \
				LocationInit Location Listen ClientMaxBodySize ErrorPage \
				Client ClientGetters ClientSetters ClientEvsSet ClientOnEvs \
				ClientFeats Clients Header Headers \
				Request RequestGetters RequestSetters RequestStMach \
				RequestParse RequestCnfg Requests Response StatusCodes \
				CgiExecutor CgisMap Router RouterCgi RouterProcess \
				ServerFinder ConfigApply ConfigUtils MimeMap \
				EventsTarget Events Log

#<---------------------------------->LANG<---------------------------------->#
LANG		:=	CPP
CFLAGS		:=	-Wall -Wextra -Werror

ifeq ($(LANG),C)
	CC := cc
	SUFFIX := c
else
	CC := c++
	SUFFIX := cpp
	CFLAGS += -std=c++98
endif

#<--------------------------------->SRCS<----------------------------------->#
SRCS		:=	$(addsuffix .$(SUFFIX),$(FILES))

#<----------------------------->OBJS && DEPS<------------------------------->#
OBJS		:=	$(addprefix $(OBJ_ROOT),$(subst .$(SUFFIX),.o,$(SRCS)))
DEPS		:=	$(addprefix $(DEP_ROOT),$(subst .$(SUFFIX),.d,$(SRCS)))

#<-------------------------------->COMANDS<---------------------------------->#
INCLUDE		:=	$(addprefix -I,$(HEADERS))
RM			:=	rm -rf
MKD			:=	mkdir -p
MK			:=	Makefile
MKFLAGS		:=	--no-print-directory
MUTE		:=	&> /dev/null


#<--------------------------------->VPATHS<---------------------------------->#

vpath %.$(SUFFIX) $(SRC_DIRS)
vpath %.d $(DEP_ROOT)

#<-------------------------------->FUNCTIONS<-------------------------------->#

define msg_creating
	printf "$(3)$(1): $(YELLOW)$(2)...$(DEF_COLOR)\n"
endef

create_dir = $(shell $(MKD) $(1))

#<--------------------------------->RULES<----------------------------------->#

all : $(NAME)

.SECONDEXPANSION:

$(DEP_ROOT)%.d : %.$(SUFFIX) $(MK) | $$(call create_dir,$$(dir $$@)) $$(call create_dir,$$(dir $$(OBJ_ROOT)))
	$(call msg_creating,Compiling,$*,$(BLUE))
	$(CC) $(CFLAGS) -MMD -MF $@ $(INCLUDE) -c $< \
		&& mv $(addsuffix .o,$(notdir $*)) $(OBJ_ROOT)
	sed -i "" '1 s|:| $@ :|' $@
	sed -i "" '1 s|^$*|$(OBJ_ROOT)$*|' $@

$(OBJ_ROOT)%.o : %.$(SUFFIX) $(MK) | $$(call create_dir,$$(dir $$@))
	$(call msg_creating,Object,$*,$(PINK))
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(NAME): $(DEPS) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	echo "\n$(GREEN)"$(NAME)" has been compiled\n$(DEF_COLOR)"

bonus : $(NAME)

clean :
	$(RM) $(OBJ_ROOT) $(DEP_ROOT)
	echo ""
	echo "$(RED)All OBJS && DEPS has been removed$(DEF_COLOR)"
	echo ""

fclean :
	$(MAKE) $(MKFLAGS) clean
	$(RM) $(NAME)
	echo ""
	echo "$(RED)Program has been removed$(DEF_COLOR)"

re :
	$(MAKE) $(MKFLAGS) fclean
	$(MAKE) $(MKFLAGS) all
	echo ""
	echo "$(CIAN)$(NAME) has been recompiled$(DEF_COLOR)"

.PHONY : all clean fclean re bonus

.SILENT :

ifeq (,$(filter-out all $(NAME) bonus,$(MAKECMDGOALS)))
 -include $(DEPS)
endif
