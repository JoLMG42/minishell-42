/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 09:36:05 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/04 17:23:19 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc(char *eof, char *file_name)
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
	{
		perror("Error opening output file in heredoc");
		exit(EXIT_FAILURE);
	}
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

void	redir_in(char *file_name)
{
	int	fd;

	fd = open(file_name, O_RDONLY);
	printf("filename = %s\n", file_name);
	printf("fd = %d\n", fd);
	if (fd == -1)
	{
		perror("Error opening input file");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("Error redirecting stdin");
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void	redir_out(char *file_name)
{
	int	fd;

	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("Error opening output file");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("Error redirecting stdout");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void	appen_redir_out(char *file_name)
{
	int	fd;

	fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("Error opening output file");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("Error redirecting stdout");
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void	handle_redir(t_data *datas)
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
				redir_in(current->tmpfile_hd);
			else if (current->redir_type_in == IN)
				redir_in(current->namein[i]);
			i++;
		}
		i = 0;
		while (current->nameout && current->nameout[i])
		{
			if (current->redir_type_out == OUT)
				redir_out(current->nameout[i]);
			else if (current->redir_type_out == APPEND)
				appen_redir_out(current->nameout[i]);
			i++;
		}
		current = current->next;
	}
}
