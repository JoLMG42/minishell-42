/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 12:41:15 by jtaravel          #+#    #+#             */
/*   Updated: 2024/07/17 14:55:22 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_clear_one_block(t_data *datas)
{
	if (!datas)
		return ;
	if (datas->cmd)
		free(datas->cmd);
	if (datas->path)
		free(datas->path);
	if (datas->args)
		freetab(datas->args);
	if (datas->namein)
		freetab(datas->namein);
	if (datas->nameout)
		freetab(datas->nameout);
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

void	freetab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		if (tab[i])
			free(tab[i]);
		i++;
	}
	free(tab);
}

void	free_child(t_data *data, t_shell *shell, int exit_status)
{
	ft_free_env_list(&(shell->envp));
	ft_free_env_list(&(shell->exp));
	free(data->path);
	data->path = NULL;
	free_hd_file(&data, 1);
	ft_clear_datas(&(shell->datas));
	free(shell);
	exit(exit_status);
}
