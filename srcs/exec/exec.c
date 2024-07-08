/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 09:11:03 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/08 12:05:40 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	ft_dup(t_data *datas)
// {
// 	datas->fdin = dup(0);
// 	datas->fdout = dup(1);
// }

// void	ft_dup2(t_data *datas)
// {
// 	dup2(datas->fdin, 0);
// 	dup2(datas->fdout, 1);
// 	close(datas->fdin);
// 	close(datas->fdout);
// 	if (datas->is_hd)
// 	{
// 		unlink(datas->tmpfile_hd);
// 		free(datas->tmpfile_hd);
// 		datas->tmpfile_hd = NULL;
// 	}
// }

int	exec(t_shell *shell)
{
	int		i;
	t_data	*datas;

	datas = shell->datas;
	datas->print_exit = 0;
	if (!datas)
		return (1);
	i = 0;
	if (datas->is_hd && !datas->next)
	{
		while (datas)
		{
			while (datas->limiter_hd[i])
			{
				if (!datas->tmpfile_hd)
					get_tmp_file(datas);
				heredoc(datas, shell, datas->limiter_hd[i++],
					datas->tmpfile_hd);
			}
			datas = datas->next;
		}
	}
	if (datas->next != NULL)
	{
		datas->print_exit = 1;
		exec_pipe(shell);
	}
	else
		exec_simple_cmd(datas, shell);
	return (0);
}
