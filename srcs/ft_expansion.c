/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expansion.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 11:14:03 by ffoissey          #+#    #+#             */
/*   Updated: 2019/02/27 17:44:45 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_dollar(char **tab, t_list_ex *lst, char *value, int i)
{
	char	*tmp;
	char	*out;

	tmp = tab[i];
	out = ft_strcdup(tmp, '$');
	tmp = ft_strchr(tmp, '$');
	while (tmp && *tmp)
	{
		if (*(tmp + 1) == '$' || *(tmp + 1) == '?')
		{
			value = ft_itoa(*(tmp + 1) == '$' ? getpid() : g_glob.ret);
			out = ft_strjoinfree_n(&out, &value, BOTH);
			tmp += 2;
		}
		else
		{
			value = ft_strcdup(tmp + 1, '$');
			out = ft_strjoinfree(out, ft_lst_returnvalue(lst, value), FIRST);
			ft_strdel(&value);
			tmp = ft_strchr(tmp + 1, '$');
		}
	}
	ft_strdel(&tab[i]);
	tab[i] = out;
}

static int	ft_error_tilde(char *tmp, char *value)
{
	ft_dprintf(2, "minishell: No such user or named directory: %s\n", tmp + 1);
	ft_strdel(&value);
	return (FALSE);
}

int			ft_tilde(char **tab, t_list_ex *lst, char *value, int i)
{
	char			*tmp;
	struct passwd	*pwd;

	pwd = getpwuid(geteuid());
	value = ft_lst_returnvalue(lst, "HOME");
	value = ft_strdup(value
			? value : pwd->pw_dir);
	if (!tab[i][1] || tab[i][1] == '/')
	{
		tmp = tab[i];
		value = ft_strjoinfree(value, tab[i] + 1, FIRST);
		ft_strdel(&tmp);
		tab[i] = value;
		return (TRUE);
	}
	tmp = ft_strcdup(tab[i] + 1, '/');
	if (!(pwd = getpwnam(tmp)))
		return (ft_error_tilde(tab[i], value));
	ft_strdel(&tmp);
	tmp = ft_strchr(tab[i], '/');
	tab[i] = ft_strdup(pwd->pw_dir);
	tab[i] = ft_strjoinfree(tab[i], tmp, FIRST);
	return (TRUE);
}

int			ft_change_dollar_and_tild(char **tab, t_list_ex *lst)
{
	int		i;
	char	*value;
	char	*tmp;

	i = 0;
	value = NULL;
	while (tab && tab[i])
	{
		if ((tmp = ft_strchr(tab[i], '$')))
		{
			if (*(tmp + 1))
				ft_dollar(tab, lst, value, i);
		}
		else if (tab[i][0] == '~')
		{
			if (!(ft_tilde(tab, lst, value, i)))
				return (FALSE);
		}
		ft_strdel(&value);
		i++;
	}
	return (TRUE);
}
