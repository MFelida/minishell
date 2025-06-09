REDIR_SRCFILES = command.c find_bin.c

REDIR_OBJS = $(addprefix $(OBJDIR)/,$(REDIR_SRCFILES:.c=.o))

$(OBJDIR)/%.o: $(REDIR_DIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

