/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 10:21:22 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/04 11:30:17 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_var(t_env *env, const char *name, const char *value)
{
	t_env	*new_var;

	new_var = allocate_new_var();
	if (!new_var)
		return ;
	if (!set_key(new_var, name))
		return ;
	if (!set_str(new_var, name, value))
		return ;
	if (!set_value(new_var, value))
		return ;
	if (env == NULL)
	{
		new_var->next = new_var;
		env = new_var;
	}
	else
		insert_new_var(env, new_var);
}

void	handle_env_change(t_env *envp, char **var, char *args)
{
	t_env	*current;

	current = envp;
	while (current)
	{
		if (ft_strncmp(current->name, var[0], ft_strlen(current->name)) == 0)
		{
			if (count_args(var) <= 1)
				break ;
			modify_value(current, var[1]);
			break ;
		}
		current = current->next;
		if (current == envp)
		{
			if (ft_strchr(args, '='))
				create_var(envp, var[0], var[1]);
			break ;
		}
	}
}

void	handle_exp_change(t_env *exp, char **var)
{
	t_env	*current_exp;

	current_exp = exp;
	while (current_exp)
	{
		if (ft_strncmp(current_exp->name, var[0], ft_strlen(current_exp->name)) == 0)
		{
			if (count_args(var) <= 1)
				break ;
			modify_value(current_exp, var[1]);
			break ;
		}
		current_exp = current_exp->next;
		if (current_exp == exp)
		{
			create_var(exp, var[0], var[1]);
			break ;
		}
	}
}

void	ft_export(t_data *data, t_shell *shell)
{
	int		i;
	char	**var;

	if (!data)
		return ;
	i = 1;
	if (!data->args[1])
		return (ft_print_exp(shell->exp));
	while (data->args[1])
	{
		var = ft_split(data->args[1], '=');
		if (!var || !var[0])
		{
			freetab(var);
			var[1] = NULL;
			var = NULL;
			i++;
			continue ;
		}
		handle_env_change(shell->envp, var, data->args[i]);
		handle_exp_change(shell->exp, var);
		freetab(var);
		i++;
	}
}
