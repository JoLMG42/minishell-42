/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 10:42:27 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/04 10:42:41 by jsarda           ###   ########.fr       */
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
