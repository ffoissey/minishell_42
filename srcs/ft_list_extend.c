/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_extend.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 11:28:27 by ffoissey          #+#    #+#             */
/*   Updated: 2019/02/27 16:32:27 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			ft_lst_changevalue(t_list_ex **lst, char *name, char *value)
{
	t_list_ex	*tmp;

	tmp = *lst;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, name))
		{
			ft_strdel(&tmp->value);
			tmp->value = ft_strdup(value);
			return (TRUE);
		}
		tmp = tmp->next;
	}
	ft_lstexadd(lst, ft_lstexnew(name, value));
	return (FALSE);
}

void		ft_lst_printenv(t_list_ex *lst)
{
	while (lst)
	{
		ft_putstr(lst->name);
		ft_putchar('=');
		ft_putendl(lst->value);
		lst = lst->next;
	}
}

char		*ft_lst_returnvalue(t_list_ex *lst, char *name)
{
	while (lst)
	{
		if (!ft_strcmp(lst->name, name))
			return (lst->value);
		lst = lst->next;
	}
	return (NULL);
}

void		ft_freelistex(t_list_ex *lst)
{
	if (lst)
	{
		ft_freelistex(lst->next);
		ft_strdel(&lst->name);
		ft_strdel(&lst->value);
		free(lst);
		lst = NULL;
	}
}

t_list_ex	*ft_duplist(t_list_ex *lst)
{
	t_list_ex	*new;

	new = NULL;
	while (lst)
	{
		ft_lstexadd(&new, ft_lstexnew(lst->name, lst->value));
		lst = lst->next;
	}
	return (new);
}
