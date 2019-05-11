/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/05 10:34:59 by ffoissey          #+#    #+#             */
/*   Updated: 2019/02/26 11:37:11 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list_ex	*ft_lstexnew(char *name, char *value)
{
	t_list_ex	*list;

	if (!(list = (t_list_ex *)malloc(sizeof(t_list_ex))))
		return (NULL);
	list->next = NULL;
	list->name = ft_strdup(name);
	list->value = ft_strdup(value);
	return (list);
}

void		ft_free_node(t_list_ex **lst)
{
	ft_strdel(&(*lst)->name);
	ft_strdel(&(*lst)->value);
	free(*lst);
	*lst = NULL;
}

void		ft_lstexpop(t_list_ex **lst, char *name)
{
	t_list_ex	*before;
	t_list_ex	*tmp;
	int			first;

	before = *lst;
	tmp = *lst;
	first = 0;
	while (tmp)
	{
		if (tmp->name && !ft_strcmp(tmp->name, name))
		{
			if (first != 0)
				before->next = tmp->next;
			else
			{
				before = tmp->next;
				*lst = before;
			}
			ft_free_node(&tmp);
			return ;
		}
		before = tmp;
		first++;
		tmp = tmp->next;
	}
}

void		ft_lstexadd(t_list_ex **alst, t_list_ex *new)
{
	t_list_ex	*tmp;

	if (!alst || !new)
		return ;
	if (!*alst)
		*alst = new;
	else
	{
		tmp = *alst;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

int			ft_lstexsize(t_list_ex *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		lst = lst->next;
		count++;
	}
	return (count);
}
