/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 13:41:20 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/16 10:15:35 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_syntax
{
	int		sq_opened;
	int		dq_opened;
}			t_s;

void	init_syntax_struct(t_s *s)
{
	s->dq_opened = 0;
	s->sq_opened = 0;
}

void	check_sq_dq(t_s *s_s, char c)
{
	if (c == '\'' && s_s->dq_opened == 0)
	{
		if (s_s->sq_opened == 0)
			s_s->sq_opened = 1;
		else
			s_s->sq_opened = 0;
	}
	if (c == '"' && s_s->sq_opened == 0)
	{
		if (s_s->dq_opened == 0)
			s_s->dq_opened = 1;
		else
			s_s->dq_opened = 0;
	}
}

static int	ft_check_charset(const char str, char c, t_s *s_s)
{
	if (str == c && s_s->sq_opened == 0 && s_s->dq_opened == 0)
		return (1);
	return (0);
}

static int	ft_count_word(const char *str, char c)
{
	int	i;
	int	count;
	t_s	s_s;

	init_syntax_struct(&s_s);
	count = 0;
	i = 0;
	while (str[i])
	{
		check_sq_dq(&s_s, str[i]);
		if (ft_check_charset(str[i], c, &s_s) == 0)
			i++;
		else
		{
			while (str[i] && ft_check_charset(str[i], c, &s_s) == 1)
				i++;
			count++;
		}
	}
	return (count);
}

static int	ft_lenword(const char *str, char c, t_s *s_s)
{
	int	i;

	i = 0;
	while (str[i] && ft_check_charset(str[i], c, s_s) == 0)
	{
		check_sq_dq(s_s, str[i]);
		i++;
	}
	return (i);
}

static char	*ft_cpy(const char *str, char c, int n, t_s *s_s)
{
	int		i;
	char	*cpy;

	i = 0;
	cpy = malloc(sizeof(char) * (ft_lenword(str, c, s_s) + 1));
	if (!cpy)
		return (0);
	while (str[i] && i < n)
	{
		cpy[i] = str[i];
		i++;
	}
	cpy[i] = '\0';
	return (cpy);
}

char	**ft_split_quotes(char *s, char c)
{
	int		i;
	int		j;
	char	**str;
	t_s		s_s;

	init_syntax_struct(&s_s);
	i = 0;
	j = 0;
	str = malloc(sizeof(char *) * (ft_count_word(s, c) + 2));
	if (!str)
		return (0);
	while (s[i])
	{
		if (ft_check_charset(s[i], c, &s_s) == 1)
			i++;
		else
		{
			str[j] = ft_cpy(s + i, c, ft_lenword(s + i, c, &s_s), &s_s);
			j++;
			i = i + ft_lenword(s + i, c, &s_s);
		}
	}
	str[j] = 0;
	return (str);
}
