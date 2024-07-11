/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 09:28:46 by juliensarda       #+#    #+#             */
/*   Updated: 2024/07/11 09:50:01 by juliensarda      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_unset_1(t_env *env, char **args)
{
	t_env	*current;
	t_env	*prev;
	int		i;
    
	i = 0;
	while (args[i])
	{
		current = env;
		prev = NULL;
		while (current)
		{
			if (ft_strncmp(current->name, args[i], ft_strlen(current->name)) == 0)
			{
				if (prev == NULL)
					env = current->next;
				else
					prev->next = current->next;
				free(current->value);
				free(current->name);
				prev = current;
				break ;
			}
			prev = current;
			current = current->next;
		}
		i++;
	}
}

void	ft_unset(t_data *data, t_shell *shell, char **args)
{
    (void)data;
    ft_unset_1(shell->envp, args);
    ft_unset_1(shell->exp, args);
}
