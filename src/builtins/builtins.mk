BLTIN_SRCFILES = builtins.c cd.c env_bltin.c exit.c export.c pwd.c unset.c

BLTIN_OBJS = $(addprefix $(OBJDIR)/,$(BLTIN_SRCFILES:.c=.o))

$(OBJDIR)/%.o: $(BLTIN_DIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

