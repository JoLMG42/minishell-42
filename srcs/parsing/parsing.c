/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 15:09:42 by jtaravel          #+#    #+#             */
/*   Updated: 2024/07/05 14:03:25 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	DEBUG_print_block(t_data **list)
{
	t_data *datas = *list;
	int i = 0;
	while (datas)
	{
		printf("CMD %d\n", i);
		printf("\tcmd = %s\n", datas->cmd);
		int j = 0;
		printf("\targs:\n");
		while (datas->args[j])
		{
			printf("\t\targs[%d] = %s\n", j, datas->args[j]);
			j++;
		}
		printf("\tredir_type_in = %d\n", datas->redir_type_in);
		printf("\tredir_type_out = %d\n", datas->redir_type_out);
		j = 0;
		printf("\tnamein:\n");
		while (datas->namein && datas->namein[j])
		{
			printf("\t\tnamein[%d] = %s\n", j, datas->namein[j]);
			j++;
		}
		j = 0;
		printf("\tnameout:\n");
		while (datas->nameout && datas->nameout[j])
		{
			printf("\t\tnameout[%d] = %s\n", j, datas->nameout[j]);
			j++;
		}
		printf("\tis_hd = %d\n", datas->is_hd);
		j = 0;
		printf("\tlimiters:\n");
		while (datas->limiter_hd[j])
		{
			printf("\t\tlimiters[%d] = %s\n", j, datas->limiter_hd[j]);
			j++;
		}
		datas = datas->next;
		i++;
	}
}

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
	res = malloc(sizeof(char) * (ft_strlen(input) + count + 5));
	if (!res)
		return (NULL);
	res = add_space_loop(res, input, 0, 0);

	free(input);
	return (res);
}

char	*ft_erase(char *str, int pos, int len)
{
	char	*res;
	int		i;
	int		j;

	res = malloc(sizeof(char) * ((ft_strlen(str) - len) + 2));
	if (!res)
		free(NULL);
	i = -1;
	j = 0;
	while (str[++i])
	{
		if (i == pos)
			i += len;
		if ((size_t)i >= ft_strlen(str))
			break ;
		res[j] = str[i];
		j++;
	}
	free(str);
	res[j] = 0;
	return (res);
}

int	count_hd_operator(char *str)
{
	int i;
	int	c;

	i = 0;
	c = 0;
	while (str[i])
	{
		if (str[i] == '<' && str[i + 1] == '<')
			c++;
		i++;
	}
	return (c);
}

int	count_in_operator(char *str, int i)
{
	int	c;

	c = 0;
	if (i == 0)
	{
		if (str[i] == '<' && str[i + 1] != '<')
			c++;
	}
	else
	{
		if (str[i] == '<' && str[i + 1] != '<' && str[i - 1] != '<')
			c++;
	}
	return (c);
}

int	count_out_operator(char *str, int i)
{
	int	c;

	c = 0;
	if (i == 0)
	{
		if (str[i] == '>' && str[i + 1] != '>')
			c++;
	}
	else
	{
		if (str[i] == '>' && str[i + 1] != '>' && str[i - 1] != '>')
			c++;
	}
	return (c);
}

int	count_redir_operator(char *str, int mode)
{
	int i;
	int	c;

	i = 0;
	c = 0;
	while (str[i])
	{
		if (mode == 1)
			c += count_in_operator(str, i);
		else
			c += count_out_operator(str, i);
		i++;
	}
	return (c);
}

int		recup_second_quote(char *str, int i, int mode)
{
	if (mode == 1)
	{
		while (str[i])
		{
			if (str[i] == '"')
				return (i);
			i++;
		}
	}
	else if (mode == 2)
	{
		while (str[i])
		{
			if (str[i] == '\'')
				return (i);
			i++;
		}
	}
	return (-1);
}

char	*delete_extra_quotes(char *str)
{
	int		i;
	int	sq = 0;
	int	dq = 0;
	int	pos1 = 0;
	int	pos2 = 0;
	
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && sq == 0 && dq == 0)
		{
			pos1 = i;
			sq = 1;
		}
		else if (str[i] == '\"' && dq == 0 && sq == 0)
			dq = 1;
		if (dq == 1 && sq == 0)
		{
			pos2 = recup_second_quote(str, i+1, 1);
			if (pos2 == -1)
				return (NULL);
			else
			{
				str = ft_erase(str, pos1, 1);
				str = ft_erase(str, pos2 - 1, 1);
				dq = 0;
			}
			i += pos2 - 2;
		}
		if (sq == 1 && dq == 0)
		{
			pos2 = recup_second_quote(str, i+1, 2);
			if (pos2 == -1)
				return (NULL);
			else
			{
				str = ft_erase(str, pos1, 1);
				str = ft_erase(str, pos2 - 1, 1);
				sq = 0;
			}
			i += pos2 - 2;
		}
		i++;
	}
	return (str);
}

int	check_valid_quotes(char *str)
{
	int i;
	int	sq;
	int	dq;

	i = 0;
	sq = 0;
	dq = 0;
	while (str[i])
	{
		if (str[i] == '"' && dq == 0 && sq == 0)
			dq = 1;
		else if (str[i] == '\'' && sq == 0 && dq == 0)
			sq = 1;
		else if (str[i] == '"' && dq == 1 && sq == 0)
			dq = 0;
		else if (str[i] == '\'' && sq == 1 && dq == 0)
			sq = 0;
		i++;
	}
	if (sq || dq)
		return (1);
	return (0);
}


int	check_syntaxes(char *str)
{
	if (str == NULL || ft_strncmp(str, " ", ft_strlen(str)) == 0)
		return (1);
	if (check_valid_quotes(str))
		return (1);
	return (0);
}

t_data	*parse_block(char *str, t_data *datas, t_shell *shell)
{
	int		i;
	char	**split;
	int		flag;
	char	*tmp_str;
	//char	*res; // initialised but not used

	//res = NULL;
	i = 0;
	flag = 0;
	split = ft_split_quotes(str, ' ');
	free(str);
	// str = malloc(1);
	// str[0] = 0;
	int k = 0;
	tmp_str = malloc(1);
	tmp_str[0] = '\0';
	while (split[k])
	{
		str = ft_strjoin(tmp_str, split[k]);
		free(tmp_str);
		tmp_str = ft_strjoin(str, " ");
		free(str);
		k++;
	}
	flag = 0;
	if (check_syntaxes(tmp_str))
		return (free(tmp_str), freetab(split), ft_errors_parsing(0, "syntax error\n", shell), NULL);
		
	printf("STR = %s\n", tmp_str);
	// int j =0;
	// while (split[j])
	// {
	// 	printf("split[%d] = %s\n", j, split[j]);
	// 	j++;
	// }
	if (!split)
		return (freetab(split), NULL);
	int nb_redir_in = count_redir_operator(tmp_str, 1);
	int nb_redir_out = count_redir_operator(tmp_str, 0);
	datas->limiter_hd = malloc(sizeof(char *) * (count_hd_operator(tmp_str) + 1));
	if (nb_redir_in)
		datas->namein = malloc(sizeof(char *) * (nb_redir_in + 1));
	if (nb_redir_out)
		datas->nameout = malloc(sizeof(char *) * (nb_redir_out + 1));
	while (split[i])
	{
		flag = 0;
		if (!ft_strncmp(split[i], "<", ft_strlen(split[i])) && i == 0)
		{
			if (!split[i + 1])
				return (ft_errors_parsing(0, "syntax error near unexpected token `newline'\n", shell), NULL);
			datas->namein[datas->nb_in++] = expander(ft_strdup(split[i + 1]), &shell->envp, 0, NULL);
			datas->redir_type_in = IN;
			flag = 1;
		}
		else if (!ft_strncmp(split[i], ">", ft_strlen(split[i])) && i == 0)
		{
			if (!split[i + 1])
				return (ft_errors_parsing(0, "syntax error near unexpected token `newline'\n", shell), NULL);
			datas->nameout[datas->nb_out++] = expander(ft_strdup(split[i + 1]), &shell->envp, 0, NULL);
			datas->redir_type_out = OUT;
			flag = 1;
		}
		else if (!ft_strncmp(split[i], ">>", ft_strlen(split[i])) && i == 0)
		{
			if (!split[i + 1])
				return (ft_errors_parsing(0, "syntax error near unexpected token `newline'\n", shell), NULL);
			datas->nameout[datas->nb_out++] = expander(ft_strdup(split[i + 1]), &shell->envp, 0, NULL);
			datas->redir_type_out = APPEND;
			flag = 1;
		}
		else if (!ft_strncmp(split[i], "<<", ft_strlen(split[i])) && i == 0)
		{
			if (!split[i + 1])
				return (ft_errors_parsing(0, "syntax error near unexpected token `newline'\n", shell), NULL);
			datas->limiter_hd[datas->nb_hd++] = ft_strdup(split[1]);
			datas->redir_type_in = HD;
			datas->is_hd = 1;
			flag = 1;
		}

		else if (!ft_strncmp(split[i], "<", ft_strlen(split[i])))
		{
			if (!split[i + 1])
				return (ft_errors_parsing(0, "syntax error near unexpected token `newline'\n", shell), NULL);
			datas->namein[datas->nb_in++] = expander(ft_strdup(split[i + 1]), &shell->envp, 0, NULL);
			datas->redir_type_in = IN;
			flag = 1;
		}
		else if (!ft_strncmp(split[i], ">", ft_strlen(split[i])))
		{
			if (!split[i + 1])
				return (ft_errors_parsing(0, "syntax error near unexpected token `newline'\n", shell), NULL);
			datas->nameout[datas->nb_out++] = expander(ft_strdup(split[i + 1]), &shell->envp, 0, NULL);
			datas->redir_type_out = OUT;
			flag = 1;
		}
		else if (!ft_strncmp(split[i], ">>", ft_strlen(split[i])))
		{
			if (!split[i + 1])
				return (ft_errors_parsing(0, "syntax error near unexpected token `newline'\n", shell), NULL);
			datas->nameout[datas->nb_out++] = expander(ft_strdup(split[i + 1]), &shell->envp, 0, NULL);
			datas->redir_type_out = APPEND;
			flag = 1;
		}
		else if (!ft_strncmp(split[i], "<<", ft_strlen(split[i])))
		{
			if (!split[i + 1])
				return (ft_errors_parsing(0, "syntax error near unexpected token `newline'\n", shell), NULL);
			datas->limiter_hd[datas->nb_hd++] = ft_strdup(split[i + 1]);
			datas->redir_type_in = HD;
			datas->is_hd = 1;
			flag = 1;
		}
		if (flag == 1)
		{
			split = ft_erase_in_tab(split, i, i + 1);
			i = 0;
			continue ;
		}
		if (!split[i])
			break ;
		i++;
	}
	datas->limiter_hd[datas->nb_hd] = 0;
	if (nb_redir_in)
		datas->namein[datas->nb_in] = 0;
	if (nb_redir_out)
		datas->nameout[datas->nb_out] = 0;
	i = 0;
	datas->args = malloc(sizeof(char *) * (ft_tablen(split) + 1));
	while (split[i])
	{
		datas->args[i] = ft_strdup(split[i]);
		i++;
	}
	datas->args[i] = 0;
	i = 0;
	while (datas->args[i])
	{
		datas->args[i] = expander(datas->args[i], &shell->envp, 0, NULL);
		datas->args[i] = delete_extra_quotes(datas->args[i]);
		// datas->args[i] = ft_wildcards(datas->args[i]);
		i++;
	}
	datas->cmd = ft_strdup(datas->args[0]);
	freetab(split);
	free(tmp_str);
	return (datas);
}

t_data	*pre_init_block()
{
	t_data	*tmp;

	tmp = malloc(sizeof(struct s_data));
	tmp->cmd = NULL;
	tmp->path = NULL;
	tmp->args = NULL;
	tmp->redir_type_in = 0;
	tmp->redir_type_out = 0;
	tmp->fdin = -1;
	tmp->fdout = -1;
	tmp->namein = NULL;
	tmp->tmpfile_hd = NULL;
	tmp->nameout = NULL;
	tmp->is_hd = 0;
	tmp->limiter_hd = NULL;
	tmp->tmpfile_hd = NULL;
	tmp->nb_hd = 0;
	tmp->nb_in = 0;
	tmp->nb_out = 0;
	tmp->next = NULL;
	return (tmp);
}

t_data   *ft_lstlast_block(t_data *lst)
{
        t_data   *tmp;

        while (lst)
        {
                tmp = lst;
                lst = lst->next;
        }
        return (tmp);
}

int	block_add_back(t_data **alst, t_data *new)
{
		t_data   *tmp;

		tmp = *alst;
		if (!new)
			return (1);
		if ((*alst))
		{
				tmp = ft_lstlast_block(*alst);
				tmp->next = new;
		}
		else
		{
				*alst = new;
		}
		return (0);
}

int	create_list(char *input, t_data **datas, t_shell *shell)
{
	char	**split;
	int		i;
	t_data	*tmp_data;

	*datas = NULL;
	split = ft_split(input, '|');
	if (!split || !split[0])
		return (freetab(split), 0);
	i = 0;
	while (split[i])
	{
		tmp_data = pre_init_block();
		if (block_add_back(datas, parse_block(ft_strdup(split[i]), tmp_data, shell)))
			return (free(tmp_data), free(input), freetab(split), 1);
		i++;
	}
	freetab(split);
	return (0);
}

// int	last_check_syntax(t_data **datas)
// {
// 	t_data	*tmp;

// 	tmp = *datas;
// 	while (tmp->next)
// 	{
// 		if (tmp->redir_type_in && tmp->namein == NULL)
// 			return (1);
// 		if (tmp->redir_type_out && tmp->nameout == NULL)
// 			return (1);
// 		tmp = tmp->next;
// 	}
// 	return (0);
// }

int	parse_input(char *input, t_shell *shell)
{
	input = add_space(input);
	if (!input)
		return (1);
	if (create_list(input, &(shell->datas), shell))// && last_check_syntax(&(shell->datas)))
		return (ft_clear_datas(&(shell->datas)), 2);

	DEBUG_print_block(&(shell->datas));	// POUR AFFICHER LES BLOCKS DE COMMANDES
	
	free(input);
	return (0);

}
