/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 09:36:05 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/11 12:36:52 by juliensarda      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc(t_data *data, t_shell *shell, char *eof, char *file_name)
{
	char	*buf;
	int		tmpfd;

	if (!eof)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `newline'",
			2);
		return ;
	}
	tmpfd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmpfd == -1)
		return (perror("Error opening output file in heredoc"), free_child(data,
				shell, 1));
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
}

void	redir_in(t_data *data, t_shell *shell, char *file_name)
{
	data->fdin = open(file_name, O_RDONLY);
	if (data->fdin == -1)
		free_child(data, shell, 1);
}

void	redir_out(t_data *data, t_shell *shell, char *file_name)
{
	data->fdout = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->fdout == -1)
		free_child(data, shell, 1);
}

void	appen_redir_out(t_data *data, t_shell *shell, char *file_name)
{
	data->fdout = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (data->fdout == -1)
		free_child(data, shell, 1);
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
