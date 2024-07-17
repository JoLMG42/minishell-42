/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_add_spaces.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:37:21 by jtaravel          #+#    #+#             */
/*   Updated: 2024/07/17 11:38:00 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*add_space_loop(char *res, char *input, int i, int j)
{
	while (input[i])
	{
		if ((input[i] == '<' && input[i + 1] == '<')
			|| (input[i] == '>' && input[i + 1] == '>'))
		{
			res[j++] = ' ';
			res[j++] = input[i++];
			res[j++] = input[i];
			res[j] = ' ';
		}
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
		{
			res[j++] = ' ';
			res[j++] = input[i];
			res[j] = ' ';
		}
		else
			res[j] = input[i];
		j++;
		i++;
	}
	res[j] = 0;
	return (res);
}

char	*add_space(char *input)
{
	char	*res;
	int		count;

	count = count_operators(input);
	if (count == 0)
		return (input);
	res = malloc(sizeof(char) * (ft_strlen(input) + (count * 2) + 1));
	if (!res)
		return (NULL);
	ft_bzero(res, count + 1);
	res = add_space_loop(res, input, 0, 0);
	free(input);
	return (res);
}
