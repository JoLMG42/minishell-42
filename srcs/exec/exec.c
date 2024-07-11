/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 09:11:03 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/11 12:42:31 by juliensarda      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	open_file(t_data *datas, t_shell *shell)
{
	int i;
	t_data *current;
	
	i = 0;
	current = datas;
	if (current->is_hd && !current->next)
	{
		while (current->limiter_hd[i])
		{
			if (!current->tmpfile_hd)
				get_tmp_file(current);
			heredoc(current, shell, current->limiter_hd[i++], current->tmpfile_hd);
		}
	}
	current = datas;
	if (check_if_redir(datas) == 0 || datas->is_hd == 1)
	{
		while (current)
		{
			handle_redir(shell, datas);
			current = current->next;
		}
	}
	current = datas;
}

int	exec(t_shell *shell)
{
	t_data	*datas;

	datas = shell->datas;
	datas->print_exit = 0;
	open_file(datas, shell);
	if (datas->next)
	{
		datas->print_exit = 1;
		exec_pipe(shell);
	}
	else
		exec_simple_cmd(datas, shell);
	return (0);
}
