/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nferrad <nferrad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:51:49 by clouaint          #+#    #+#             */
/*   Updated: 2024/11/01 23:16:54 by nferrad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->value)
			ft_printf("%s=%s\n", tmp->name, tmp->value);
		else if (!ft_strncmp(tmp->name, "=", 1))
			ft_printf("%s\n", tmp->name);
		tmp = tmp->next;
	}
}

t_env	*add_env_var(char *env_var)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		exit(EXIT_FAILURE);
	new = init_env_var(new, env_var);
	return (new);
}

t_env	*init_env(char **env)
{
	t_env	*head;
	t_env	*tmp;
	t_env	*new;
	int		i;

	head = NULL;
	tmp = NULL;
	i = 0;
	while (env[i])
	{
		new = add_env_var(env[i]);
		if (!head)
		{
			head = new;
			tmp = head;
		}
		else
		{
			tmp->next = new;
			tmp = new;
		}
		i++;
	}
	update_shlvl(head);
	return (head);
}

void	ft_unset(t_token *token, t_env **env)
{
	t_env	*tmp;
	t_env	*prev;
	char	*name;

	tmp = *env;
	name = token->next->str;
	prev = NULL;
	while (tmp)
	{
		if (!ft_strncmp(tmp->name, name, ft_strlen(name) + 1))
		{
			if (prev)
				prev->next = tmp->next;
			else
				*env = tmp->next;
			free(tmp->name);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	ft_printf("minishell: unset: `%s': not a valid identifier\n", name);
}
