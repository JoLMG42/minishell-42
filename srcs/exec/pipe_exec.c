/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 18:15:58 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/08 19:37:30 by juliensarda      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_lstsize_cmd(t_data *lst)
{
	int i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

void handle_heredoc(t_shell *shell, t_data *data)
{
	int i;

	i = 0;
	if (data->is_hd && !data->next)
	{
		while (data)
		{
			while (data->limiter_hd[i])
			{
				if (!data->tmpfile_hd)
					get_tmp_file(data);
				heredoc(data, shell, data->limiter_hd[i++], data->tmpfile_hd);
			}
			data = data->next;
		}
	}
}

void handle_builtin(t_shell *shell)
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

void close_fd(t_data *data)
{
	if (data->fdin != -1)
		close(data->fdin);
	if (data->fdout != -1)
	{
		if (data->next && data->next->fdout != data->fdout)
			return;
		close(data->fdout);
	}
}

void exit_first_child(t_data *data, t_shell *shell)
{
	if (!data->cmd)
	{
		close(shell->pipes[1]);
		free_child(data, shell, 0);
		exit(0);
	}
	close(shell->pipes[1]);
	free_child(data, shell, 0);
	exit(127);
}

void exit_other_child(t_data *data, t_shell *shell)
{
	if (!data->cmd)
	{
		free_child(data, shell, 0);
		exit(1);
	}
	free_child(data, shell, 0);
	if (errno == 13)
		exit(126);
	exit(127);
}

void manager_mid(t_data *data, t_shell *shell, int fd_tmp)
{
	printf("shell pipe[0] shell pipe[1]%d %d\n", shell->pipes[0], shell->pipes[1]);
	data->fdin = fd_tmp;
	close(shell->pipes[0]);
	data->fdout = shell->pipes[1];
}

void first_exec(t_shell *shell, t_data *data, char *path)
{
	char **env;

	env = NULL;
	data->pid = fork();
	if (data->pid == 0)
	{
		close(shell->pipes[0]);
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
		else
			dup2(shell->pipes[1], STDOUT_FILENO);
		if (data->cmd && is_built_in(data) == -1)
		{
			env = create_char_env(shell->envp, get_env_list_size(shell->envp));
			if (execve(path, data->args, env) == -1)
			{
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
		exit_first_child(data, shell);
	}
	close(shell->pipes[1]);
	close_fd(data);
}

void middle_exec(t_shell *shell, t_data *data, char *path, int fd_tmp)
{
	char **env;

	env = NULL;
	pipe(shell->pipes);
	data->pid = fork();
	if (data->pid == 0)
	{
		manager_mid(data, shell, fd_tmp);
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
		else
			dup2(shell->pipes[1], STDOUT_FILENO);
		if (data->cmd && is_built_in(data) == -1)
		{
			env = create_char_env(shell->envp, get_env_list_size(shell->envp));
			if (execve(path, data->args, env) == -1)
			{
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
		exit_other_child(data, shell);
	}
	close(shell->pipes[1]);
	close(fd_tmp);
	close_fd(data);
}

void last_exec(t_shell *shell, t_data *data, char *path)
{
	char **env;

	env = NULL;
	data->pid = fork();
	if (data->pid == 0)
	{
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
			if (execve(path, data->args, env) == -1)
			{
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
		exit_other_child(data, shell);
	}
	close(shell->pipes[0]);
	close_fd(data);
}

void ft_wait(t_data *data)
{
	int status;

	while (data)
	{
		if (data->next == NULL)
		{
			waitpid(data->pid, &status, 0);
			if (WIFSIGNALED(status))
			{
				status = (WTERMSIG(status) + 128);
			}
			else
				status = WEXITSTATUS(status);
			break;
		}
		waitpid(data->pid, &status, 0);
		if (WIFSIGNALED(status) && WIFSIGNALED(status) != 1)
			status = WTERMSIG(status) + 128;
		else
			status = WEXITSTATUS(status);
		data = data->next;
	}
}

void exec_pipe(t_shell *shell)
{
	int i;
	int num_cmd;
	t_data *head;

	head = shell->datas;
	i = 0;
	num_cmd = ft_lstsize_cmd(shell->datas);
	pipe(shell->pipes);
	if (num_cmd > 1)
	{
		head->path = get_cmd_path(head, shell);
		first_exec(shell, head, head->path);
		free(head->path);
		head->path = NULL;
	}
	else
		close(shell->pipes[1]);
	head = head->next;
	while (i < (num_cmd - 2))
	{
		head->path = get_cmd_path(head, shell);
		middle_exec(shell, head, head->path, shell->pipes[0]);
		free(head->path);
		head->path = NULL;
		head = head->next;
		i++;
	}
	head->path = get_cmd_path(head, shell);
	last_exec(shell, head, head->path);
	free(head->path);
	head->path = NULL;
	head = shell->datas;
	ft_wait(head);
}
