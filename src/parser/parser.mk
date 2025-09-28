PARSE_SRCFILES = parser.c lexer.c print_tree.c expand.c

PARSE_OBJS = $(addprefix $(OBJDIR)/,$(PARSE_SRCFILES:.c=.o))

$(OBJDIR)/%.o: $(PARSE_DIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
