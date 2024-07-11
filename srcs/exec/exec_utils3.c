/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 21:06:08 by juliensarda       #+#    #+#             */
/*   Updated: 2024/07/11 21:40:45 by juliensarda      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_wait(t_data *data)
{
	while (data)
	{
		if (data->next == NULL)
		{
			waitpid(data->pid, &data->status, 0);
			if (WIFSIGNALED(data->status))
			{
				data->status = (WTERMSIG(data->status) + 128);
			}
			else
				data->status = WEXITSTATUS(data->status);
			break ;
		}
		waitpid(data->pid, &data->status, 0);
		if (WIFSIGNALED(data->status) && WIFSIGNALED(data->status) != 1)
			data->status = WTERMSIG(data->status) + 128;
		else
			data->status = WEXITSTATUS(data->status);
		data = data->next;
	}
}

void	handle_heredoc(t_shell *shell, t_data *data)
{
	int	i;

	while (data)
	{
		if (data->is_hd)
		{
			i = 0;
			while (data->limiter_hd[i])
			{
				get_tmp_file(data);
				heredoc(data, shell, data->limiter_hd[i++], data->tmpfile_hd);
			}
		}
		data = data->next;
	}
}

char	*get_cmd_path(t_data *data, t_shell *shell)
{
	char		*path_value;
	char		**paths;
	int			i;
	struct stat	statbuf;
	char		*path;
	char		*cmd_path;

	if (!data || !shell || !data->cmd || !data->cmd[0])
		return (NULL);
	path_value = get_path_value(shell, "PATH");
	if (!path_value)
	{
		if (access(data->cmd, X_OK) == 0)
			return (data->cmd);
		return (NULL);
	}
	paths = ft_split(path_value, ':');
	if (!paths)
	{
		if (access(data->cmd, X_OK) == 0)
			return (data->cmd);
		return (NULL);
	}
	i = 0;
	while (paths[i])
	{
		cmd_path = ft_strjoin(paths[i], "/");
		if (!cmd_path)
			return (freetab(paths), NULL);
		path = ft_strjoin(cmd_path, data->cmd);
		free(cmd_path);
		if (!path)
			return (freetab(paths), NULL);
		if (stat(path, &statbuf) == 0 && (statbuf.st_mode & S_IXUSR))
			return (freetab(paths), path);
		free(path);
		i++;
	}
	if (access(data->cmd, X_OK) == 0)
		return (freetab(paths), ft_strdup(data->cmd));
	return (freetab(paths), NULL);
}