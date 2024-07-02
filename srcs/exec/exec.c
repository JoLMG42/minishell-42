/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 09:11:03 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/02 16:40:22 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec(t_shell *shell)
{
	int		i;
	int		stdin_copy;
	int		stdout_copy;
	t_data	*datas;

	datas = shell->datas;
	i = 0;
	if (datas->is_hd && !datas->next)
	{
		while (datas->limiter_hd[i])
		{
			if (!datas->tmpfile_hd)
				get_tmp_file(datas);
			heredoc(datas->limiter_hd[i], datas->tmpfile_hd);
			i++;
			if (datas->limiter_hd)
			{
				unlink(datas->tmpfile_hd);
				free(datas->tmpfile_hd);
			}
		}
	}
	stdin_copy = dup(STDIN_FILENO);
	stdout_copy = dup(STDOUT_FILENO);
	// if (list->next != NULL)
	// {
	// 	data->print_exit = 1;
	// 	exec_pipe(list, data);
	// }
	// else
	exec_simple_cmd(datas);
	dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdin_copy);
	close(stdout_copy);
	unlink(datas->tmpfile_hd);
}
