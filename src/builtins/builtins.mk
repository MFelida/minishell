BLTIN_SRCFILES = builtins.c cd.c cd_utils.c echo.c env_bltin.c exit.c export.c pwd.c unset.c bltin_utils.c

BLTIN_OBJS = $(addprefix $(OBJDIR)/,$(BLTIN_SRCFILES:.c=.o))

OBJS += $(BLTIN_OBJS)

$(OBJDIR)/%.o: $(BLTIN_DIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
