PASS_SRCFILES = passes.c passes_helpers.c

PASS_OBJS = $(addprefix $(OBJDIR)/,$(PASS_SRCFILES:.c=.o))

$(OBJDIR)/%.o: $(PASS_DIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPASSLAGS) -c -o $@ $<
