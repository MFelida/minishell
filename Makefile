CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -xc -Iinclude -Isrc -Ilibft

NAME = minishell

SRCFILES = main.c
SRCDIR = src

OBJDIR = obj

OBJS = $(addprefix $(OBJDIR)/,$(SRCFILES:.c=.o))

BLTIN_DIR = $(SRCDIR)/builtins
include $(BLTIN_DIR)/builtins.mk

CMD_DIR = $(SRCDIR)/command
include $(CMD_DIR)/command.mk

ENV_DIR = $(SRCDIR)/env
include $(ENV_DIR)/env.mk

EXEC_DIR = $(SRCDIR)/exec
include $(EXEC_DIR)/exec.mk

PARSE_DIR = $(SRCDIR)/parser
include $(PARSE_DIR)/parser.mk

REDIR_DIR = $(SRCDIR)/redirect
include $(REDIR_DIR)/redirect.mk

LIBFILES = libft.a
LIBDIR = lib
LIBS = $(addprefix $(LIBDIR)/,$(LIBFILES))
LIBFLAGS = -L$(LIBDIR) -lft -lc -lreadline

.PHONY: all re clean fclean run debug asan

all: CFLAGS += -O2
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFLAGS)

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

ARGS = test/cmd.yml

run: all
	@./$(NAME) $(ARGS)

debug: CFLAGS += -O0 -gdwarf-2
debug: CPPFLAGS += -DDEBUG
debug: fclean $(NAME)

clean:
	@rm -rfv $(OBJDIR)

fclean: clean
	@rm -rfv $(NAME)

asan: CC += -fsanitize=address
asan: debug

re: fclean all
