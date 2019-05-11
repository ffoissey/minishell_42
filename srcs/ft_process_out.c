/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_out.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 16:53:46 by ffoissey          #+#    #+#             */
/*   Updated: 2019/02/27 19:40:35 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_check_dir(DIR *dir, char *command, char **bin, int i)
{
	struct dirent	*mydir;
	char			*path_ex;

	while ((mydir = readdir(dir)))
	{
		if (mydir->d_name[0] != '.'
				&& !ft_strcmp(command, mydir->d_name))
		{
			if (bin[i][ft_strlen(bin[i]) - 1] == '/')
				path_ex = ft_strjoin(bin[i], command);
			else
			{
				path_ex = ft_strjoin(bin[i], "/");
				path_ex = ft_strjoinfree(path_ex, command, FIRST);
			}
			closedir(dir);
			return (path_ex);
		}
	}
	return (NULL);
}

static char	*ft_bin(t_list_ex *lst, char *command)
{
	char			**bin;
	int				i;
	DIR				*dir;
	char			*ret;

	if (!(bin = ft_strsplit(ft_lst_returnvalue(lst, "PATH"), ':')))
		return (NULL);
	i = 0;
	while (bin[i])
	{
		if ((dir = opendir(bin[i])))
		{
			if ((ret = ft_check_dir(dir, command, bin, i)))
			{
				ft_free_tab_str(bin);
				return (ret);
			}
			closedir(dir);
		}
		i++;
	}
	ft_free_tab_str(bin);
	return (NULL);
}

static void	ft_signal(int status)
{
	g_glob.pid = 0;
	g_glob.ret = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGKILL)
			ft_putendl_fd("Killed", 2);
		else if (WTERMSIG(status) == SIGSEGV)
			ft_putendl_fd("Segmentation Fault", 2);
		else if (WTERMSIG(status) == SIGFPE)
			ft_putendl_fd("Floating Point Exception", 2);
		else if (WTERMSIG(status) == SIGABRT)
			ft_putendl_fd("ABORT", 2);
		else if (WTERMSIG(status) == SIGINT)
			ft_putendl_fd("Stopped", 2);
		else if (WTERMSIG(status) == SIGILL)
			ft_putendl_fd("Illegal Instruction", 2);
		else if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Exit proccess", 2);
	}
}

static void	ft_rights(char *process, char *exec)
{
	if (access(exec, F_OK) != -1 && access(exec, X_OK) == -1)
		ft_dprintf(2, "%s: %s: Permission denied\n", process, exec);
	else
		ft_dprintf(2, "%s: %s: Command not found\n", process, exec);
}

void		ft_process_out(t_list_ex *lst, char *process, char **tab)
{
	char		**env;
	char		*exec;
	int			status;

	env = ft_lst_to_tab(lst);
	exec = NULL;
	if (!(exec = ft_bin(lst, tab[0])))
		exec = ft_strdup(tab[0]);
	if ((g_glob.pid = fork()) == -1)
		ft_dprintf(2, "%s: %s: Command failed\n", process, tab[0]);
	else if (g_glob.pid == 0)
	{
		ft_strdel(&tab[0]);
		tab[0] = ft_strdup(exec);
		if ((g_glob.ret = execve(exec, tab, env)) == -1)
			ft_rights(process, exec);
		exit(g_glob.ret);
	}
	else
	{
		waitpid(g_glob.pid, &status, 0);
		ft_signal(status);
	}
	ft_free_tab_str(env);
	ft_strdel(&exec);
}
