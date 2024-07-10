/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 09:11:03 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/10 14:51:24 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		while (datas->limiter_hd[i])
		{
			if (!datas->tmpfile_hd)
				get_tmp_file(datas);
			heredoc(datas, shell, datas->limiter_hd[i++], datas->tmpfile_hd);
		}
	}
	if (datas->next != NULL)
	{
		datas->print_exit = 1;
		exec_pipe(shell);
	}
	else
		exec_simple_cmd(datas, shell);
	if (datas->tmpfile_hd)
		free(datas->tmpfile_hd);
	return (0);
}
