/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 14:24:02 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/05 17:19:23 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_dup(t_data *datas)
{
	dup2(datas->fdin, 0);
	dup2(datas->fdout, 1);
}

void	exec_child_process(t_shell *shell, char *path)
{
	t_data	*datas;
	t_data	*current;
	char	**env;

	datas = shell->datas;
	current = datas;
	if (check_if_redir(datas) == 0 || datas->is_hd == 1)
	{
		while (current)
		{
			handle_redir(shell, datas);
			current = current->next;
		}
	}
	env = create_char_env(shell->envp, get_env_list_size(shell->envp));
	// if (!env)
	// {
	// 	free_minishell(data, list);
	// 	exit(EXIT_FAILURE);
	// }
	ft_dup(datas);
	if (path == NULL || execve(path, datas->args, env) == -1)
	{
		perror("execve");
		fprintf(stderr, "minishell: %s: command not found\n", datas->cmd);
	}
	// free_child(datas, shell, 0);
	close(datas->fdin);
	close(datas->fdout);
	// free_minishell(data, list);
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
	pid_t	pid;
	char	*path;

	current = data;
	if (is_built_in(data) != -1)
	{
		if (check_if_redir(current) == 0 || current->is_hd == 1)
		{
			while (current)
			{
				handle_redir(shell, current);
				current = current->next;
			}
		}
		exec_built_in(data, shell);
		return ;
	}
	data->path = get_cmd_path(current, shell);
	path = data->path;
	if (!path)
		return ;
	pid = fork();
	if (pid < 0)
		perror("fork");
	else if (pid == 0)
		exec_child_process(shell, path);
	else
		exec_parent_process(pid);
	free(path);
}
