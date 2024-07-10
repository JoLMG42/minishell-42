/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 10:21:22 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/10 23:45:00 by juliensarda      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	modify_value(t_env *env, const char *value)
{
	free(env->value);
	env->value = ft_strdup(value);
	if (!env->value)
	{
		env->value = NULL;
		return ;
	}
}

void	handle_env_change(t_shell *shell, char **var, char *args)
{
	t_env	*current;
	char	*tmp_name;
	char	*tmp_line;

	current = shell->envp;
	while (current)
	{
		if (ft_strncmp(current->name, var[0], ft_strlen(current->name)) == 0)
		{
			if (count_args(var) < 1)
				break ;
			modify_value(current, var[1]);
			break ;
		}
		current = current->next;
	}
	if (ft_strchr(args, '=') && !current)
	{
		tmp_name = ft_strjoin(var[0], "=");
		tmp_line = ft_strjoin(tmp_name, var[1]);
		ft_lstadd_back_env(&(shell->envp), ft_lstnew_env(tmp_line, var[0],
				var[1]));
		free(tmp_name);
		free(tmp_line);
	}
}

void	handle_exp_change(t_shell *shell, char **var)
{
	t_env	*curr_exp;
	char	*tmp_name;
	char	*tmp_line;

	curr_exp = shell->exp;
	while (curr_exp)
	{
		if (ft_strncmp(curr_exp->name, var[0], ft_strlen(curr_exp->name)) == 0)
		{
			if (count_args(var) < 1)
				break ;
			modify_value(curr_exp, var[1]);
			break ;
		}
		curr_exp = curr_exp->next;
	}
	if (!curr_exp)
	{
		tmp_name = ft_strjoin(var[0], "=");
		tmp_line = ft_strjoin(tmp_name, var[1]);
		ft_lstadd_back_env(&(shell->exp), ft_lstnew_env(tmp_line, var[0],
				var[1]));
		free(tmp_name);
		free(tmp_line);
	}
}

void	ft_export(t_data *data, t_shell *shell, char **args)
{
	int		i;
	char	**var;
	if (!data)
		return ;
	i = 1;
	if (!args[1])
		return (ft_print_exp(shell->exp));
	while (args[i])
	{
		var = ft_split(args[i], '=');
		if (!var || !var[0])
		{
			freetab(var);
			var[1] = NULL;
			var = NULL;
			i++;
			continue ;
		}
		handle_env_change(shell, var, args[i]);
		handle_exp_change(shell, var);
		freetab(var);
		i++;
	}
}
