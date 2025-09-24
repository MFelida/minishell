PF_SRCFILES = fake_parser.c print_tree.c

PF_OBJS = $(addprefix $(OBJDIR)/,$(PF_SRCFILES:.c=.o))

$(OBJDIR)/%.o: $(PF_DIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
