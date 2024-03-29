/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kschmidt <kevin@imkx.dev>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 00:14:52 by kschmidt          #+#    #+#             */
/*   Updated: 2023/02/13 01:02:52 by kschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "libft.h"
#include "environment.h"

char	*get_env(t_env *environment, char *key)
{
	t_env	*env;

	env = environment;
	while (env)
	{
		if (ft_strncmp(env->name, key, ft_strlen(key)) == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (0);
}

const char	*get_env_nc(t_env *environment, char *key)
{
	t_env	*env;

	env = environment;
	while (env)
	{
		if (ft_strncmp(env->name, key, ft_strlen(key)) == 0)
			return (env->value);
		env = env->next;
	}
	return (0);
}
