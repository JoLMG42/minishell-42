/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 12:29:13 by jtaravel          #+#    #+#             */
/*   Updated: 2024/07/04 12:29:13 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strslen_tab_until(char **tab, int pos)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (tab[i] && i <= pos)
	{
		len += ft_strlen(tab[i]);
		// printf("in utils split[i] = %s      len = %d\n", tab[i], len);
		i++;
	}
	return (len - ft_strlen(tab[i - 1]) + 1);
}

int	ft_tablen(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

char	**ft_erase_in_tab(char **tab, int pos, int pos2)
{
	int		i;
	int		j;
	char	**res;

	res = malloc(sizeof(char *) * (ft_tablen(tab)));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (tab[i])
	{
		if (i != pos && i != pos2)
			res[j++] = ft_strdup(tab[i]);
		i++;
	}
	res[j] = 0;
	freetab(tab);
	return (res);
}
size_t	count_args(char **args)
{
	size_t	len;

	len = 0;
	while (args[len])
		len++;
	return (len);
}
