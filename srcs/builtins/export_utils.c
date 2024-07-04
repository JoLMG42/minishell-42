/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 10:20:37 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/04 11:33:14 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	insert_new_var(t_env *env, t_env *new_var)
{
	t_env	*current;

	current = env;
	while (current->next != env)
		current = current->next;
	current->next = new_var;
	new_var->next = env;
}

t_env	*allocate_new_var()
{
	t_env	*new_var;

	new_var = malloc(sizeof(t_env));
	if (!new_var)
	{
		// ft_error("malloc : ", strerror(errno), 1, data);
		return (NULL);
	}
	return (new_var);
}

int	set_key(t_env *new_var, const char *name)
{
	new_var->name = ft_strdup(name);
	if (!new_var->name)
	{
		// ft_error("malloc : ", strerror(errno), 1);
		free(new_var);
		return (0);
	}
	return (1);
}

int	set_str(t_env *new_var, const char *name, const char *value)
{
	char	*tmp;

	tmp = ft_strjoin(name, "=");
	if (!tmp)
	{
		// ft_error("malloc : ", strerror(errno), 1, data);
		return (0);
	}
	if (value)
		new_var->line = ft_strjoin(tmp, value);
	else
		new_var->line = ft_strdup(tmp);
	free(tmp);
	if (!new_var->line)
	{
		// ft_error("malloc : ", strerror(errno), 1, data);
		free(new_var->name);
		free(new_var);
		return (0);
	}
	return (1);
}

int	set_value(t_env *new_var, const char *value)
{
	if (value)
	{
		new_var->value = ft_strdup(value);
		if (!new_var->value)
		{
			// ft_error("malloc : ", strerror(errno), 1, data);
			free(new_var->line);
			free(new_var->name);
			free(new_var);
			return (0);
		}
	}
	else
		new_var->value = NULL;
	return (1);
}
