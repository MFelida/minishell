PARSE_SRCFILES = parser.c parser_cmd_node.c parser_pipe_node.c parser_redir_nodes.c parser_new_free.c lexer.c lexer_checks.c lexer_meta.c lexer_quote.c lexer_utils.c print_tree.c\
				expand.c parser_strerror.c

PARSE_OBJS = $(addprefix $(OBJDIR)/,$(PARSE_SRCFILES:.c=.o))

OBJS += $(PARSE_OBJS)

$(OBJDIR)/%.o: $(PARSE_DIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
