/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kschmidt <kevin@imkx.dev>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 23:03:49 by kschmidt          #+#    #+#             */
/*   Updated: 2023/03/10 17:41:04 by kschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

# include "types.h"

void		load_env(t_shell *shell, char **envp);
char		*get_env(t_env *environment, char *key);
const char	*get_env_nc(t_env *environment, char *key);
void		set_env(t_shell *shell, char *key, const char *value);
void		set_secret_env(t_shell *shell, char *key, const char *value);
char		**export_env(t_env *env);
void		remove_env(t_shell *shell, const char *key);
void		remove_secret_env(t_shell *shell, const char *key);
void		clear_env(t_env **environment);

#endif //ENVIRONMENT_H
