BLTIN_SRCFILES = builtins.c export.c

BLTIN_OBJS = $(addprefix $(OBJDIR)/,$(BLTIN_SRCFILES:.c=.o))

$(OBJDIR)/%.o: $(BLTIN_DIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

