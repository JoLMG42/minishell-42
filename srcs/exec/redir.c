/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 09:36:05 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/18 11:11:12 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc(t_data *data, t_shell *shell, char *eof, char *file_name)
{
	int		tmpfd;

	if (!eof)
		return (ft_errors_exec(0, "syntax error near unexpected \
		token `newline'\n", NULL, 2), free(data->tmpfile_hd));
	tmpfd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmpfd == -1)
		return (ft_errors_exec(1, strerror(errno),
				NULL, errno), free(data->tmpfile_hd));
	data->pid = fork();
	if (data->pid == 0)
	{
		manage_sig();
		readline_loop(data, shell, eof, tmpfd);
		exit(0);
	}
	else
	{
		close(tmpfd);
		free(data->tmpfile_hd);
		ft_wait(data);
	}
}

void	redir_in(t_data *data, t_shell *shell, char *file_name)
{
	data->fdin = open(file_name, O_RDONLY);
	if (data->fdin == -1)
		free_child(data, shell, 1);
	if (dup2(data->fdin, STDIN_FILENO) == -1)
	{
		perror("Error redirecting stdin");
		free_child(data, shell, 1);
	}
}

void	redir_out(t_data *data, t_shell *shell, char *file_name)
{
	data->fdout = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->fdout == -1)
		free_child(data, shell, 1);
	if (dup2(data->fdout, STDOUT_FILENO) == -1)
	{
		perror("Error redirecting stdout");
		close(data->fdout);
		free_child(data, shell, 1);
	}
}

void	appen_redir_out(t_data *data, t_shell *shell, char *file_name)
{
	data->fdout = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (data->fdout == -1)
		free_child(data, shell, 1);
	if (dup2(data->fdout, STDOUT_FILENO) == -1)
	{
		perror("Error redirecting stdout");
		free_child(data, shell, 1);
	}
}

void	handle_redir(t_shell *shell, t_data *data)
{
	int	i;

	i = 0;
	if (data->redir_type_in == HD)
		redir_in(data, shell, data->tmpfile_hd);
	while (data->namein && data->namein[i])
	{
		if (data->redir_type_in == IN)
			redir_in(data, shell, data->namein[i]);
		i++;
	}
	i = 0;
	while (data->nameout && data->nameout[i])
	{
		if (data->redir_type_out == OUT)
			redir_out(data, shell, data->nameout[i]);
		else if (data->redir_type_out == APPEND)
			appen_redir_out(data, shell, data->nameout[i]);
		i++;
	}
}
