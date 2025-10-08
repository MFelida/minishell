EXEC_SRCFILES = exec.c signals.c

EXEC_OBJS = $(addprefix $(OBJDIR)/,$(EXEC_SRCFILES:.c=.o))

OBJS += $(EXEC_OBJS)

$(OBJDIR)/%.o: $(EXEC_DIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

