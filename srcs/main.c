/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 14:14:32 by jtaravel          #+#    #+#             */
/*   Updated: 2024/07/08 09:08:04 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int g_return_satus = 0;

void	sigint_handler(int sig)
{
	(void)sig;
	g_return_satus = 130;
	rl_on_new_line();
	printf("\n");
	rl_replace_line("", 1);
	rl_redisplay();
	return ;
}

int	loop_shell(t_shell *shell)
{
	char	*str;
	int		ret_parsing;

	ret_parsing = 0;
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, sigint_handler);
		str = readline("ft_jsardashell$ ");
		if (!str)
			return (1);
		ret_parsing = parse_input(ft_strdup(str), shell);
		if (ret_parsing == 1)
			return (1);
		else if (ret_parsing == 2)
			g_return_satus = 2;
		if (ret_parsing != 2 && ret_parsing != 3)
			exec(shell);
		add_history(str);
		free(str);
		ft_clear_datas(&(shell->datas));
	}
	return (0);
}

t_env	*ft_lstlast_env(t_env *lst)
{
	t_env	*tmp;

	while (lst)
	{
		tmp = lst;
		lst = lst->next;
	}
	return (tmp);
}

t_env	*ft_lstnew_env(char *line, char *name, char *value)
{
	t_env	*tmp;

	tmp = malloc(sizeof(struct s_env));
	if (!tmp)
		return (0);
	tmp->line = ft_strdup(line);
	tmp->name = ft_strdup(name);
	tmp->value = ft_strdup(value);
	tmp->next = NULL;
	return (tmp);
}

void	ft_lstadd_back_env(t_env **alst, t_env *new)
{
	t_env	*tmp;

	tmp = *alst;
	if ((*alst))
	{
		tmp = ft_lstlast_env(*alst);
		tmp->next = new;
	}
	else
	{
		*alst = new;
	}
}

void	freetab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		if (tab[i])
			free(tab[i]);
		i++;
	}
	free(tab);
}

int	env_init(t_env **env, char **envp)
{
	int		i;
	char	**split;

	*env = NULL;
	//*env = ft_lstnew_env(NULL, NULL, NULL);
	i = 0;
	if (!envp[i])
	{
		// little_env(env);
		return (0);
	}
	while (envp[i])
	{
		split = ft_split(envp[i], '=');
		if (!split || !split[0] || !split[1])
			return (freetab(split), 0);
		ft_lstadd_back_env(env, ft_lstnew_env(envp[i], split[0], split[1]));
		freetab(split);
		split = NULL;
		i++;
	}
	return (0);
}

void	init_shell(t_shell *shell, t_env *env, t_env *exp)
{
	shell->envp = env;
	shell->exp = exp;
}

int	main(int ac, char **av, char **envp)
{
	t_shell	*shell;
	t_env	*env;
	t_env	*exp;

	(void)ac;
	(void)av;
	shell = malloc(sizeof(struct s_shell));
	env_init(&env, envp);
	env_init(&exp, envp);
	init_shell(shell, env, exp);
	// while (env->next)
	// {
	// 	printf("%s\n", env->line);
	// 	env = env->next;
	// }
	loop_shell(shell);
	ft_free_env_list(&(shell->envp));
	ft_free_env_list(&(shell->exp));
	free(shell);
}
