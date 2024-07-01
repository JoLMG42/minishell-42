/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 15:09:42 by jtaravel          #+#    #+#             */
/*   Updated: 2024/07/01 17:52:57 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_operators(char *input)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (input[i])
	{
		if (input[i] == '|' || input[i] == '<' || input[i] == '>'
			|| (input[i] == '<' && input[i + 1] == '<')
			|| (input[i] == '>' && input[i + 1] == '>'))
			count++;
		i++;
	}
	return (count);
}

char	*add_space(char *input)
{
	char	*res;
	int		count;
	int		j;
	int		i;

	count = count_operators(input);
	if (count == 0)
		return (input);
	res = malloc(sizeof(char) * (ft_strlen(input) + count + 1));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
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
	free(input);
	return (res);
}

void	DEBUG_print_block(t_shell *shell)
{
	t_data *datas = shell->datas;
	while (datas->next)
	{
		printf("cmd = %s\n", datas->cmd);
		datas = datas->next;
	}
}

char	*ft_erase(char *str, int pos, int len)
{
	char	*res;
	int		i;
	int		j;

	res = malloc(sizeof(char) * ((ft_strlen(str) - len) + 1));
	if (!res)
		free(NULL);
	i = -1;
	j = 0;
	while (str[++i])
	{
		if (i == pos)
			i += len;
		res[j++] = str[i];
	}
	free(str);
	res[j] = 0;
	return (res);
}

void	parse_block(char *str, t_data *datas)
{
	int		i;
	char	**split;

	i = 0;
	split = ft_split(str, ' ');
	if (!split || !split[0] || !split[1])
		return (freetab(split));
	while (split[i])
	{
		if (!ft_strncmp(split[i], "<", 1) && i == 0)
		{
			datas->namein = ft_strdup(split[1]);
			datas->cmd = ft_strdup(split[2]);
			datas->redir_type = IN;
			i += 2;
		}
		else if (!ft_strncmp(split[i], ">", 1) && i == 0)
		{
			datas->nameout = ft_strdup(split[1]);
			datas->cmd = ft_strdup(split[2]);
			datas->redir_type = OUT;
			str = ft_erase(str, ft_strlen(str) - ft_strlen(ft_strnstr(str, ">", ft_strlen(str))), 1);
			i += 2;
		}
		else if (!ft_strncmp(split[i], ">>", 2) && i == 0)
		{
			datas->nameout = ft_strdup(split[1]);
			datas->cmd = ft_strdup(split[2]);
			datas->redir_type = APPEND;
			i += 2;
		}
		else if (!ft_strncmp(split[i], "<<", 2) && i == 0)
		{
			datas->limiter_hd = ft_strdup(split[1]);
			datas->cmd = ft_strdup(split[2]);
			datas->redir_type = HD;
			datas->is_hd = 1;
			i += 2;

		}
		
		else if (!ft_strncmp(split[i], "<", 1))
		{
			datas->namein = ft_strdup(split[i + 1]);
			datas->cmd = ft_strdup(split[0]);
			datas->redir_type = IN;
			i += 2;
		}
		else if (!ft_strncmp(split[i], ">", 1))
		{
			datas->nameout = ft_strdup(split[i + 1]);
			datas->cmd = ft_strdup(split[0]);
			datas->redir_type = OUT;
			printf("avant = %s\n", str);
			str = ft_erase(str, ft_strlen(str) - ft_strlen(ft_strnstr(str, ">", ft_strlen(str))), 1);
			str = ft_erase(str, ft_strlen(str) - ft_strlen(ft_strnstr(str, datas->nameout, ft_strlen(str))), ft_strlen(datas->nameout));
			printf("apres = %s\n", str);
			i += 2;
		}
		else if (!ft_strncmp(split[i], ">>", 2))
		{
			datas->nameout = ft_strdup(split[i + 1]);
			datas->cmd = ft_strdup(split[0]);
			datas->redir_type = APPEND;
			i += 2;
		}
		else if (!ft_strncmp(split[i], "<<", 2))
		{
			datas->limiter_hd = ft_strdup(split[i + 1]);
			datas->cmd = ft_strdup(split[0]);
			datas->redir_type = HD;
			datas->is_hd = 1;
			i += 2;
		}

		i++;
			
	}
	printf("str fin decoupage = %s\n", str);
	return ;
}

int	create_list(char *input, t_shell *shell)
{
	char	**split;
	int		i;

	shell->datas = malloc(sizeof(struct s_data));
	split = ft_split(input, '|');
	if (!split || !split[0])
		return (freetab(split), 0);
	i = 0;
	while (split[i])
	{
	printf("VSVSFVSFVSF\n");

		parse_block(split[i], shell->datas);
		shell->datas = shell->datas->next;
		i++;
	}
	
	return (0);
}

int	parse_input(char *input, t_shell *shell)
{
	(void)shell;
	char	*res;

	res = NULL;
	input = add_space(input);
	if (!input)
		return (1);
	printf("after add_space = %s\n", input);
	create_list(input, shell);
	// DEBUG_print_list(shell);
	// input = expander(input, &shell->envp, 0, res);
	// printf("after expander = %s\n", input);
	return (0);
	
}
