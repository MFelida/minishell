ENV_SRCFILES = env.c env_exit_status.c env_init.c env_utils.c print_env.c hashmap.c hm_utils.c

ENV_OBJS = $(addprefix $(OBJDIR)/,$(ENV_SRCFILES:.c=.o))

OBJS += $(ENV_OBJS)

$(OBJDIR)/%.o: $(ENV_DIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

