/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 18:15:58 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/02 18:34:07 by jsarda           ###   ########.fr       */
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
			heredoc(cmd->limiter_hd[i], cmd->heredoc_filename);
			i++;
			if (cmd->limiter_hd[i])
				unlink(cmd->heredoc_filename);
		}
	}
}

void	handle_builtin(t_node *list, t_minishell *data)
{
	if (is_built_in(list) != -1)
	{
		if (check_if_redir(list) == 0 || list->here_doc == 1)
			handle_redir(list);
		exec_built_in(data, list);
	}
}

void	exec_mid(t_data *cmd, t_shell *data)
{
	int		pid;
	char	**env;
	char	*path;

	path = NULL;
	env = NULL;
	if (pipe(cmd->pipes) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	handle_heredoc(cmd);
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
		if (cmd->next)
			dup2(cmd->pipes[1], STDOUT_FILENO);
		if (check_if_redir(cmd) == 0)
			handle_redir(cmd);
		close(cmd->pipes[0]);
		close(cmd->pipes[1]);
		if (cmd->cmd && is_built_in(cmd) == -1)
		{
			env = create_char_env(data->env);
			path = get_cmd_path(cmd->cmd, data);
			if (execve(path, cmd->args, env) == -1)
			{
				perror("execve");
				fprintf(stderr, "minishell: %s: command not found\n",
					cmd->tokens_in_node->cmd);
				exit(EXIT_FAILURE);
			}
		}
		else if (is_built_in(cmd) != -1)
			handle_builtin(cmd, data);
		free_minishell(data, cmd);
		exit(EXIT_SUCCESS);
	}
	close(cmd->pipes[1]);
	if (prev)
		close(prev->pipes[0]);
}

void	exec_first(t_data *data, t_shell *shell, char *path)
{
	int		pid;
	char	**env;
	char	*path;

	env = NULL;
	path = NULL;
	if (pipe(shell->pipes) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	handle_heredoc(cmd);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		dup2(cmd->pipes[1], STDOUT_FILENO);
		if (check_if_redir(cmd) == 0)
			handle_redir(cmd);
		close(cmd->pipes[0]);
		close(cmd->pipes[1]);
		if (cmd->cmd && is_built_in(cmd) == -1)
		{
			env = create_char_env(data->env);
			if (execve(path, cmd->args, env) == -1)
			{
				perror("execve");
				fprintf(stderr, "minishell: %s: command not found\n",
					cmd->tokens_in_node->cmd);
				exit(EXIT_FAILURE);
			}
		}
		else if (is_built_in(cmd) != -1)
			handle_builtin(cmd, data);
		free_minishell(data, cmd);
		exit(EXIT_SUCCESS);
	}
	close(cmd->pipes[1]);
}

void	exec_last(t_data *cmd, t_shell *data)
{
	int		pid;
	char	**env;
	char	*path;

	env = NULL;
	path = NULL;
	handle_heredoc(cmd);
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
		if (check_if_redir(cmd) == 0)
			handle_redir(cmd);
		if (cmd->cmd && is_built_in(cmd) == -1)
		{
			env = create_char_env(data->env);
			path = get_cmd_path(cmd->cmd, data);
			if (execve(path, cmd->args, env) == -1)
			{
				perror("execve");
				fprintf(stderr, "minishell: %s: command not found\n",
					cmd->tokens_in_node->cmd);
				exit(EXIT_FAILURE);
			}
		}
		else if (is_built_in(cmd) != -1)
			handle_builtin(cmd, data);
		free_minishell(data, cmd);
		exit(EXIT_SUCCESS);
	}
	if (prev)
		close(prev->pipes[0]);
}

void	exec_pipe(t_shell *shell)
{
	t_data	*data;
	t_data	*prev;
	char *path;

	path = get_cmd_path(shell);
	prev = NULL;
	data = shell->datas;
	if (data)
	{
		exec_first(data, shell, path);
		prev = data;
		data = data->next;
	}
	while (data && data->next)
	{
		exec_mid(data, shell, prev, path);
		prev = data;
		data = data->next;
	}
	if (data)
		exec_last(data, shell, prev, path);
	while (wait(NULL) > 0)
		;
}
