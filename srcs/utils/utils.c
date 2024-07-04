//salut je suis un header 42

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

size_t	count_args(char **args)
{
	size_t	len;

	len = 0;
	while (args[len])
		len++;
	return (len);
}
