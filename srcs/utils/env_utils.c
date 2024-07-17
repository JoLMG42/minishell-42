/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 11:07:49 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/17 15:23:27 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_swap(t_env *exp)
{
	char	*temp_name;
	char	*temp_value;
	int		swapped;

	swapped = 0;
	if (exp == NULL || exp->next == NULL)
		return (swapped);
	if (strcmp(exp->name, exp->next->name) > 0)
	{
		temp_name = exp->name;
		temp_value = exp->value;
		exp->name = exp->next->name;
		exp->value = exp->next->value;
		exp->next->name = temp_name;
		exp->next->value = temp_value;
		swapped = 1;
		return (swapped);
	}
	return (swapped);
}

void	sort_ascii(t_env *env)
{
	int		swapped;
	t_env	*exp;

	exp = env;
	if (env == NULL)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		while (exp)
		{
			swapped = ft_swap(exp);
			exp = exp->next;
		}
	}
}

void	print_env(t_env *env)
{
	if (!env)
		return ;
	while (env)
	{
		printf("%s=%s\n", env->name, env->value);
		env = env->next;
	}
}

void	ft_print_exp(t_env *exp)
{
	if (exp == NULL)
		return ;
	sort_ascii(exp);
	print_env(exp);
}
