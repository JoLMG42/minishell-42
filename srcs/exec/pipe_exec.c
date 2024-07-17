/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 18:15:58 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/17 14:50:33 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fd(t_data *data)
{
	if (data->fdin != -1)
		close(data->fdin);
	if (data->fdout != -1)
	{
		if (data->next && data->next->fdout != data->fdout)
			return ;
		close(data->fdout);
	}
}

void	first_exec(t_shell *shell, t_data *data, char *path)
{
	char	**env;

	env = NULL;
	data->pid = fork();
	if (data->pid == 0)
	{
		signal(SIGINT, handler_sig_cmd);
		signal(SIGQUIT, handler_sig_cmd);
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
		if (is_built_in(data) != -1)
			exec_built_in(data, shell);
		exit_first_child(data, shell);
	}
	if (data->tmpfile_hd)
	{
		unlink(data->tmpfile_hd);
		free(data->tmpfile_hd);
		data->tmpfile_hd = NULL;
	}
	close(shell->pipes[1]);
	close_fd(data);
}

void	middle_exec(t_shell *shell, t_data *data, char *path, int fd_tmp)
{
	char	**env;

	env = NULL;
	pipe(shell->pipes);
	data->pid = fork();
	if (data->pid == 0)
	{
		signal(SIGINT, handler_sig_cmd);
		signal(SIGQUIT, handler_sig_cmd);
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
		if (is_built_in(data) != -1)
			exec_built_in(data, shell);
		exit_other_child(data, shell);
	}
	if (data->tmpfile_hd)
	{
		unlink(data->tmpfile_hd);
		free(data->tmpfile_hd);
		data->tmpfile_hd = NULL;
	}
	close(shell->pipes[1]);
	close(fd_tmp);
	close_fd(data);
}

void	last_exec(t_shell *shell, t_data *data, char *path)
{
	char	**env;

	env = NULL;
	data->pid = fork();
	if (data->pid == 0)
	{
		signal(SIGINT, handler_sig_cmd);
		signal(SIGQUIT, handler_sig_cmd);
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
		if (is_built_in(data) != -1)
			exec_built_in(data, shell);
		exit_other_child(data, shell);
	}
	if (data->tmpfile_hd)
	{
		unlink(data->tmpfile_hd);
		free(data->tmpfile_hd);
		data->tmpfile_hd = NULL;
	}
	close(shell->pipes[0]);
	close_fd(data);
}

void	exec_pipe(t_shell *shell)
{
	int		i;
	int		num_cmd;
	t_data	*head;

	head = shell->datas;
	head->pid = 0;
	i = 0;
	num_cmd = ft_lstsize_cmd(shell->datas);
	pipe(shell->pipes);
	if (num_cmd > 1)
	{
		head->path = get_cmd_path(head, shell);
		if (!head->path && head->cmd)
		{
			close(shell->pipes[0]);
			close(shell->pipes[1]);
			if (head->tmpfile_hd)
			{
				unlink(head->tmpfile_hd);
				free(head->tmpfile_hd);
				head->tmpfile_hd = NULL;
			}
			close_fd(head);
			ft_errors_exec(1, "command not found", head->cmd, 127);
		}
		else
			first_exec(shell, head, head->path);
		// free(head->path);
		// head->path = NULL;
	}
	else
		close(shell->pipes[1]);
	head = head->next;
	while (i < (num_cmd - 2))
	{
		head->path = get_cmd_path(head, shell);
		if (!head->path && head->cmd)
		{
			ft_errors_exec(1, "command not found", head->cmd, 127);
			if (head->tmpfile_hd)
			{
				unlink(head->tmpfile_hd);
				free(head->tmpfile_hd);
				head->tmpfile_hd = NULL;
			}
			close(shell->pipes[1]);
			close_fd(head);
		}
		else
			middle_exec(shell, head, head->path, shell->pipes[0]);
		// free(head->path);
		// head->path = NULL;
		head = head->next;
		i++;
	}
	head->path = get_cmd_path(head, shell);
	if (!head->path && head->cmd)
	{
		ft_errors_exec(1, "command not found", head->cmd, 127);
		if (head->tmpfile_hd)
		{
			unlink(head->tmpfile_hd);
			free(head->tmpfile_hd);
			head->tmpfile_hd = NULL;
		}
		close(shell->pipes[0]);
		close_fd(head);
	}
	else
		last_exec(shell, head, head->path);
	// free(head->path);
	// head->path = NULL;
	head = shell->datas;
	ft_wait(head);
}
