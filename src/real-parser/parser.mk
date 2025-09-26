PARSER_SRCFILES = parser.c

PARSER_OBJS = $(addprefix $(OBJDIR)/,$(PARSER_SRCFILES:.c=.o))

$(OBJDIR)/%.o: $(PARSER_DIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
