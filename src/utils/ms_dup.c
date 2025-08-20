// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   ms_dup.c                                          :+:    :+:             //
//                                                    +:+                     //
//   By: mifelida <mifelida@student.email.com>       +#+                      //
//                                                  +#+                       //
//   Created: 2025/08/20 19:34:02 by mifelida     #+#    #+#                  //
//   Updated: 2025/08/20 19:49:43 by mifelida     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include <unistd.h>

//	Intended to be used to direct printf to a different file descriptor
//	If oldfd is non-negative, newfd is first stored in the static
//	and then redirected to olfd
//	If oldfd is negative newfd will be redirected to the stored fd
int	ms_dup(int oldfd, int newfd)
{
	static int	stored_fd = -1;
	int			ret;

	if (oldfd < 0)
	{
		if (stored_fd < 0)
			return (-1);
		ret = dup2(stored_fd, newfd);
		close(stored_fd);
		stored_fd = -1;
		return (ret);
	}
	if (dup(newfd) || dup2(oldfd, newfd))
		return (-1);
	return (0);
}
