/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfinnis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 11:11:11 by dfinnis           #+#    #+#             */
/*   Updated: 2019/01/16 18:16:30 by svaskeli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

static void	ft_parse_ants(t_lem_in *lem_in)
{
	int	i;

	i = 0;
	while (lem_in->line[i] == ' ' || lem_in->line[i] == '+')
		i++;
	while (lem_in->line[i])
	{
		if (!(ft_isdigit(lem_in->line[i])))
			ft_lem_in_error(lem_in, "invalid first line: number of ants");
		i++;
	}
	if (!(lem_in->ant_c = ft_atoi(lem_in->line)))
		ft_lem_in_error(lem_in, "invalid first line: number of ants");
	if (lem_in->ant_c < 1 || lem_in->ant_c > MAX_ANTS)
		ft_lem_in_error(lem_in, "number of ants out of acceptable range");
}

static void	ft_parse_comment(t_lem_in *lem_in)
{
	if (ft_strcmp(lem_in->line, "##start") == 0)
	{
		lem_in->start_flag = 1;
		lem_in->end_flag = 0;
	}
	else if (ft_strcmp(lem_in->line, "##end") == 0)
	{
		lem_in->end_flag = 1;
		lem_in->start_flag = 0;
	}
}

static void	ft_parse_room(t_lem_in *lem_in)
{
	int		n;
	t_room	*new;

	n = 0;
	if (lem_in->link)
		ft_lem_in_error(lem_in, "room after link");
	ft_add_room(lem_in, &new);
	while (lem_in->line[n])
		n++;
	ft_coordinate_isdigit(lem_in, lem_in->line[--n]);
	n = ft_find_coordinate(lem_in->line, n);
	new->y = ft_atoi_intmax(lem_in->line + n + 1);
	ft_max_coordinate(lem_in, new->y);
	ft_coordinate_isdigit(lem_in, lem_in->line[--n]);
	n = ft_find_coordinate(lem_in->line, n);
	new->x = ft_atoi_intmax(lem_in->line + n + 1);
	ft_max_coordinate(lem_in, new->x);
	if (n < 0)
		ft_lem_in_error(lem_in, "empty room name");
	if (lem_in->line[n] != ' ')
		ft_lem_in_error(lem_in, "room incorrectly formatted");
	if (!(new->name = ft_strndup(lem_in->line, n)))
		ft_lem_in_error(lem_in, "strndup fail in ft_parse_room");
	ft_room_error(lem_in, new);
	ft_start_end(lem_in, new);
}

static void	ft_parse_link(t_lem_in *lem_in)
{
	t_link	*new;
	t_link	*tmp;

	ft_parse_error(lem_in);
	tmp = lem_in->link;
	ft_add_link(lem_in, &new);
	if (!(new->from = ft_find_room(lem_in, 'f')))
		ft_lem_in_error(lem_in, "link to unknown room");
	if (!(new->to = ft_find_room(lem_in, 't')))
		ft_lem_in_error(lem_in, "link to unknown room");
	if (!LINK_SELF && (new->from == new->to))
		ft_lem_in_error(lem_in, "link to self \
			(to accept links to self set LINK_SELF to 1)");
	if (!LINK_DUP)
	{
		while (tmp && tmp->next)
		{
			if ((new->from == tmp->from && new->to == tmp->to)
				|| (new->from == tmp->to && new->to == tmp->from))
				ft_lem_in_error(lem_in, "link duplicated");
			tmp = tmp->next;
		}
	}
}

void		ft_lem_in_parse(t_lem_in *lem_in)
{
	get_next_line(0, &lem_in->line);
	ft_parse_ants(lem_in);
	if (lem_in->flag_wc)
		ft_printf("%s\n", lem_in->line);
	ft_freestr(lem_in->line);
	while (get_next_line(0, &lem_in->line) == 1)
	{
		if (lem_in->line[0] == '#')
			ft_parse_comment(lem_in);
		else if (ft_strchr(lem_in->line, '-'))
			ft_parse_link(lem_in);
		else if (ft_strchr(lem_in->line, ' '))
			ft_parse_room(lem_in);
		else
			ft_lem_in_error(lem_in, "line not comment, command, room or link");
		if (lem_in->flag_wc)
			ft_printf("%s\n", lem_in->line);
		ft_freestr(lem_in->line);
	}
	ft_parse_error(lem_in);
	if (!lem_in->link)
		ft_lem_in_error(lem_in, "no links");
	if (lem_in->flag_wc)
		ft_printf("\n");
}
