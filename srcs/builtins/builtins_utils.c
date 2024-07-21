/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 15:13:32 by juliensarda       #+#    #+#             */
/*   Updated: 2024/07/21 15:13:52 by juliensarda      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_identifier(const char *name)
{
	int	i;

	i = 0;
	if (!name[0])
		return (0);
	if (name[0] >= '0' && name[0] <= '9')
		return (0);
	while (name[i])
	{
		if (!((name[i] >= 'a' && name[i] <= 'z')
				|| (name[i] >= 'A' && name[i] <= 'Z')
				|| (name[i] >= '0' && name[i] <= '9')
				|| (name[i] == '_')))
			return (0);
		i++;
	}
	return (1);
}
