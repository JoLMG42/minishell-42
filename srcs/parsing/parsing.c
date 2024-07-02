/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 15:09:42 by jtaravel          #+#    #+#             */
/*   Updated: 2024/07/02 16:59:44 by jtaravel         ###   ########.fr       */
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
		printf("\tredir_type = %d\n", datas->redir_type);
		printf("\tnamein = %s\n", datas->namein);
		printf("\tnameout = %s\n", datas->nameout);
		printf("\tis_hd = %d\n", datas->is_hd);
		j = 0;
		printf("\tlimiters:\n");
		while (datas->limiter_hd[j])
		{
			printf("\t\tlimiters[%d] = %s\n", j, datas->limiter_hd[j]);
			j++;
		}		datas = datas->next;
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

char	*add_space(char *input)
{
	char	*res;
	int		count;
	int		j;
	int		i;

	count = count_operators(input);
	if (count == 0)
		return (input);
	res = malloc(sizeof(char) * (ft_strlen(input) + count + 5));
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

t_data	*parse_block(char *str, t_data *datas, t_shell *shell)
{
	int		i;
	char	**split;
	char	*res;

	res = NULL;
	i = 0;
	str = expander(str, &shell->envp, 0, res);
	split = ft_split(str, ' ');
	if (!split)
		return (freetab(split), NULL);
	datas->limiter_hd = malloc(sizeof(char *) * (count_hd_operator(str) + 1));
	while (split[i])
	{
		if (!ft_strncmp(split[i], "<", ft_strlen(split[i])) && i == 0)
		{
			datas->namein = ft_strdup(split[1]);
			datas->cmd = ft_strdup(split[2]);
			datas->redir_type = IN;
			str = ft_erase(str, ft_strlen(str) - ft_strlen(ft_strnstr(str, "<", ft_strlen(str))), 1);
			str = ft_erase(str, ft_strlen(str) - ft_strlen(ft_strnstr(str, datas->namein, ft_strlen(str))), ft_strlen(datas->namein));
			i += 1;
		}
		else if (!ft_strncmp(split[i], ">", ft_strlen(split[i])) && i == 0)
		{
			datas->nameout = ft_strdup(split[1]);
			datas->cmd = ft_strdup(split[2]);
			datas->redir_type = OUT;
			str = ft_erase(str, ft_strlen(str) - ft_strlen(ft_strnstr(str, ">", ft_strlen(str))), 1);
			str = ft_erase(str, ft_strlen(str) - ft_strlen(ft_strnstr(str, datas->nameout, ft_strlen(str))), ft_strlen(datas->nameout));
			i += 1;
		}
		else if (!ft_strncmp(split[i], ">>", ft_strlen(split[i])) && i == 0)
		{
			datas->nameout = ft_strdup(split[1]);
			datas->cmd = ft_strdup(split[2]);
			datas->redir_type = APPEND;
			str = ft_erase(str, ft_strlen(str) - ft_strlen(ft_strnstr(str, ">>", ft_strlen(str))), 2);
			str = ft_erase(str, ft_strlen(str) - ft_strlen(ft_strnstr(str, datas->nameout, ft_strlen(str))), ft_strlen(datas->nameout));
			i += 1;
		}
		else if (!ft_strncmp(split[i], "<<", ft_strlen(split[i])) && i == 0)
		{
			datas->limiter_hd[datas->nb_hd++] = ft_strdup(split[1]);
			datas->cmd = ft_strdup(split[2]);
			datas->redir_type = HD;
			datas->is_hd = 1;
			str = ft_erase(str, ft_strlen(str) - ft_strlen(ft_strnstr(str, "<<", ft_strlen(str))), 2);
			str = ft_erase(str, ft_strlen(str) - ft_strlen(ft_strnstr(str, split[1], ft_strlen(str))), ft_strlen(split[1]));
			i += 1;

		}

		else if (!ft_strncmp(split[i], "<", ft_strlen(split[i])))
		{
			datas->namein = ft_strdup(split[i + 1]);
			datas->cmd = ft_strdup(split[0]);
			datas->redir_type = IN;
			str = ft_erase(str, ft_strlen(str) - ft_strlen(ft_strnstr(str, "<", ft_strlen(str))), 1);
			str = ft_erase(str, ft_strlen(str) - ft_strlen(ft_strnstr(str, datas->namein, ft_strlen(str))), ft_strlen(datas->namein));
			i += 1;
		}
		else if (!ft_strncmp(split[i], ">", ft_strlen(split[i])))
		{
			datas->nameout = ft_strdup(split[i + 1]);
			datas->cmd = ft_strdup(split[i - 1]);
			datas->redir_type = OUT;
			str = ft_erase(str, ft_strlen(str) - ft_strlen(ft_strnstr(str, ">", ft_strlen(str))), 1);
			str = ft_erase(str, ft_strlen(str) - ft_strlen(ft_strnstr(str, datas->nameout, ft_strlen(str))), ft_strlen(datas->nameout));
			i += 1;
		}
		else if (!ft_strncmp(split[i], ">>", ft_strlen(split[i])))
		{
			datas->nameout = ft_strdup(split[i + 1]);
			datas->cmd = ft_strdup(split[i - 1]);
			datas->redir_type = APPEND;
			str = ft_erase(str, ft_strlen(str) - ft_strlen(ft_strnstr(str, ">>", ft_strlen(str))), 2);
			str = ft_erase(str, ft_strlen(str) - ft_strlen(ft_strnstr(str, datas->nameout, ft_strlen(str))), ft_strlen(datas->nameout));
			i += 1;
		}
		else if (!ft_strncmp(split[i], "<<", ft_strlen(split[i])))
		{
			datas->limiter_hd[datas->nb_hd++] = ft_strdup(split[i + 1]);
			datas->cmd = ft_strdup(split[0]);
			datas->redir_type = HD;
			datas->is_hd = 1;
			str = ft_erase(str, ft_strlen(str) - ft_strlen(ft_strnstr(str, "<<", ft_strlen(str))), 2);
			str = ft_erase(str, ft_strlen(str) - ft_strlen(ft_strnstr(str, split[i + 1], ft_strlen(str))), ft_strlen(split[i + 1]));
			i += 1;
		}
		if (!split[i])
			break ;
		i++;
	}
	datas->limiter_hd[datas->nb_hd] = 0;
	datas->args = ft_split(str, ' ');
	if (datas->args && datas->args[0] && !datas->cmd)
		datas->cmd = datas->args[0];
	return (datas);
}

t_data	*pre_init_block()
{
	t_data	*tmp;

	tmp = malloc(sizeof(struct s_data));
	tmp->cmd = NULL;
	tmp->args = NULL;
	tmp->redir_type = 0;
	tmp->fdin = -1;
	tmp->fdout = -1;
	tmp->namein = NULL;
	tmp->tmpfile_hd = NULL;
	tmp->nameout = NULL;
	tmp->is_hd = 0;
	tmp->limiter_hd = NULL;
	tmp->tmpfile_hd = NULL;
	tmp->nb_hd = 0;
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

void    block_add_back(t_data **alst, t_data *new)
{
        t_data   *tmp;

        tmp = *alst;
        if ((*alst))
        {
                tmp = ft_lstlast_block(*alst);
                tmp->next = new;
        }
        else
        {
                *alst = new;
        }
}

int	create_list(char *input, t_data **datas, t_shell *shell)
{
	char	**split;
	int		i;

	*datas = NULL;
	split = ft_split(input, '|');
	if (!split || !split[0])
		return (freetab(split), 0);
	i = 0;
	while (split[i])
	{
		block_add_back(datas, parse_block(split[i], pre_init_block(), shell));
		i++;
	}
	// *datas = tmp_data;
	return (0);
}

int	parse_input(char *input, t_shell *shell)
{
	(void)shell;
	// char	*res;

	// res = NULL;
	// shell->datas = malloc(sizeof(struct s_data));
	input = add_space(input);
	if (!input)
		return (1);
	printf("after add_space = %s\n", input);
	create_list(input, &(shell->datas), shell);

	// DEBUG_print_block(&(shell->datas));	// POUR AFFICHER LES BLOCKS DE COMMANDES

	// input = expander(input, &shell->envp, 0, res);
	// printf("after expander = %s\n", input);
	return (0);

}
