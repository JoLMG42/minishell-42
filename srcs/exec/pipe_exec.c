/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 18:15:58 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/04 18:29:05 by jsarda           ###   ########.fr       */
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

int	exec_mid(t_data *data, t_shell *shell, t_data *prev, char *path)
{
	int		pid;
	char	**env;

	env = NULL;
	handle_heredoc(shell, data);
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
		dup2(data->pipes[1], STDOUT_FILENO);
		close(data->pipes[0]);
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
		// free_minishell(data, cmd);
		exit(EXIT_SUCCESS);
	}
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
	handle_heredoc(shell, data);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		dup2(data->pipes[1], STDOUT_FILENO);
		close(data->pipes[0]);
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
		// free_minishell(data, cmd);
		exit(EXIT_SUCCESS);
	}
	close(data->pipes[1]);
	return (pid);
}

int	exec_last(t_data *data, t_shell *shell, t_data *prev, char *path)
{
	int		pid;
	char	**env;

	env = NULL;
	handle_heredoc(shell, data);
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
		// free_minishell(data, cmd);
		exit(EXIT_SUCCESS);
	}
	if (prev)
		close(prev->pipes[0]);
	return (pid);
}

// static int	wait_children(t_shell *shell, t_data *data)
// {
// 	while (data)
// 	{
// 		if (data->next == NULL)
// 		{
// 			waitpid(data->pid, &b->status, 0);
// 			if (WIFSIGNALED(b->status))
// 			{
// 				b->status = (WTERMSIG(b->status) + 128);
// 				ft_check_signal(b->status);
// 			}
// 			else
// 				b->status = WEXITSTATUS(b->status);
// 			break ;
// 		}
// 		waitpid(data->pid, &b->status, 0);
// 		if (WIFSIGNALED(b->status) && WIFSIGNALED(b->status) != 1)
// 			b->status = WTERMSIG(b->status) + 128;
// 		else
// 			b->status = WEXITSTATUS(b->status);
// 		data = data->next;
// 	}
// }

void	exec_pipe(t_data *datas, t_shell *shell)
{
	t_data	*current;
	t_data	*prev;

	current = datas;
	pipe(datas->pipes);
	if (current->fdin != -1)
		exec_first(shell, current);
	else
		close(datas->pipesfd[1]);
	current = current->next;
	while (current && current->next)
	{
		exec_mid(shell, current, 0, datas->pipes[0]);
		current = current->next;
	}
	exec_last(shell, current);
	current = datas;
	wait_children(shell, current);
}
