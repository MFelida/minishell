CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -Iinclude -Ilibft

NAME = minishell

SRCFILES = main.c
SRCDIR = src
SRCS = $(adprefix $(SRCDIR)/,$(SRCFILES))

OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/,$(SRCFILES:.c=.o))

LIBFILES = libft.a
LIBDIR = lib
LIBS = $(addprefix $(LIBDIR)/,$(LIBFILES))
LIBFLAGS = -L$(LIBDIR) -lft

.PHONY: all re clean fclean

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(LIBFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(LIBS) | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(LIBS): | $(LIBDIR)
	@git submodule update --init --recursive
	@make -C libft all
	@cd $(LIBDIR); ln -s ../libft/libft.a

$(LIBDIR):
	@mkdir -p $(LIBDIR)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@rm -rfv $(OBJDIR)

fclean: clean
	@rm -rfv $(NAME)

re: fclean all
