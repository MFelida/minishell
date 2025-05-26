CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -Iinclude

NAME = minishell

SRCFILES = main.c
SRCDIR = src
SRCS = $(adprefix $(SRCDIR)/,$(SRCFILES))

OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/,$(SRCFILES:.c=.o))

.PHONY: all re clean fclean

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@rm -rfv $(OBJDIR)

fclean: clean
	@rm -rfv $(NAME)

re: fclean all
