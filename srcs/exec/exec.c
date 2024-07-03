/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 09:11:03 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/03 18:04:43 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_dup(t_data *datas)
{
	datas->fdin = dup(0);
	datas->fdout = dup(1);
}

void	ft_dup2(t_data *datas)
{
	dup2(datas->fdin, 0);
	dup2(datas->fdout, 1);
	close(datas->fdin);
	close(datas->fdout);
	if (datas->tmpfile_hd)
		unlink(datas->tmpfile_hd);
}

void	exec(t_shell *shell)
{
	int		i;
	t_data	*datas;

	datas = shell->datas;
	i = 0;
	if (datas->is_hd && !datas->next)
	{
		while (datas->limiter_hd[i])
		{
			if (!datas->tmpfile_hd)
				get_tmp_file(datas);
			heredoc(datas->limiter_hd[i++], datas->tmpfile_hd);
			unlink(datas->tmpfile_hd);
		}
	}
	ft_dup(datas);
	if (datas->next != NULL)
	{
		// data->print_exit = 1;
		exec_pipe(datas, shell);
	}
	// else
	// 	exec_simple_cmd(data, shell);
	ft_dup2(datas);
}
