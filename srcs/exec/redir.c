/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 09:36:05 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/11 22:15:52 by juliensarda      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc(t_data *data, t_shell *shell, char *eof, char *file_name)
{
	int		tmpfd;

	if (!eof)
		return (ft_errors_exec(0, "syntax error near unexpected \
		token `newline'\n", shell, NULL, 2), free(data->tmpfile_hd));
	tmpfd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmpfd == -1)
		return (ft_errors_exec(1, strerror(errno),
				shell, NULL, errno), free(data->tmpfile_hd));
	readline_loop(data, shell, eof, tmpfd);
}

int	redir_in(t_data *data, t_shell *shell, char *file_name)
{
	data->fdin = open(file_name, O_RDONLY);
	if (data->fdin == -1)
		free_child(data, shell, 1);
	if (dup2(data->fdin, STDIN_FILENO) == -1)
	{
		perror("Error redirecting stdin");
		free_child(data, shell, 1);
	}
	return (data->fdin);
}

int	redir_out(t_data *data, t_shell *shell, char *file_name)
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
	return (data->fdout);
}

int	appen_redir_out(t_data *data, t_shell *shell, char *file_name)
{
	data->fdout = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (data->fdout == -1)
		free_child(data, shell, 1);
	if (dup2(data->fdout, STDOUT_FILENO) == -1)
	{
		perror("Error redirecting stdout");
		free_child(data, shell, 1);
	}
	return (data->fdout);
}

void	handle_redir(t_shell *shell, t_data *data)
{
	int	i;

	i = 0;
	if (data->redir_type_in == HD)
		data->fdin = redir_in(data, shell, data->tmpfile_hd);
	while (data->namein && data->namein[i])
	{
		if (data->redir_type_in == IN)
			data->fdin = redir_in(data, shell, data->namein[i]);
		i++;
	}
	i = 0;
	while (data->nameout && data->nameout[i])
	{
		if (data->redir_type_out == OUT)
			data->fdout = redir_out(data, shell, data->nameout[i]);
		else if (data->redir_type_out == APPEND)
			data->fdout = appen_redir_out(data, shell, data->nameout[i]);
		i++;
	}
}
