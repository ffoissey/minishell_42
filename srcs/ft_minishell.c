/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 18:27:59 by ffoissey          #+#    #+#             */
/*   Updated: 2019/02/27 19:54:31 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_glob			g_glob;

void		ft_prepare_cd(char **tab, t_list_ex **lst)
{
	char			*oldpwd;

	oldpwd = NULL;
	if (tab[1] && tab[2])
	{
		ft_dprintf(2, "cd: %s: string not in pwd\n", tab[1]);
		return ;
	}
	if (ft_lst_returnvalue(*lst, "PWD"))
		oldpwd = ft_strdup(ft_lst_returnvalue(*lst, "PWD"));
	else
	{
		if (!(oldpwd = getcwd(oldpwd, 0)))
		{
			ft_dprintf(2, "minishell: current dir not found\n");
			return ;
		}
		ft_lst_changevalue(lst, "PWD", oldpwd);
	}
	if (!ft_lst_returnvalue(*lst, "OLDPWD"))
		ft_lst_changevalue(lst, "OLDPWD", oldpwd);
	ft_cd(*lst, tab[1]);
	chdir(ft_lst_returnvalue(*lst, "PWD"));
	ft_lst_changevalue(lst, "OLDPWD", oldpwd);
	ft_strdel(&oldpwd);
}

static void	ft_exit(char **tab, t_list_ex **lst, char **sc)
{
	if (tab[1] && tab[2])
		ft_dprintf(2, "exit: Too much arguments\n");
	else if (tab[1])
	{
		if (!ft_str_is_numeric(tab[1]))
			ft_dprintf(2, "exit: Numeric argument required\n");
		else
		{
			ft_free_global();
			ft_freelistex(*lst);
			g_glob.ret = ft_atoi(tab[1]);
			ft_free_tab_str(tab);
			ft_free_tab_str(sc);
			exit(g_glob.ret);
		}
	}
	else
	{
		ft_free_global();
		ft_freelistex(*lst);
		ft_free_tab_str(tab);
		ft_free_tab_str(sc);
		exit(0);
	}
}

static void	ft_command(char **tab, t_list_ex **lst, char *process, char **sc)
{
	t_list_ex	*env;

	if (!ft_strcmp(tab[0], "exit"))
		ft_exit(tab, lst, sc);
	else if (!ft_strcmp(tab[0], "env"))
	{
		env = ft_duplist(*lst);
		ft_process_env((tab + 1), &env);
		ft_freelistex(env);
	}
	else if (!ft_strcmp(tab[0], "setenv"))
		ft_setenv(lst, tab + 1);
	else if (!ft_strcmp(tab[0], "unsetenv"))
		ft_unsetenv(lst, tab + 1);
	else if (!ft_strcmp(tab[0], "cd"))
		ft_prepare_cd(tab, lst);
	else if (!ft_strcmp(tab[0], "echo"))
		ft_echo(tab + 1);
	else
		ft_process_out(*lst, process, tab);
}

static void	ft_split_line(char *s, t_list_ex **lst)
{
	char	**tab;
	char	**sc;
	int		i;

	i = 0;
	if (!(sc = ft_strsplit(s, ';')))
		return ;
	while (sc[i])
	{
		if (!(tab = ft_strsplit_whitespaces(sc[i])) || !tab[0])
		{
			if (tab)
				ft_free_tab_str(tab);
			break ;
		}
		if (ft_change_dollar_and_tild(tab, *lst))
			ft_command(tab, lst, "minishell", sc);
		ft_free_tab_str(tab);
		i++;
	}
	ft_free_tab_str(sc);
}

int			main(int ac, char **av, char **envp)
{
	char		*line;
	t_list_ex	*env;

	(void)ac;
	(void)av;
	if (signal(SIGINT, ft_ctrlc) == SIG_ERR)
		return (1);
	env = ft_dup_env(envp);
	ft_shlvl(&env);
	ft_set_global(env);
	ft_print_prompt();
	line = NULL;
	while (get_next_line(0, &line) > 0)
	{
		ft_split_line(line, &env);
		ft_strdel(&line);
		ft_set_global(env);
		ft_print_prompt();
	}
	ft_free_global();
	ft_freelistex(env);
	return (0);
}
