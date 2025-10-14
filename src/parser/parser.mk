PARSE_SRCFILES = parser.c parser_cmd_node.c parser_pipe_node.c parser_redir_nodes.c lexer.c print_tree.c\
				expand.c parser_strerror.c

PARSE_OBJS = $(addprefix $(OBJDIR)/,$(PARSE_SRCFILES:.c=.o))

OBJS += $(PARSE_OBJS)

$(OBJDIR)/%.o: $(PARSE_DIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
