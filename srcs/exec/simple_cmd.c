/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 14:24:02 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/11 15:21:22 by juliensarda      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_child_process(t_shell *shell, char *path)
{
	t_data	*datas;
	char	**env;

	datas = shell->datas;
	env = create_char_env(shell->envp, get_env_list_size(shell->envp));
	if (!env)
	{
	 	free_child(datas, shell, 1);
	 	exit(EXIT_FAILURE);
	}
	ft_dup(datas);
	if (is_built_in(datas) == -1)
	{
		if (path == NULL || execve(path, datas->args, env) == -1)
			perror("execve");
	}
	else
		exec_built_in(datas, shell);
	free(path);
	exit(0);
}

void	exec_parent_process(pid_t pid)
{
	int	status;
	if (waitpid(pid, &status, 0) == -1)
		perror("waitpid");
}

void	exec_simple_cmd(t_data *data, t_shell *shell)
{
	t_data	*current;

	current = data;
	if (is_built_in(data) != -1 && check_if_redir(data))
		return (exec_built_in(data, shell));
	data->path = get_cmd_path(current, shell);
	if (!data->path && data->cmd)
		return (ft_errors_exec(1, "command not found", shell, data->cmd, 127));
	data->pid = fork();
	if (data->pid < 0)
		perror("fork");
	else if (data->pid == 0)
		exec_child_process(shell, data->path);
	else
	{
		close_fd(data);
		exec_parent_process(data->pid);
	}
	free(data->path);
}
