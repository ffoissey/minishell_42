/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_extend.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 11:22:59 by ffoissey          #+#    #+#             */
/*   Updated: 2019/02/26 11:24:00 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*ft_strcnextdup(char *s, char c)
{
	char	*new;
	char	*tmp;

	new = ft_strdup(s);
	if (!(tmp = ft_strchr(s, c)))
		return (new);
	*(tmp + 1) = '\0';
	return (new);
}

char		**ft_lst_to_tab(t_list_ex *lst)
{
	char	**tab;
	int		count;
	int		i;

	count = ft_lstexsize(lst);
	if (!(tab = (char **)malloc(sizeof(char *) * (count + 1))))
		return (NULL);
	tab[count] = NULL;
	i = 0;
	while (lst)
	{
		tab[i] = ft_strnew(ft_strlen(lst->name) + ft_strlen(lst->value) + 2);
		ft_strcat(tab[i], lst->name);
		ft_strcat(tab[i], "=");
		ft_strcat(tab[i], lst->value);
		lst = lst->next;
		i++;
	}
	return (tab);
}

t_list_ex	*ft_dup_env(char **env)
{
	t_list_ex	*lst;
	char		*tmp;
	char		*tmp2;

	lst = NULL;
	while (*env)
	{
		ft_lstexadd(&lst, ft_lstexnew((tmp = ft_strcdup(*env, '=')),
					(tmp2 = ft_strchr(*env, '=')) ? tmp2 + 1 : NULL));
		ft_strdel(&tmp);
		env++;
	}
	return (lst);
}
