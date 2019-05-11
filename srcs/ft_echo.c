/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 18:27:49 by ffoissey          #+#    #+#             */
/*   Updated: 2019/02/26 11:26:57 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_quote(char **tab)
{
	char	*tmp;

	tmp = ft_strctrim(*tab, '\'');
	ft_strdel(tab);
	*tab = tmp;
	tmp = ft_strctrim(*tab, '\"');
	ft_strdel(tab);
	*tab = tmp;
}

void		ft_echo(char **tab)
{
	int		i;
	int		flag_n;
	char	*s;

	flag_n = 0;
	i = 0;
	s = NULL;
	ft_quote(tab);
	if (tab[0] && !ft_strcmp(tab[0], "-n"))
	{
		flag_n = 1;
		i++;
	}
	while (tab[i])
	{
		s = ft_strjoinfree(s, tab[i++], FIRST);
		if (tab[i])
			s = ft_strjoinfree(s, " ", FIRST);
	}
	if (!flag_n)
		s = ft_strjoinfree(s, "\n", FIRST);
	ft_putstr(s);
	ft_strdel(&s);
}
