/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoll <mdoll@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 10:35:14 by mdoll             #+#    #+#             */
/*   Updated: 2023/03/15 11:38:50 by mdoll            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include <unistd.h>
#include "execution.h"
#include "pipeline.h"
#include <sys/wait.h>
#include <stdio.h>

static void	restore_std_fds(int orig_stdin, int orig_stdout)
{
	dup2(orig_stdin, STDIN_FILENO);
	dup2(orig_stdout, STDOUT_FILENO);
	close(orig_stdin);
	close(orig_stdout);
}

static int	handle_final_case(t_shell *shell, t_cmd *cmd,
								int *status, t_fd_pipeline *pl)
{
	if (cmd->next_type == CT_REDIRECT_OUT || \
			cmd->next_type == CT_REDIRECT_OUTAPP)
	{
		pl->output_fd = redirect_output(cmd);
		if (pl->output_fd == -1)
			return (pl->output_fd);
		dup2(pl->output_fd, STDOUT_FILENO);
		close(pl->output_fd);
	}
	if (cmd->name)
	{
		dup2(pl->input_fd, STDIN_FILENO);
		if (pl->input_fd != STDIN_FILENO)
			close(pl->input_fd);
		close(pl->output_fd);
		execute_internal(shell, cmd, status);
	}
	while (waitpid(-1, NULL, 0) != -1)
		;
	return (*status);
}

static int	exec_pipeline_command(t_shell *shell, t_cmd **cmd, int *status,
					t_fd_pipeline *pl)
{
	if ((*cmd)->next_type == CT_PIPE || ((*cmd)->next_type == \
				CT_REDIRECT_HEREDOC && (*cmd + 1)->next_type == CT_PIPE))
	{
		pl->input_fd = handle_pipe_case(shell, *cmd, status, pl);
		if (pl->input_fd == 1)
			return (*status);
	}
	else
	{
		*status = handle_final_case(shell, *cmd, status, pl);
		return (*status);
	}
	if ((*cmd)->next_type == CT_REDIRECT_HEREDOC)
	{
		(*cmd)++;
		(*cmd)++;
	}
	else
		(*cmd)++;
	return (-1);
}

static int	check_input_redirection(t_cmd	**cmd, t_heredoc	*doc, \
							t_fd_pipeline	*pl, int *len)
{
	if (check_for_heredoc(*cmd, *len) == true)
	{
		if (init_heredoc(cmd, doc, pl, len) != 0)
			return (1);
	}
	else if ((*cmd)->next_type == CT_REDIRECT_IN)
	{
		if (*len == 1)
			return (printf("minishell: syntax error -> expected infile\n"), 1);
		pl->input_fd = redirect_input(cmd);
		if (pl->input_fd == -1)
			return (1);
		(*len)--;
	}
	else
		pl->input_fd = STDIN_FILENO;
	return (0);
}

int	exec_pipeline(t_shell *shell, t_cmd *cmd, int len, int *status)
{
	int				orig_stdin;
	int				orig_stdout;
	t_fd_pipeline	pl;
	t_heredoc		doc;

	orig_stdin = dup(STDIN_FILENO);
	orig_stdout = dup(STDOUT_FILENO);
	if (check_input_redirection(&cmd, &doc, &pl, &len) != 0)
		return (1);
	if (len == 1 && (cmd->next_type == CT_REDIRECT_OUT || \
			cmd->next_type == CT_REDIRECT_OUTAPP))
		return (printf("minishell: syntax error -> expected outfile\n"), 1);
	pl.output_fd = orig_stdout;
	while (len--)
	{
		if (cmd->next_type == CT_PIPE && cmd->name == NULL)
			return (printf("minishell: syntax error -> pipe\n"), 1);
		if (exec_pipeline_command(shell, &cmd, status,
				&pl) != -1)
			break ;
	}
	restore_std_fds(orig_stdin, orig_stdout);
	clear_heredoc(&doc);
	return (*status);
}
