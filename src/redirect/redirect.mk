REDIR_SRCFILES = redirect.c redir_utils.c

REDIR_OBJS = $(addprefix $(OBJDIR)/,$(REDIR_SRCFILES:.c=.o))

$(OBJDIR)/%.o: $(REDIR_DIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

