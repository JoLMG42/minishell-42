/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 22:12:43 by juliensarda       #+#    #+#             */
/*   Updated: 2024/07/11 22:13:38 by juliensarda      ###   ########.fr       */
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
			ft_errors_exec(1, strerror(errno), shell, NULL, errno);
			free(data->tmpfile_hd);
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
