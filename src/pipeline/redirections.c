/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoll <mdoll@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 12:09:32 by mdoll             #+#    #+#             */
/*   Updated: 2023/03/15 20:15:58 by mdoll            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "pipeline.h"
#include "libft.h"

int	redirect_input(t_cmd **cmd)
{
	int	open_ret;

	(*cmd)++;
	open_ret = open((*cmd)->name, O_RDONLY);
	if (open_ret == -1)
	{
		perror((*cmd)->name);
		return (open_ret);
	}
	if ((*cmd)->argc > 1)
	{
		if (rearrange_cmd(*cmd) != 0)
			return (1);
	}
	else
		(*cmd)++;
	return (open_ret);
}

int	rearrange_cmd(t_cmd *cmd)
{
	char	**new_args;
	int		i;

	i = 1;
	free(cmd->name);
	cmd->name = ft_strdup(cmd->args[1]);
	if (!cmd->name)
		return (1);
	new_args = malloc(sizeof(char *) * cmd->argc);
	if (!new_args)
		return (1);
	while (cmd->args[i])
	{
		new_args[i - 1] = ft_strdup(cmd->args[i]);
		if (!new_args[i - 1])
			ft_free_split(new_args);
		i++;
	}
	new_args[i - 1] = NULL;
	ft_free_split(cmd->args);
	cmd->args = new_args;
	return (0);
}

int	redirect_output(t_cmd *cmd)
{
	int	open_flags;
	int	fd;

	open_flags = 0;
	while (cmd->next_type == CT_REDIRECT_OUT || \
			cmd->next_type == CT_REDIRECT_OUTAPP)
	{
		if (cmd->next_type == CT_REDIRECT_OUT)
			open_flags = O_WRONLY | O_TRUNC | O_CREAT;
		else
			open_flags = O_WRONLY | O_CREAT | O_APPEND;
		cmd++;
		fd = open(cmd->name, open_flags, 0644);
		if (fd == -1)
			return (-1);
		close(fd);
	}
	return (open(cmd->name, open_flags, 0644));
}
