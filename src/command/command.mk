CMD_SRCFILES = command.c find_bin.c params.c

CMD_OBJS = $(addprefix $(OBJDIR)/,$(CMD_SRCFILES:.c=.o))

OBJS += $(CMD_OBJS)

$(OBJDIR)/%.o: $(CMD_DIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
