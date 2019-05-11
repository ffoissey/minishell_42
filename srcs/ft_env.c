/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 18:29:20 by ffoissey          #+#    #+#             */
/*   Updated: 2019/02/27 19:49:08 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_unsetenv(t_list_ex **lst, char **tab)
{
	int		i;

	i = 0;
	while (tab[i])
		ft_lstexpop(lst, tab[i++]);
}

void		ft_setenv(t_list_ex **lst, char **tab)
{
	char	*tmp;
	char	*tmp2;

	if (!tab || !tab[0])
		return ;
	if (tab[0] && (tmp = ft_strchr(tab[0], '=')))
	{
		tmp2 = ft_strcdup(tab[0], '=');
		tmp = ft_strdup(tmp + 1);
		if (tmp && *(tmp) == '~')
			if (!ft_tilde(&tmp, *lst, NULL, 0))
				return ;
		ft_lst_changevalue(lst, tmp2, tmp);
		ft_strdel(&tmp2);
		ft_strdel(&tmp);
	}
	else if (tab[0] && tab[1])
	{
		tmp2 = ft_strcdup(tab[0], '=');
		if (tab[1][0] == '~')
			if (!ft_tilde(tab, *lst, NULL, 1))
				return ;
		ft_lst_changevalue(lst, tmp2, tab[1]);
		ft_strdel(&tmp2);
	}
}

static int	ft_env_options(char **tab, int *i, int *flag_i)
{
	int			j;

	while (tab[*i] && tab[*i][0] == '-')
	{
		j = 1;
		if (tab[*i][j] == '-')
			break ;
		while (tab[*i][j] == 'i')
		{
			*flag_i = 1;
			j++;
		}
		if (tab[*i][j])
		{
			ft_dprintf(2, "env: Illegal option -- %c\n", tab[*i][j]);
			ft_dprintf(2, "usage: env [-i] [name=value ...]");
			ft_dprintf(2, " [utility [argument ...]]\n");
			return (FALSE);
		}
		(*i)++;
	}
	return (TRUE);
}

static int	ft_is_builtin(char *s, t_list_ex **lst, t_list_ex **new, int flag_i)
{
	if (!ft_strcmp(s, "exit")
		|| !ft_strcmp(s, "cd")
		|| !ft_strcmp(s, "env")
		|| !ft_strcmp(s, "setenv")
		|| !ft_strcmp(s, "unsetenv"))
	{
		ft_dprintf(2, "env: %s: is a built-in\n", s);
		return (TRUE);
	}
	if (flag_i && !ft_lst_returnvalue(*new, "PATH")
		&& ft_lst_returnvalue(*lst, "PATH"))
		ft_lst_changevalue(new, "PATH", ft_lst_returnvalue(*lst, "PATH"));
	return (FALSE);
}

void		ft_process_env(char **tab, t_list_ex **lst)
{
	int			i;
	int			flag_i;
	t_list_ex	*new_env;
	char		*tmp;
	char		*tmp2;

	i = 0;
	flag_i = 0;
	new_env = NULL;
	if (!ft_env_options(tab, &i, &flag_i))
		return ;
	while (tab[i] && (tmp = ft_strchr(tab[i], '=')))
	{
		tmp2 = ft_strsub(tab[i], 0, tmp - tab[i]);
		ft_lst_changevalue(flag_i ? &new_env : lst, tmp2, tmp + 1);
		ft_strdel(&tmp2);
		i++;
	}
	if (!tab[i])
		ft_lst_printenv(flag_i ? new_env : *lst);
	else if (!ft_is_builtin(tab[i], lst, &new_env, flag_i))
		ft_process_out((flag_i ? new_env : *lst), "env", tab + i);
	ft_freelistex(new_env);
}
