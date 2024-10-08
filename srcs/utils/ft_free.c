/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 12:41:15 by jtaravel          #+#    #+#             */
/*   Updated: 2024/07/23 10:23:46 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_clear_one_block_2(t_data *datas)
{
	if (datas->namein)
	{
		freetab(datas->namein);
		datas->namein = NULL;
	}
	if (datas->nameout)
	{
		freetab(datas->nameout);
		datas->nameout = NULL;
	}
	if (datas->limiter_hd)
	{
		freetab(datas->limiter_hd);
		datas->limiter_hd = NULL;
	}
	if (datas->tmpfile_hd)
	{
		if (datas->fdin != -1 && datas->fdin != 0)
			close(datas->fdin);
		free(datas->tmpfile_hd);
		datas->tmpfile_hd = NULL;
	}
	if (datas)
		free(datas);
	datas = NULL;
}

void	ft_clear_one_block(t_data *datas)
{
	if (!datas)
		return ;
	if (datas->cmd)
	{
		free(datas->cmd);
		datas->cmd = NULL;
	}
	if (datas->path)
	{
		free(datas->path);
		datas->path = NULL;
	}
	if (datas->args)
	{
		freetab(datas->args);
		datas->args = NULL;
	}
	ft_clear_one_block_2(datas);
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
	t_env	*tmp;

	if (!env || !*env)
		return ;
	while (env && *env)
	{
		tmp = (*env)->next;
		ft_clear_one_block_env(*env);
		*env = tmp;
	}
}
