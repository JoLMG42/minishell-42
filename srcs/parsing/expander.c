/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 16:44:22 by jtaravel          #+#    #+#             */
/*   Updated: 2024/07/05 20:01:12 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_content_env(t_env **env, char *find)
{
	t_env	*tmp;

	if (ft_strncmp(find, "?", ft_strlen(find)) == 0)
		return ((ft_itoa(g_return_satus)));
	tmp = *env;
	while (tmp)
	{
		if (ft_strncmp(find, tmp->name, ft_strlen(find)) == 0)
			return (ft_strdup(tmp->value));
		tmp = tmp->next;
	}
	return (NULL);
}

int	valid_name(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '?')
		return (1);
	return (0);
}

char	*reallocator(char *res, char c, int i)
{
	char	*cpy;

	cpy = NULL;
	if (!res)
	{
		res = malloc(2);
		res[0] = c;
		res[1] = 0;
		return (res);
	}
	if (res)
	{
		cpy = ft_strdup(res);
		free(res);
	}
	res = malloc(sizeof(char) * (ft_strlen(cpy) + 2));
	while (cpy && cpy[i])
	{
		res[i] = cpy[i];
		i++;
	}
	res[i] = c;
	res[i + 1] = 0;
	free(cpy);
	return (res);
}

void	recup_dq_sq_expander(char *str, int i, int *sq, int *dq)
{
	if (str[i] == '\'' && *sq == 0 && *dq == 1)
		;
	else if (str[i] == '\'' && *sq == 0 && *dq == 0)
		*sq = 1;
	else if (str[i] == '\'' && *sq == 1)
		*sq = 0;
	else if (str[i] == '\"' && *dq == 0 && *sq == 1)
		;
	else if (str[i] == '\"' && *dq == 0 && *sq == 0)
		*dq = 1;
	else if (str[i] == '\"' && *dq == 1)
		*dq = 0;
}

char	*expander(char *str, t_env **env, int i, char *res)
{
	int	sq;
	int	dq;
	int	f;
	char	*recup;
	char	*tmp_res;

	sq = 0;
	dq = 0;
	f = 0;
	if (!str || !str[i])
		return (str);
	while (str && str[i])
	{
		recup = NULL;
		recup_dq_sq_expander(str, i, &sq, &dq);
		if (str[i] == '$' && sq == 0)
		{
			i++;
			while (str && str[i] && str[i] != ' ' && valid_name(str[i]) && str[i] != '$')
			{
				recup = reallocator(recup, str[i], 0);
				i++;
			}
			if (str[i] == '$' || str[i] == ' ' || !valid_name(str[i]))
				i--;
			if (recup)
			{

				if (!res)
				{
					res = malloc(1);
					res[0] = 0;
				}
				char *toto = get_content_env(env, recup);
				tmp_res = ft_strjoin(res, toto);
				free(toto);
				free(res);
				res = ft_strdup(tmp_res);
				free(tmp_res);
				free(recup);
				recup = NULL;
				f = 1;
			}
		}
		else
		{
			f = 1;
			res = reallocator(res, str[i], 0);
		}
		if (!recup && f == 0)
			res = reallocator(res, str[i], 0);
		i++;
		f = 0;
	}
	free(str);
	free(recup);
	if (res == NULL)
	{
		free(res);
		res = ft_strdup("");
	}
	return (res);
}
