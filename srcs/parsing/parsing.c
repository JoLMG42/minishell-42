/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 15:09:42 by jtaravel          #+#    #+#             */
/*   Updated: 2024/07/09 17:18:46 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	DEBUG_print_block(t_data **list)
// {
// 	t_data *datas = *list;
// 	int i = 0;
// 	while (datas)
// 	{
// 		printf("CMD %d\n", i);
// 		printf("\tcmd = %s\n", datas->cmd);
// 		int j = 0;
// 		printf("\targs:\n");
// 		while (datas->args[j])
// 		{
// 			printf("\t\targs[%d] = %s\n", j, datas->args[j]);
// 			j++;
// 		}
// 		printf("\tredir_type_in = %d\n", datas->redir_type_in);
// 		printf("\tredir_type_out = %d\n", datas->redir_type_out);
// 		j = 0;
// 		printf("\tnamein:\n");
// 		while (datas->namein && datas->namein[j])
// 		{
// 			printf("\t\tnamein[%d] = %s\n", j, datas->namein[j]);
// 			j++;
// 		}
// 		j = 0;
// 		printf("\tnameout:\n");
// 		while (datas->nameout && datas->nameout[j])
// 		{
// 			printf("\t\tnameout[%d] = %s\n", j, datas->nameout[j]);
// 			j++;
// 		}
// 		printf("\tis_hd = %d\n", datas->is_hd);
// 		j = 0;
// 		printf("\tlimiters:\n");
// 		while (datas->limiter_hd && datas->limiter_hd[j])
// 		{
// 			printf("\t\tlimiters[%d] = %s\n", j, datas->limiter_hd[j]);
// 			j++;
// 		}
// 		datas = datas->next;
// 		i++;
// 	}
// }

char	*ft_recreate_input(char *str, char **tab, char *s, t_shell *shell)
{
	int	k;

	if (!tab)
		return (freetab(tab), free(str), NULL);
	free(str);
	s = malloc(1);
	s[0] = '\0';
	k = -1;
	while (tab[++k])
	{
		str = ft_strjoin(s, tab[k]);
		free(s);
		s = ft_strjoin(str, " ");
		free(str);
	}
	if (check_syntaxes(s))
		return (free(s), freetab(tab),
			ft_errors_parsing(0, "syntax error\n", shell), NULL);
	return (s);
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
	res = malloc(sizeof(char) * (ft_strlen(input) + (count * 2) + 1));
	if (!res)
		return (NULL);
	ft_bzero(res, count + 1);
	res = add_space_loop(res, input, 0, 0);
	free(input);
	return (res);
}

int	create_list(char *input, t_data **datas, t_shell *shell)
{
	char	**split;
	int		i;
	t_data	*tmp_data;

	*datas = NULL;
	split = ft_split(input, '|');
	if (!split || !split[0])
		return (freetab(split), 2);
	i = 0;
	while (split[i])
	{
		tmp_data = pre_init_block();
		if (block_add_back(datas, parse_block(ft_strdup(split[i]),
					tmp_data, shell, 0)))
			return (free(tmp_data), freetab(split), 1);
		i++;
	}
	freetab(split);
	return (0);
}

int	parse_input(char *input, t_shell *shell)
{
	input = add_space(input);
	if (!input)
		return (1);
	if (create_list(input, &(shell->datas), shell))
		return (ft_clear_datas(&(shell->datas)), free(input), 3);
	// DEBUG_print_block(&(shell->datas));
	free(input);
	return (0);
}
