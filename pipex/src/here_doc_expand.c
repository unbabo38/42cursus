/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 20:51:57 by tmura             #+#    #+#             */
/*   Updated: 2025/07/07 21:31:10 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

char	*get_env_value(char *name, char **envp)
{
	int	len;
	int	i;

	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], name, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

char	*extract_var_name(const char *line, int *i)
{
	int		start;
	char	save;
	char	*var;

	start = *i;
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
		(*i)++;
	save = line[*i];
	((char *)line)[*i] = '\0';
	var = ft_strdup(line + start);
	((char *)line)[*i] = save;
	return (var);
}

char	*append_value(char *res, char *value)
{
	char	*tmp;

	tmp = ft_strjoin(res, value);
	free(res);
	return (tmp);
}

char *next_token(const char *line, int *idx, char **envp)
{
    char tmp[2];
    char *var;
    char *val;

    if (line[*idx] == '\\' && line[*idx + 1] == '$')
    {
        tmp[0] = '$';
        tmp[1] = '\0';
        *idx += 2;
        return (ft_strdup(tmp));
    }
    if (line[*idx] == '$')
    {
        (*idx)++;
        var = extract_var_name(line, idx);
        val = get_env_value(var, envp);
        free(var);
        return ft_strdup(val ? val : "");
    }
    tmp[0] = line[*idx]; 
    tmp[1] = '\0';
    (*idx)++;
    return ft_strdup(tmp);
}

char *expand_env(char *line, char **envp)
{
    char *result = ft_strdup("");
    char *token;
    char *tmp;
    int idx = 0;

    if (!result)
        return NULL;
    while (line[idx])
    {
        token = next_token(line, &idx, envp);
        if (!token)
        {
            free(result);
            return NULL;
        }
        tmp = append_value(result, token);
        free(token);
        result = tmp;
    }
    return result;
}

char	*expand_buffer(char *buffer, int *capacity, int length)
{
	int		new_capacity;
	char	*new_buffer;
	int		i;

	new_capacity = *capacity * 2;
	new_buffer = malloc(new_capacity);
	if (!new_buffer)
	{
		free(buffer);
		return (NULL);
	}
	i = 0;
	while (i < length)
	{
		new_buffer[i] = buffer[i];
		i++;
	}
	free(buffer);
	*capacity = new_capacity;
	return (new_buffer);
}
