/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kschmidt <kevin@imkx.dev>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 23:08:07 by kschmidt          #+#    #+#             */
/*   Updated: 2023/02/12 23:41:19 by kschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "types.h"
# include "builtins.h"

const static t_builtin	g_builtins[] = {
{"exit", exit_builtin},
{"echo", echo_builtin},
{0}
};

int	execute(t_shell *shell, char *line);

#endif //EXECUTION_H
