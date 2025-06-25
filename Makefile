CC = cc
CFLAGS = -Wall -Wextra -Werror -xc
CPPFLAGS = -Iinclude -Ilibft

NAME = minishell

SRCFILES = main.c
SRCDIR = src

OBJDIR = obj

REDIR_DIR = $(SRCDIR)/redirect
include $(REDIR_DIR)/redirect.mk

CMD_DIR = $(SRCDIR)/command
include $(CMD_DIR)/command.mk

PF_DIR = $(SRCDIR)/fake-parser
include $(PF_DIR)/fake-parser.mk

OBJS = $(addprefix $(OBJDIR)/,$(SRCFILES:.c=.o)) \
	$(REDIR_OBJS) $(CMD_OBJS) $(PF_OBJS)

LIBFILES = libft.a
LIBDIR = lib
LIBS = $(addprefix $(LIBDIR)/,$(LIBFILES))
LIBFLAGS = -L$(LIBDIR) -lft

.PHONY: all re clean fclean debug

all: CFLAGS += -O2
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(LIBFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(LIBS) | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(LIBS): | $(LIBDIR)
	@git submodule update --init --recursive
	@make -C libft all
	@cd $(LIBDIR); ln -sf ../libft/libft.a

$(LIBDIR):
	@mkdir -p $(LIBDIR)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

debug: CFLAGS += -O0 -g
debug: fclean $(NAME)

clean:
	@rm -rfv $(OBJDIR)

fclean: clean
	@rm -rfv $(NAME)

re: fclean all
