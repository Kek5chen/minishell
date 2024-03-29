/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoll <mdoll@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:26:25 by mdoll             #+#    #+#             */
/*   Updated: 2023/03/20 10:26:25 by mdoll            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline.h"
#include <unistd.h>
#include <stdio.h>
#include <libft.h>

bool	check_for_heredoc(t_cmd *cmd, int len)
{
	while (len--)
	{
		if (cmd->next_type == CT_REDIRECT_HEREDOC)
			return (true);
		cmd++;
	}
	return (false);
}

void	clear_heredoc(t_heredoc *doc)
{
	if (ft_strnstr(doc->name, ".heredoc", 19) != 0)
	{
		unlink(doc->name);
		close(doc->fd);
	}
}

int	init_heredoc(t_cmd **cmd, t_heredoc *doc, t_fd_pipeline *pl, int *len)
{
	while ((*cmd)->next_type != CT_REDIRECT_HEREDOC)
	{
		(*cmd)++;
		(*len)--;
	}
	if ((*cmd + 1)->name == NULL)
		return (printf("minishell: syntax error -> expected limiter\n"), 1);
	if (here_doc(doc, (*cmd + 1)->name, *cmd + 1) != 0)
		return (clear_heredoc(doc), 1);
	if ((*cmd)->name == NULL)
	{
		(*cmd)++;
		(*len) -= 2;
		if ((*cmd)->argc > 1)
		{
			if (rearrange_cmd(*cmd) != 0)
				return (1);
			(*len)++;
		}
		else
			(*cmd)++;
	}
	else
		(*len)--;
	return (pl->input_fd = doc->fd, 0);
}
