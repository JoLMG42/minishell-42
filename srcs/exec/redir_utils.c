/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 22:12:43 by juliensarda       #+#    #+#             */
/*   Updated: 2024/07/18 10:51:25 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	readline_loop(t_data *data, t_shell *shell, char *eof, int fd)
{
	char	*buf;

	while (1)
	{
		buf = readline("> ");
		if (!buf)
		{
			free_child(data, shell, 155);
			ft_errors_exec(1, strerror(errno), NULL, errno);
			break ;
		}
		if (buf && !ft_strncmp(eof, buf, ft_strlen(eof)))
		{
			free(buf);
			break ;
		}
		if (buf)
		{
			ft_putendl_fd(buf, fd);
			free(buf);
		}
	}
}
