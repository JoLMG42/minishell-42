/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 18:13:55 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/18 21:39:50 by juliensarda      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_fd_closed(int fd)
{
	return fcntl(fd, F_GETFL) == -1 && errno == EBADF;
}

void	last_child(t_shell *shell, t_data *data, char **env)
{
	manage_sig();
	data->fdin = shell->pipes[0];
	handle_redir(shell, data);
	printf("FD %d is %s\n", data->fdin, is_fd_closed(data->fdin) ? "closed" : "open");
	if (data->fdin != -1 && data->fdin != 0)
	{
		if (dup2(data->fdin, STDIN_FILENO) == -1)
		{
			perror("dup2");
			fprintf(stderr, "vfvfdvdfvdfvfdvfdvvdf fdin = %d\n", data->fdin);
		}
		close(data->fdin);
	}
	if (data->fdout != -1 && data->fdout != 1)
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
	close(shell->pipes[0]);
	close_fd(data);
}
