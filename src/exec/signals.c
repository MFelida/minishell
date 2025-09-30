/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 13:10:51 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/30 13:47:46 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "env.h"
#include "libft.h"

#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>

static void	_sighandler(int signum)
{
	extern sig_atomic_t	g_signal;

	if (!rl_done)
	{
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
		 ms_set_exitstatus(128 + SIGINT);
	}
	else
		g_signal = signum;
}

int	ms_is_interactive(void)
{
	return (isatty(STDIN_FILENO) && isatty(STDERR_FILENO));
}

static	void _reset_signal(void)
{
	extern sig_atomic_t	g_signal;

	g_signal = 0;
}

void	setup_sighandlers(void)
{
	struct sigaction	sa;

	sa = (struct sigaction){.sa_handler = _sighandler,
		.sa_flags = 0, .sa_mask={0}};
	if (ms_is_interactive())
		sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	ft_atexit(_reset_signal);
}

void	forward_sigint(t_cmd_params *params)
{
	while (params)
	{
		if (params->pid > 0)
			kill(params->pid, SIGINT);
		params = params->next;
	}
}

void	rl_sigint(void)
{
	
}
