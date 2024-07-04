/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 12:41:15 by jtaravel          #+#    #+#             */
/*   Updated: 2024/07/04 14:30:04 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_clear_one_block(t_data *datas)
{
	if (!datas)
		return ;
	if (datas->cmd)
		free(datas->cmd);
	if (datas->args)
		freetab(datas->args);
	if (datas->namein)
		free(datas->namein);
	if (datas->nameout)
		free(datas->nameout);
	if (datas->limiter_hd)
		freetab(datas->limiter_hd);
	if (datas)
		free(datas);
	datas = NULL;
}

void	ft_clear_one_block_env(t_env *env)
{
	if (!env)
		return ;
	if (env->line)
		free(env->line);
	if (env->name)
		free(env->name);
	if (env->value)
		free(env->value);
	if (env)
		free(env);
	env = NULL;
}

void	ft_clear_datas(t_data **datas)
{
	t_data	*tmp;

	if (!datas || !*datas)
		return ;
	while (datas && *datas)
	{
		tmp = (*datas)->next;
		ft_clear_one_block(*datas);
		*datas = tmp;
	}
}

void	ft_free_env_list(t_env **env)
{
	t_env *tmp;
	
	if (!env || !*env)
		return ;
	while (env && *env)
	{
		tmp = (*env)->next;
		ft_clear_one_block_env(*env);
		*env = tmp;
	}
}