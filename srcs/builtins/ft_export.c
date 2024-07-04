/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 10:21:22 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/04 15:16:06 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	modify_value(t_env *env, const char *value)
{
	free(env->value);
	env->value = ft_strdup(value);
	if (!env->value)
	{
		perror("strdup");
		return ;
	}
}

void	handle_env_change(t_shell *shell, char **var, char *args)
{
	t_env	*current;
	char	*tmp_name;

	current = shell->envp;
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
	}
	if (ft_strchr(args, '=') && !current)
	{
		tmp_name = ft_strjoin(var[0], "=");
		ft_lstadd_back_env(&(shell->envp), ft_lstnew_env(ft_strjoin(tmp_name,
					var[1]), var[0], var[1]));
	}
}

void	handle_exp_change(t_shell *shell, char **var)
{
	t_env	*current_exp;
	char	*tmp_name;

	current_exp = shell->exp;
	while (current_exp)
	{
		if (ft_strncmp(current_exp->name, var[0],
				ft_strlen(current_exp->name)) == 0)
		{
			if (count_args(var) <= 1)
				break ;
			modify_value(current_exp, var[1]);
			break ;
		}
		current_exp = current_exp->next;
	}
	if (!current_exp)
	{
		tmp_name = ft_strjoin(var[0], "=");
		printf("name is : %s\n", var[0]);
		printf("valeur is : %s\n", var[1]);
		printf("line is : %s\n", ft_strjoin(tmp_name, var[1]));
		ft_lstadd_back_env(&(shell->exp), ft_lstnew_env(ft_strjoin(tmp_name,
					var[1]), var[0], var[1]));
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
	while (data->args[i])
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
		handle_env_change(shell, var, data->args[i]);
		handle_exp_change(shell, var);
		freetab(var);
		i++;
	}
}
