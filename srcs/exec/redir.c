/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 09:36:05 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/10 11:44:08 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc(t_data *data, t_shell *shell, char *eof, char *file_name)
{
	char	*buf;
	int		tmpfd;

	if (!eof)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `newline'",
			2);
		return (0);
	}
	tmpfd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmpfd == -1)
		return (perror("Error opening output file in heredoc"), free_child(data,
				shell, 1), 0);
	while (1)
	{
		buf = readline("> ");
		if (!buf)
		{
			ft_putstr_fd("minishell: warning: here-document at line 1 delimited by end-of-file ",
				2);
			ft_putstr_fd("(", 2);
			ft_putendl_fd(eof, 2);
			ft_putstr_fd(")", 2);
			break ;
		}
		if (buf && !ft_strncmp(eof, buf, ft_strlen(eof)))
		{
			free(buf);
			break ;
		}
		if (buf)
		{
			ft_putendl_fd(buf, tmpfd);
			free(buf);
		}
	}
	return (tmpfd);
}

int	redir_in(t_data *data, t_shell *shell, char *file_name)
{
	data->fdin = open(file_name, O_RDONLY);
	if (data->fdin == -1)
	{
		perror("Error opening input file");
		free_child(data, shell, 1);
	}
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
	{
		perror("Error opening output file");
		free_child(data, shell, 1);
	}
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
	{
		perror("Error opening output file");
		free_child(data, shell, 1);
	}
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
