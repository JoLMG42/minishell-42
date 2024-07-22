/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 10:41:13 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/22 09:42:32 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_first_child(t_data *data, t_shell *shell)
{
	if (!data->cmd)
	{
		close(shell->pipes[1]);
		free_child(data, shell, 0);
		exit(0);
	}
	close(shell->pipes[1]);
	free_child(data, shell, 0);
	exit(127);
}

void	exit_other_child(t_data *data, t_shell *shell)
{
	if (!data->cmd)
	{
		free_child(data, shell, 0);
		exit(1);
	}
	free_child(data, shell, 0);
	if (errno == 13)
		exit(126);
	exit(127);
}

void	manager_mid(t_data *data, t_shell *shell, int fd_tmp)
{
	data->fdin = fd_tmp;
	close(shell->pipes[0]);
	data->fdout = shell->pipes[1];
}

void	close_fd(t_data *data)
{
	if (data->fdin != -1 && data->fdin != 0)
		close(data->fdin);
	if (data->fdout != -1 && data->fdout != 1)
	{
		if (data->next && data->next->fdout != data->fdout)
			return ;
		close(data->fdout);
	}
}

void	manage_no_path(t_data *head, t_shell *shell, int mod)
{
	ft_errors_exec(1, "command not found", head->cmd, 127);
	head->status = 127;
	if (head->tmpfile_hd)
	{
		unlink(head->tmpfile_hd);
		free(head->tmpfile_hd);
		head->tmpfile_hd = NULL;
	}
	if (mod == 0)
		close(shell->pipes[0]);
	else
		close(shell->pipes[1]);
	close_fd(head);
}
