/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 18:28:43 by ffoissey          #+#    #+#             */
/*   Updated: 2019/02/27 20:02:57 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_curdir(void)
{
	char	*tmp;
	char	*tmp2;

	tmp = NULL;
	if (!(tmp2 = getcwd(tmp, 0)))
	{
		ft_dprintf(2, "minishell: current dir not found\n");
		return (NULL);
	}
	tmp = ft_strdup(ft_strrchr(tmp2, '/'));
	ft_strdel(&tmp2);
	if (tmp && *(tmp + 1))
	{
		tmp2 = ft_strdup(tmp + 1);
		ft_strdel(&tmp);
		return (tmp2);
	}
	return (tmp);
}

int		ft_cd_access(t_list_ex *lst, char *tmp, char *s)
{
	DIR		*dir;
	int		ret;

	ret = 0;
	if (access(tmp, F_OK) == -1)
		ret = 1;
	else if (access(tmp, R_OK) == -1)
		ret = -1;
	else if (!(dir = opendir(tmp)))
		ret = -2;
	else
	{
		closedir(dir);
		ft_lst_changevalue(&lst, "PWD", tmp);
		ft_strdel(&tmp);
		return (ft_process_cd(lst, ft_strchr(s, '/')));
	}
	ft_strdel(&tmp);
	g_glob.ret = ret;
	return (ret);
}

int		ft_process_cd(t_list_ex *lst, char *s)
{
	char	*new;
	char	*tmp;
	char	*tmp2;

	new = NULL;
	tmp = ft_lst_returnvalue(lst, "PWD");
	if (!s || !s[0])
		return (0);
	else if (s[0] == '/')
		return (ft_process_cd(lst, s + 1));
	else if (!ft_strncmp(s, "..", 2))
	{
		tmp2 = ft_strrchr(tmp + 1, '/');
		new = tmp2 ? ft_strsub(tmp, 0, tmp2 - tmp) : ft_strdup("/");
		ft_lst_changevalue(&lst, "PWD", new);
		ft_strdel(&new);
		return (ft_process_cd(lst, s + 2));
	}
	else if (!ft_strncmp(s, ".", 1) && (*(s + 1) == '/' || !*(s + 1)))
		return (ft_process_cd(lst, s + 1));
	tmp = ft_strcmp(tmp, "/") ? ft_strjoin(tmp, "/") : ft_strdup(tmp);
	tmp2 = ft_strchr(s, '/');
	new = tmp2 ? ft_strsub(s, 0, tmp2 - s) : ft_strdup(s);
	tmp = ft_strjoinfree_n(&tmp, &new, BOTH);
	return (ft_cd_access(lst, tmp, s));
}

void	ft_cd(t_list_ex *lst, char *s)
{
	int				ret;
	char			*tru;
	struct passwd	*pwd;

	tru = ft_lst_returnvalue(lst, "PWD");
	if (!s)
	{
		pwd = getpwuid(geteuid());
		if (!(s = ft_lst_returnvalue(lst, "HOME")))
			s = pwd->pw_dir;
	}
	else if (!ft_strcmp(s, "."))
		return ;
	else if (!ft_strcmp(s, "-"))
		s = ft_lst_returnvalue(lst, "OLDPWD");
	if (s && s[0] == '/')
		ft_lst_changevalue(&lst, "PWD", "/");
	if ((ret = ft_process_cd(lst, s)) == 1)
		ft_dprintf(2, "cd: %s: No such file or directory\n", s);
	else if (ret == -1)
		ft_dprintf(2, "cd: %s: Permission denied\n", s);
	else if (ret == -2)
		ft_dprintf(2, "cd: %s: Not a directory\n", s);
	if (ret)
		ft_lst_changevalue(&lst, "PWD", ft_strdup(tru));
}
