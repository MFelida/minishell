UTILS_SRCFILES = ast_utils.c initialization.c lexing_utils.c linked_list_utils.c parsing_utils.c token_and_var_utils.c

UTILS_OBJS = $(addprefix $(OBJDIR)/,$(UTILS_SRCFILES:.c=.o))

$(OBJDIR)/%.o: $(UTILS_DIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPUTILSLAGS) -c -o $@ $<
