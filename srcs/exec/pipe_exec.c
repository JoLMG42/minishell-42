/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 18:15:58 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/05 13:50:57 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_heredoc(t_shell *shell, t_data *data)
{
	int	i;

	i = 0;
	if (data->is_hd)
	{
		while (data->limiter_hd[i])
		{
			get_tmp_file(data);
			heredoc(data, shell, data->limiter_hd[i], data->tmpfile_hd);
			i++;
			if (data->limiter_hd[i])
				unlink(data->tmpfile_hd);
		}
	}
}

void	handle_builtin(t_shell *shell)
{
	t_data	*data;

	data = shell->datas;
	if (is_built_in(data) != -1)
	{
		if (check_if_redir(data) == 0 || data->is_hd == 1)
			handle_redir(shell, data);
		exec_built_in(data, shell);
	}
}

void	close_fd(t_data *data)
{
	if (data->fdin != STDIN_FILENO)
		close(data->fdin);
	if (data->fdout != STDOUT_FILENO)
	{
		if (data->next && data->next->fdout != data->fdout)
			return ;
		close(data->fdout);
	}
}

void	exit_first_child(t_shell *shell)
{
	if (!shell->datas->cmd)
	{
		close(shell->datas->pipes[1]);
		free_child(shell->datas, shell, 0);
		exit(0);
	}
	close(shell->datas->pipes[1]);
	free_child(shell->datas, shell, 0);
	exit(127);
}

void	exit_other_child(t_shell *shell)
{
	if (!shell->datas->cmd || is_built_in(shell->datas))
	{
		free_child(shell->datas, shell, 0);
		exit(1);
	}
	free_child(shell->datas, shell, 0);
	if (errno == 13)
		exit(126);
	exit(127);
}

void	manager_mid(t_data *data, int fd_tmp)
{
	if (data->fdin == STDIN_FILENO)
		data->fdin = fd_tmp;
	if (data->fdout == STDOUT_FILENO)
	{
		close(data->pipes[0]);
		data->fdout = data->pipes[1];
	}
}

void	last_exec(t_shell *shell, t_data *data, char *path)
{
	char	**env;

	env = NULL;
	handle_heredoc(shell, data);
	data->pid = fork();
	if (data->pid == 0)
	{
		if (data->fdin == STDIN_FILENO)
			data->fdin = data->pipes[0];
		if (data->cmd && is_built_in(data) == -1)
		{
			env = create_char_env(shell->envp, get_env_list_size(shell->envp));
			if (execve(path, data->args, env) == -1)
			{
				perror("execve");
				// fprintf(stderr, "minishell: %s: command not found\n",
				// 	cmd->tokens_in_node->cmd);
				exit(EXIT_FAILURE);
			}
		}
		else if (is_built_in(data) != -1)
			handle_builtin(shell);
		exit_other_child(shell);
	}
	close(data->pipes[0]);
	close_fd(data);
}

void	middle_exec(t_shell *shell, t_data *data, char *path, int fd_tmp)
{
	char	**env;

	env = NULL;
	handle_heredoc(shell, data);
	pipe(data->pipes);
	data->pid = fork();
	if (data->pid == 0)
	{
		manager_mid(data, fd_tmp);
		//pipes_dup(data);
		if (data->cmd && is_built_in(data) == -1)
		{
			env = create_char_env(shell->envp, get_env_list_size(shell->envp));
			if (execve(path, data->args, env) == -1)
			{
				perror("execve");
				// fprintf(stderr, "minishell: %s: command not found\n",
				// 	cmd->tokens_in_node->cmd);
				exit(EXIT_FAILURE);
			}
		}
		else if (is_built_in(data) != -1)
			handle_builtin(shell);
		exit_other_child(shell);
	}
	close(data->pipes[1]);
	close(fd_tmp);
	close_fd(data);
}

void	first_exec(t_shell *shell, t_data *data, char *path)
{
	char	**env;

	env = NULL;
	//handle_heredoc(shell, data);
	data->pid = fork();
	if (data->pid == 0)
	{
		printf("pid in child = %d\n", data->pid);
		close(data->pipes[0]);
		// if (data->fdout == STDOUT_FILENO)
		// 	data->fdout = data->pipes[1];
		//pipes_dup(data);
		if (data->cmd && is_built_in(data) == -1)
		{
			env = create_char_env(shell->envp, get_env_list_size(shell->envp));
			if (execve(path, data->args, env) == -1)
			{
				perror("execve");
				// fprintf(stderr, "minishell: %s: command not found\n",
				// 	cmd->tokens_in_node->cmd);
				exit(EXIT_FAILURE);
			}
		}
		else if (is_built_in(data) != -1)
			handle_builtin(shell);
		exit_first_child(shell);
	}
	printf("pid in parent = %d\n", data->pid);
	close(data->pipes[1]);
	close_fd(data);
}

void	ft_wait(t_data *data)
{
	while (data)
	{
		if (data->next == NULL)
		{
			waitpid(data->pid, NULL, 0);
			break ;
		}
		waitpid(data->pid, NULL, 0);
		data = data->next;
	}
}

void	exec_pipe(t_shell *shell)
{
	t_data	*data;

	data = shell->datas;
	pipe(data->pipes);
	if (data->fdin != -1)
	{
		data->path = get_cmd_path(data, shell);
		printf("path : %s\n", data->path);
		first_exec(shell, data, data->path);
		free(data->path);
	}
	else
		close(data->pipes[1]);
	data = data->next;
	while (data->next && data)
	{
		data->path = get_cmd_path(data, shell);
		middle_exec(shell, data, data->path, data->pipes[0]);
		free(data->path);
		data = data->next;
	}
	if (data->next)
	{
		data->path = get_cmd_path(data, shell);
		last_exec(shell, data, data->path);
		free(data->path);
		data->path = NULL;
		data = shell->datas;
	}
	ft_wait(data);
}
