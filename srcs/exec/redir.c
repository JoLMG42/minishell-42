/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 09:36:05 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/08 09:54:22 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc(t_data *data, t_shell *shell, char *eof, char *file_name)
{
	char	*buf;

	if (!eof)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `newline'",
			2);
		return 0;
	}
	data->fdin = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->fdin == -1)
		return (perror("Error opening output file in heredoc"),
			free_child(data, shell, 1), 0);
	while (1)
	{
		buf = readline("> ");
		if (buf && !ft_strncmp(eof, buf, INT_MAX) && eof)
		{
			free(buf);
			break ;
		}
		if (buf)
		{
			ft_putendl_fd(buf, data->fdin);
			free(buf);
		}
	}
	return (data->fdout);
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
	return (data->fdout);
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

void	handle_redir(t_shell *shell, t_data *datas)
{
	int		i;
	t_data	*current;

	current = datas;
	while (current)
	{
		i = 0;
		while (current->namein && current->namein[i])
		{
			if (current->redir_type_in == HD)
				datas->fdin = redir_in(datas, shell, current->tmpfile_hd);
			else if (current->redir_type_in == IN)
				datas->fdin = redir_in(datas, shell, current->namein[i]);
			i++;
		}
		i = 0;
		while (current->nameout && current->nameout[i])
		{
			if (current->redir_type_out == OUT)
				datas->fdout = redir_out(datas, shell, current->nameout[i]);
			else if (current->redir_type_out == APPEND)
				datas->fdout = appen_redir_out(datas, shell, current->nameout[i]);
			i++;
		}
		current = current->next;
	}
}
