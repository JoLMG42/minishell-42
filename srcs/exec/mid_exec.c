/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mid_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 17:45:52 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/17 18:13:38 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	mid_child(t_shell *shell, t_data *data, char **env, int fd_tmp)
{
	manage_sig();
	manager_mid(data, shell, fd_tmp);
	handle_redir(shell, data);
	if (data->fdin != -1)
	{
		dup2(data->fdin, STDIN_FILENO);
		close(data->fdin);
	}
	if (data->fdout != -1)
	{
		dup2(data->fdout, STDOUT_FILENO);
		close(data->fdout);
	}
	else
		dup2(shell->pipes[1], STDOUT_FILENO);
	if (data->cmd && is_built_in(data) == -1)
	{
		env = create_char_env(shell->envp, get_env_list_size(shell->envp));
		if (execve(data->path, data->args, env) == -1)
			execve_fail();
	}
	if (is_built_in(data) != -1)
		exec_built_in(data, shell);
	exit_other_child(data, shell);
}

void	middle_exec(t_shell *shell, t_data *data, int fd_tmp)
{
	char	**env;

	env = NULL;
	pipe(shell->pipes);
	data->pid = fork();
	if (data->pid == 0)
		mid_child(shell, data, env, fd_tmp);
	if (data->tmpfile_hd)
	{
		unlink(data->tmpfile_hd);
		free(data->tmpfile_hd);
		data->tmpfile_hd = NULL;
	}
	close(shell->pipes[1]);
	close(fd_tmp);
	close_fd(data);
}
