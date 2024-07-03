/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 17:14:00 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/03 19:28:27 by juliensarda      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_key(t_env *env, char *key)
{
	int		index;

	index = 0;
	while (env)
	{
		if (env->name && ft_strncmp(env->name, key, ft_strlen(key)) == 0)
			return (index);
		env = env->next;
		index++;
	}
	return (-1);
}

char	*get_path_value(t_shell *datas, char *key)
{
	int		index;
	t_env	*env;
	int		i;

	i = 0;
	env = datas->envp;
	index = get_key(env, key);
	if (index == -1)
		return (NULL);
	while (i < index)
	{
		env = env->next;
		i++;
	}
	return (env->value);
}

char	*get_cmd_path(t_data *data, t_shell *shell)
{
	char		*path_value;
	char		**paths;
	int			i;
	struct stat	statbuf;
	char		*path;
	char		*cmd_path;

	if (!data || !shell)
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
		return (freetab(paths), data->cmd);
	return (freetab(paths), NULL);
}

char	**create_char_env(t_env *env, int env_size)
{
	t_env	*temp_env;
	char	**dest;
	int		i;

	i = -1;
	if (!env)
		return (NULL);
	dest = malloc(sizeof(char *) * (env_size + 1));
	if (!dest)
		return (NULL);
	temp_env = env;
	while (++i < env_size)
	{
		dest[i] = ft_strdup(temp_env->line);
		if (!dest[i])
		{
			while (i > 0)
				free(dest[--i]);
			return (free(dest), NULL);
		}
		temp_env = temp_env->next;
	}
	dest[i] = NULL;
	return (dest);
}

int	get_env_list_size(t_env *list)
{
	int		count;

	count = 0;
	if (!list)
		return (0);
	while (list)
	{
		count++;
		list = list->next;
	}
	return (count);
}
