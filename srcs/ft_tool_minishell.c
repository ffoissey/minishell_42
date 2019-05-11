/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tool_minishell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 16:50:45 by ffoissey          #+#    #+#             */
/*   Updated: 2019/02/27 17:57:02 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_ctrlc(int c)
{
	(void)c;
	if (g_glob.pid)
	{
		kill(g_glob.pid, SIGINT);
		g_glob.ret = c;
	}
	else
	{
		ft_putchar('\n');
		ft_print_prompt();
	}
}

void	ft_set_global(t_list_ex *lst)
{
	struct passwd	*pw;

	ft_free_global();
	if (!ft_lst_returnvalue(lst, "USER"))
	{
		pw = getpwuid(geteuid());
		g_glob.user = ft_strdup(pw->pw_name);
	}
	else
		g_glob.user = ft_strdup(ft_lst_returnvalue(lst, "USER"));
	g_glob.cur_dir = ft_curdir();
}

void	ft_print_prompt(void)
{
	ft_printf("$ \033[32;1m%s\033[0m: \033[36;1m%s\033[0m > ",
				g_glob.user, g_glob.cur_dir);
}

void	ft_free_global(void)
{
	free(g_glob.cur_dir);
	free(g_glob.user);
	g_glob.cur_dir = NULL;
	g_glob.user = NULL;
}

void	ft_shlvl(t_list_ex **lst)
{
	char	*tmp;
	char	*tmp2;
	int		lvl;

	tmp = NULL;
	if (!(tmp2 = getcwd(tmp, 0)))
		ft_dprintf(2, "minishell: shell path not found\n");
	ft_lst_changevalue(lst, "SHELL", tmp2);
	ft_strdel(&tmp2);
	if ((tmp = ft_lst_returnvalue(*lst, "SHLVL")))
		lvl = ft_atoi(tmp) + 1;
	else
		lvl = 1;
	tmp = ft_itoa(lvl);
	ft_lst_changevalue(lst, "SHLVL", tmp);
	ft_strdel(&tmp);
}
