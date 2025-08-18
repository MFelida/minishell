ENV_SRCFILES = env.c hash.c

ENV_OBJS = $(addprefix $(OBJDIR)/,$(ENV_SRCFILES:.c=.o))

$(OBJDIR)/%.o: $(ENV_DIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

