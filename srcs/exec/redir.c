/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 09:36:05 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/04 17:01:35 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_child(t_shell *shell)
{
	ft_free_env_list(&(shell->envp));
	ft_free_env_list(&(shell->exp));
	ft_clear_datas(&(shell->datas));
	free(shell);
	exit(EXIT_FAILURE);
}

void	heredoc(t_shell *shell, char *eof, char *file_name)
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
			free_child(shell));
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

void	redir_in(t_shell *shell, char *file_name)
{
	int	fd;

	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening input file");
		free_child(shell);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("Error redirecting stdin");
		free_child(shell);
	}
	close(fd);
}

void	redir_out(t_shell *shell, char *file_name)
{
	int	fd;

	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("Error opening output file");
		free_child(shell);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("Error redirecting stdout");
		close(fd);
		free_child(shell);
	}
	close(fd);
}

void	appen_redir_out(t_shell *shell, char *file_name)
{
	int	fd;

	fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("Error opening output file");
		free_child(shell);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("Error redirecting stdout");
		free_child(shell);
	}
	close(fd);
}

void	handle_redir(t_shell *shell, t_data *datas)
{
	int		i;
	t_data	*current;

	i = 0;
	current = datas;
	while (current)
	{
		if (current->redir_type_in == HD)
			redir_in(shell, current->tmpfile_hd);
		else if (current->redir_type_in == IN)
		{
			redir_in(shell, current->namein);
			i++;
		}
		else if (current->redir_type_out == OUT)
		{
			redir_out(shell, current->nameout);
			i++;
		}
		else if (current->redir_type_out == APPEND)
			appen_redir_out(shell, current->nameout);
		current = current->next;
	}
}
