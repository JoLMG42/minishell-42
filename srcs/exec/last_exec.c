/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 18:13:55 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/17 18:19:34 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	last_child(t_shell *shell, t_data *data, char **env)
{
	manage_sig();
	data->fdin = shell->pipes[0];
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
	if (data->cmd && is_built_in(data) == -1)
	{
		env = create_char_env(shell->envp, get_env_list_size(shell->envp));
		if (execve(data->path, data->args, env) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	if (is_built_in(data) != -1)
		exec_built_in(data, shell);
	exit_other_child(data, shell);
}

void	last_exec(t_shell *shell, t_data *data)
{
	char	**env;

	env = NULL;
	data->pid = fork();
	if (data->pid == 0)
		last_child(shell, data, env);
	if (data->tmpfile_hd)
	{
		unlink(data->tmpfile_hd);
		free(data->tmpfile_hd);
		data->tmpfile_hd = NULL;
	}
	close(shell->pipes[0]);
	close_fd(data);
}
