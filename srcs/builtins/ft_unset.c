/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 09:28:46 by juliensarda       #+#    #+#             */
/*   Updated: 2024/07/17 17:10:44 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_unset(char *value, char *name, char *line)
{
	free(value);
	free(name);
	free(line);
}

int	hatenorm(char *name, char *args)
{
	if (ft_strncmp(name, args, ft_strlen(name)) == 0)
		return (0);
	else
		return (1);
}

void	ft_unset_1(t_env *env, char **args, int i)
{
	t_env	*current;
	t_env	*prev;

	while (args[i])
	{
		current = env;
		prev = NULL;
		while (current)
		{
			if (hatenorm(current->name, args[i]) == 0)
			{
				if (prev == NULL)
					env = current->next;
				else
					prev->next = current->next;
				free_unset(current->value, current->name, current->line);
				prev = current;
				free(prev);
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
	int	i;

	(void)data;
	i = 0;
	ft_unset_1(shell->envp, args, i);
	ft_unset_1(shell->exp, args, i);
}
