EXEC_SRCFILES = exec.c

EXEC_OBJS = $(addprefix $(OBJDIR)/,$(EXEC_SRCFILES:.c=.o))

$(OBJDIR)/%.o: $(EXEC_DIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

