/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 10:21:22 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/04 15:04:49 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	create_var(t_env *env, const char *name, const char *value)
// {
// 	t_env	*new_var;

// 	new_var = allocate_new_var();
// 	if (!new_var)
// 		return ;
// 	if (!set_key(new_var, name))
// 		return ;
// 	printf("setting the key...\n");
// 	if (!set_str(new_var, name, value))
// 		return ;
// 	printf("setting the str...\n");
// 	if (!set_value(new_var, value))
// 		return ;
// 	printf("setting the value...\n");
// 	if (shell == NULL)
// 	{
// 		new_var->next = new_var;
// 		shell = new_var;
// 	}
// 	else
// 	{
// 		shell = new_var;
// 		insert_new_var(shell, new_var);
// 	}
// }

void	handle_env_change(t_shell *shell, char **var, char *args)
{
	t_env	*current;

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
		char *tmp_name = ft_strjoin(var[0], "=");
		ft_lstadd_back_env(&(shell->envp), ft_lstnew_env(ft_strjoin(tmp_name, var[1]), var[0], var[1]));
	}
}

void	handle_exp_change(t_shell *shell, char **var)
{
	t_env	*current_exp;

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
		char *tmp_name = ft_strjoin(var[0], "=");
		printf("name is : %s\n", var[0]);
		printf("valeur is : %s\n", var[1]);
		printf("line is : %s\n", ft_strjoin(tmp_name, var[1]));

		ft_lstadd_back_env(&(shell->exp), ft_lstnew_env(ft_strjoin(tmp_name, var[1]), var[0], var[1]));
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
