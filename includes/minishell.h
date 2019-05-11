/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 09:38:33 by ffoissey          #+#    #+#             */
/*   Updated: 2019/02/27 17:10:50 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <libft.h>
# include <dirent.h>
# include <pwd.h>
# include <signal.h>
# include <sys/wait.h>

# define FALSE 0
# define TRUE 1

typedef struct			s_list_ex
{
	struct s_list_ex	*next;
	char				*value;
	char				*name;
}						t_list_ex;

typedef struct			s_glob
{
	pid_t				pid;
	int					ret;
	char				*cur_dir;
	char				*user;
}						t_glob;

extern t_glob			g_glob;

void					ft_echo(char **tab);
void					ft_setenv(t_list_ex **lst, char **tab);
void					ft_unsetenv(t_list_ex **lst, char **tab);
t_list_ex				*ft_dup_env(char **env);
char					*ft_curdir(void);
void					ft_cd(t_list_ex *lst, char *s);
int						ft_process_cd(t_list_ex *lst, char *s);
char					**ft_lst_to_tab(t_list_ex *lst);
void					ft_process_env(char **tab, t_list_ex **lst);
void					ft_lst_printenv(t_list_ex *lst);
t_list_ex				*ft_lstexnew(char *name, char *value);
void					ft_lstexadd(t_list_ex **alst, t_list_ex *new);
int						ft_lstexsize(t_list_ex *lst);
void					ft_freelistex(t_list_ex *lst);
void					ft_lstexpop(t_list_ex **lst, char *name);
int						ft_lst_changevalue(t_list_ex **lst, char *name,
						char *value);
char					*ft_lst_returnvalue(t_list_ex *lst, char *name);
t_list_ex				*ft_duplist(t_list_ex *lst);
int						ft_change_dollar_and_tild(char **tab, t_list_ex *lst);
void					ft_process_out(t_list_ex *lst,
						char *process, char **tab);
int						ft_tilde(char **tab, t_list_ex *lst, char *value,
						int i);
void					ft_print_prompt(void);
void					ft_free_global(void);
void					ft_ctrlc(int c);
void					ft_shlvl(t_list_ex **lst);
void					ft_free_global(void);
void					ft_set_global(t_list_ex *lst);

#endif
