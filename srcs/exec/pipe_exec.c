/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 18:15:58 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/05 19:12:34 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_lstsize_cmd(t_data *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
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
	if (data->fdin != 0)
		close(data->fdin);
	if (data->fdout != -1)
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
	if (!shell->datas->cmd )
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
	if (data->fdin == 0)
		data->fdin = fd_tmp;
	if (data->fdout == 1)
	{
		close(data->pipes[0]);
		data->fdout = data->pipes[1];
	}
}

void	last_exec(t_shell *shell, t_data *data, char *path)
{
	char	**env;

	env = NULL;
	data->pid = fork();
	if (data->pid == 0)
	{
		if (data->fdin == 0)
			data->fdin = data->pipes[0];
		if (data->cmd && is_built_in(data) == -1)
		{
			env = create_char_env(shell->envp, get_env_list_size(shell->envp));
			ft_dup(data);
			if (execve(path, data->args, env) == -1)
			{
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
		// else if (is_built_in(data) != -1)
		// 	handle_builtin(shell);
		exit_other_child(shell);
	}
	close(data->pipes[0]);
	close_fd(data);
}

void	middle_exec(t_shell *shell, t_data *data, char *path, int fd_tmp)
{
	char	**env;

	env = NULL;
	pipe(data->pipes);
	data->pid = fork();
	if (data->pid == 0)
	{
		manager_mid(data, fd_tmp);
		if (data->cmd && is_built_in(data) == -1)
		{
			env = create_char_env(shell->envp, get_env_list_size(shell->envp));
			ft_dup(data);
			if (execve(path, data->args, env) == -1)
			{
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
		// else if (is_built_in(data) != -1)
		// 	handle_builtin(shell);
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
	data->pid = fork();
	if (data->pid == 0)
	{
		close(data->pipes[0]);
		if (data->fdout == 1)
			data->fdout = data->pipes[1];
		if (data->cmd && is_built_in(data) == -1)
		{
			env = create_char_env(shell->envp, get_env_list_size(shell->envp));
			ft_dup(data);
			if (execve(path, data->args, env) == -1)
			{
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
		// else if (is_built_in(data) != -1)
		// 	handle_builtin(shell);
		exit_first_child(shell);
	}
	close(data->pipes[1]);
	close_fd(data);
}

void	ft_wait(t_data *data)
{
	int	status;

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
			break ;
		}
		waitpid(data->pid, &status, 0);
		if (WIFSIGNALED(status) && WIFSIGNALED(status) != 1)
			status = WTERMSIG(status) + 128;
		else
			status = WEXITSTATUS(status);
		data = data->next;
	}
}

void	exec_pipe(t_shell *shell)
{
	t_data	*head;
	int		i;
	int		num_cmd;

	head = shell->datas;
	i = 0;
	num_cmd = ft_lstsize_cmd(shell->datas);
	pipe(head->pipes);
	if (head->fdin != -1)
	{
		head->path = get_cmd_path(head, shell);
		first_exec(shell, head, head->path);
		free(head->path);
		head->path = NULL;
	}
	else
		close(head->pipes[1]);
	head = head->next;
	while (i < (num_cmd - 2))
	{
		head->path = get_cmd_path(head, shell);
		middle_exec(shell, head, head->path, head->pipes[0]);
		i++;
		free(head->path);
		head->path = NULL;
		head = head->next;
	}
	head->path = get_cmd_path(head, shell);
	last_exec(shell, head, head->path);
	free(head->path);
	head->path = NULL;
	head = shell->datas;
	ft_wait(head);
}
