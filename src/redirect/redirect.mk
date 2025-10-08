REDIR_SRCFILES = redirect.c redir_utils.c hear_doc.c

REDIR_OBJS = $(addprefix $(OBJDIR)/,$(REDIR_SRCFILES:.c=.o))

OBJS += $(REDIR_OBJS)

$(OBJDIR)/%.o: $(REDIR_DIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

