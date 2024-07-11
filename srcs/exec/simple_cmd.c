/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 14:24:02 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/11 22:42:52 by juliensarda      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_and_redir(t_data *datas, t_data *current, t_shell *shell)
{
	if (check_if_redir(datas) == 0 || datas->is_hd == 1)
	{
		while (current)
		{
			handle_redir(shell, datas);
			current = current->next;
		}
	}
}

void	exec_child_process(t_shell *shell, char *path)
{
	t_data	*datas;
	t_data	*current;
	char	**env;

	datas = shell->datas;
	current = datas;
	check_and_redir(datas, current, shell);
	env = create_char_env(shell->envp, get_env_list_size(shell->envp));
	// if (!env)
	// {
	// 	free_minishell(data, list);
	// 	exit(EXIT_FAILURE);
	// }
	ft_dup(datas);
	if (path == NULL || execve(path, datas->args, env) == -1)
		perror("execve");
	// free_child(datas, shell, 0);
	close(datas->fdin);
	close(datas->fdout);
	// free_minishell(data, list);
	free(path);
	exit(0);
}

void	exec_simple_cmd(t_data *data, t_shell *shell)
{
	t_data	*current;

	current = data;
	if (is_built_in(data) != -1)
	{
		check_and_redir(data, current, shell);
		return (exec_built_in(data, shell));
	}
	data->path = get_cmd_path(current, shell);
	if (!data->path && data->cmd)
		return ft_errors_exec(1, "command not found\n", shell, data->cmd, 127);
	if (!data->cmd)
		return ;
	data->pid = fork();
	if (data->pid < 0)
		perror("fork");
	else if (data->pid == 0)
		exec_child_process(shell, data->path);
	else
		ft_wait(data);
}
