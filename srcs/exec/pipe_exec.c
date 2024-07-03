/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 18:15:58 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/03 19:30:10 by juliensarda      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_heredoc(t_data *data)
{
	int	i;

	i = 0;
	if (data->is_hd)
	{
		while (data->limiter_hd[i])
		{
			get_tmp_file(data);
			heredoc(data->limiter_hd[i], data->tmpfile_hd);
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
			handle_redir(data);
		exec_built_in(shell);
	}
}

void	exec_mid(t_data *data, t_shell *shell, t_data *prev, char *path)
{
	int		pid;
	char	**env;

	env = NULL;
	if (pipe(data->pipes) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	handle_heredoc(data);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		if (prev)
		{
			dup2(prev->pipes[0], STDIN_FILENO);
			close(prev->pipes[0]);
		}
		if (data->next)
			dup2(data->pipes[1], STDOUT_FILENO);
		if (check_if_redir(data) == 0)
			handle_redir(data);
		close(data->pipes[0]);
		close(data->pipes[1]);
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
	close(data->pipes[1]);
	if (prev)
		close(prev->pipes[0]);
}

void	exec_first(t_data *data, t_shell *shell, char *path)
{
	int		pid;
	char	**env;

	env = NULL;
	if (pipe(data->pipes) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	handle_heredoc(data);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		dup2(data->pipes[1], STDOUT_FILENO);
		if (check_if_redir(data) == 0)
			handle_redir(data);
		printf("here\n");
		close(data->pipes[0]);
		close(data->pipes[1]);
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
	close(data->pipes[1]);
}

void	exec_last(t_data *data, t_shell *shell, t_data *prev, char *path)
{
	int		pid;
	char	**env;

	env = NULL;
	handle_heredoc(data);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		if (prev)
		{
			dup2(prev->pipes[0], STDIN_FILENO);
			close(prev->pipes[0]);
		}
		if (check_if_redir(data) == 0)
			handle_redir(data);
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
	if (prev)
		close(prev->pipes[0]);
}

void	exec_pipe(t_data *datas, t_shell *shell)
{
	t_data *current;
	t_data	*prev;
	char *path;

	current = datas;
	prev = NULL;
	if (current)
	{
		path = get_cmd_path(current, shell);
		exec_first(current, shell, path);
		prev = current;
		current = current->next;
	}
	while (current && current->next)
	{
		path = get_cmd_path(current, shell);
		exec_mid(current, shell, prev, path);
		prev = current;
		current = current->next;
	}
	if (current)
	{
		path = get_cmd_path(current, shell);
		exec_last(current, shell, prev, path);
	}
	while (wait(NULL) > 0)
		;
}
