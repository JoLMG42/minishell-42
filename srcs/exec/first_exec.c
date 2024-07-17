/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_exec..c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 17:24:07 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/17 17:45:38 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	first_child(t_shell *shell, t_data *data, char **env, char *path)
{
	manage_sig();
	close(shell->pipes[0]);
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
		if (execve(path, data->args, env) == -1)
			execve_fail();
	}
	if (is_built_in(data) != -1)
		exec_built_in(data, shell);
	exit_first_child(data, shell);
}

void	first_exec(t_shell *shell, t_data *data, char *path)
{
	char	**env;

	env = NULL;
	data->pid = fork();
	if (data->pid == 0)
		first_child(shell, data, env, path);
	if (data->tmpfile_hd)
	{
		unlink(data->tmpfile_hd);
		free(data->tmpfile_hd);
		data->tmpfile_hd = NULL;
	}
	close(shell->pipes[1]);
	close_fd(data);
}
