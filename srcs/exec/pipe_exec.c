/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 18:15:58 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/04 16:59:12 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	debug_print_pipes(t_data *data, const char *label)
{
	printf("%s: data->pipes[0] = %d, data->pipes[1] = %d\n", label, data->pipes[0], data->pipes[1]);
}

void	handle_heredoc(t_shell *shell, t_data *data)
{
	int	i;

	i = 0;
	if (data->is_hd)
	{
		while (data->limiter_hd[i])
		{
			get_tmp_file(data);
			heredoc(shell, data->limiter_hd[i], data->tmpfile_hd);
			i++;
			if (data->limiter_hd[i])
				unlink(data->tmpfile_hd);
		}
	}
}

void	handle_builtin(t_shell *shell)
{
	t_data *data;

	data = shell->datas;
	if (is_built_in(data) != -1)
	{
		if (check_if_redir(data) == 0 || data->is_hd == 1)
			handle_redir(shell, data);
		exec_built_in(data, shell);
	}
}

int	exec_mid(t_data *data, t_shell *shell, t_data *prev, char *path)
{
	int		pid;
	char	**env;

	env = NULL;
	if (pipe(data->pipes) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	handle_heredoc(shell, data);
	debug_print_pipes(data, "exec_mid (parent before fork)");
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		debug_print_pipes(data, "exec_mid (child after fork)");
		close(data->pipes[0]);
		if (prev)
		{
			dup2(prev->pipes[0], STDIN_FILENO);
			close(prev->pipes[0]);
		}
		//if (data->next)
		dup2(data->pipes[1], STDOUT_FILENO);
		close(data->pipes[1]);
		if (check_if_redir(data) == 0)
			handle_redir(shell, data);
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
		//free_minishell(data, cmd);
		exit(EXIT_SUCCESS);
	}
	debug_print_pipes(data, "exec_mid (parent after fork)");
	close(data->pipes[1]);
	if (prev)
		close(prev->pipes[0]);
	return (pid);
}

int	exec_first(t_data *data, t_shell *shell, char *path)
{
	int		pid;
	char	**env;

	env = NULL;
	if (pipe(data->pipes) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	handle_heredoc(shell, data);
	debug_print_pipes(data, "exec_first (parent before fork)");
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		debug_print_pipes(data, "exec_first (child after fork)");
		close(data->pipes[0]);
		dup2(data->pipes[1], STDOUT_FILENO);
		close(data->pipes[1]);
		if (check_if_redir(data) == 0)
			handle_redir(shell, data);
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
		{
			handle_builtin(shell);
		}
		//free_minishell(data, cmd);
		exit(EXIT_SUCCESS);
	}
	debug_print_pipes(data, "exec_first (parent after fork)");
	close(data->pipes[0]);
	close(data->pipes[1]);
	return (pid);
}

int	exec_last(t_data *data, t_shell *shell, t_data *prev, char *path)
{
	int		pid;
	char	**env;

	env = NULL;
	handle_heredoc(shell, data);
	debug_print_pipes(data, "exec_last (parent before fork)");
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		debug_print_pipes(data, "exec_last (child after fork)");
		if (prev)
		{
			dup2(prev->pipes[0], STDIN_FILENO);
			close(prev->pipes[0]);
		}
		if (check_if_redir(data) == 0)
			handle_redir(shell, data);
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
		//free_minishell(data, cmd);
		exit(EXIT_SUCCESS);
	}
	debug_print_pipes(data, "exec_last (parent after fork)");
	if (prev)
		close(prev->pipes[0]);
	return (pid);
}

static int	wait_children(int pid)
{
	int	wait_status;
	int	error_status;

	while (errno != ECHILD)
		if (wait(&wait_status) == pid && WIFEXITED(wait_status))
			error_status = WEXITSTATUS(wait_status);
	if (pid == -1)
		return (127);
	return (error_status);
}

void	exec_pipe(t_data *datas, t_shell *shell)
{
	t_data *current;
	t_data	*prev;
	char *path;
	int pid;

	current = datas;
	if (current)
	{
		path = get_cmd_path(current, shell);
		pid = exec_first(current, shell, path);
		prev = current;
		current = current->next;
	}
	while (current && current->next)
	{
		path = get_cmd_path(current, shell);
		pid = exec_mid(current, shell, prev, path);
		prev = current;
		current = current->next;
	}
	if (current)
	{
		path = get_cmd_path(current, shell);
		pid = exec_last(current, shell, prev, path);
	}
	wait_children(pid);
}
