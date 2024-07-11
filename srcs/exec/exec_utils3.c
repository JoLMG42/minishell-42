/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 21:06:08 by juliensarda       #+#    #+#             */
/*   Updated: 2024/07/11 21:13:33 by juliensarda      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_wait(t_data *data)
{
	while (data)
	{
		if (data->next == NULL)
		{
			waitpid(data->pid, &data->status, 0);
			if (WIFSIGNALED(data->status))
			{
				data->status = (WTERMSIG(data->status) + 128);
			}
			else
				data->status = WEXITSTATUS(data->status);
			break ;
		}
		waitpid(data->pid, &data->status, 0);
		if (WIFSIGNALED(data->status) && WIFSIGNALED(data->status) != 1)
			data->status = WTERMSIG(data->status) + 128;
		else
			data->status = WEXITSTATUS(data->status);
		data = data->next;
	}
}

void	handle_heredoc(t_shell *shell, t_data *data)
{
	int	i;

	while (data)
	{
		if (data->is_hd)
		{
			i = 0;
			while (data->limiter_hd[i])
			{
				get_tmp_file(data);
				heredoc(data, shell, data->limiter_hd[i++], data->tmpfile_hd);
			}
		}
		data = data->next;
	}
}