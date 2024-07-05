/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 09:36:05 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/05 14:11:47 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc(t_data *data, t_shell *shell, char *eof, char *file_name)
{
	char	*buf;
	int		fd;

	if (!eof)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `newline'",
			2);
		return ;
	}
	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (perror("Error opening output file in heredoc"),
			free_child(data, shell, 1));
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
			ft_putendl_fd(buf, fd);
			free(buf);
		}
	}
	close(fd);
}

void	redir_in(t_data *data, t_shell *shell, char *file_name)
{
	int	fd;

	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening input file");
		free_child(data, shell, 1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("Error redirecting stdin");
		free_child(data, shell, 1);
	}
	close(fd);
}

void	redir_out(t_data *data, t_shell *shell, char *file_name)
{
	int	fd;

	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("Error opening output file");
		free_child(data, shell, 1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("Error redirecting stdout");
		close(fd);
		free_child(data, shell, 1);
	}
	close(fd);
}

void	appen_redir_out(t_data *data, t_shell *shell, char *file_name)
{
	int	fd;

	fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("Error opening output file");
		free_child(data, shell, 1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("Error redirecting stdout");
		free_child(data, shell, 1);
	}
	close(fd);
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
				redir_in(datas, shell, current->tmpfile_hd);
			else if (current->redir_type_in == IN)
				redir_in(datas, shell, current->namein[i]);
			i++;
		}
		i = 0;
		while (current->nameout && current->nameout[i])
		{
			if (current->redir_type_out == OUT)
				redir_out(datas, shell, current->nameout[i]);
			else if (current->redir_type_out == APPEND)
				appen_redir_out(datas, shell, current->nameout[i]);
			i++;
		}
		current = current->next;
	}
}
