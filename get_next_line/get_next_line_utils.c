/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdenisse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 13:10:52 by vdenisse          #+#    #+#             */
/*   Updated: 2023/10/14 14:38:14 by vdenisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

size_t	ft_strlen_gnl(const char *s)
{
	size_t	len;

	if (!s || s[0] == '\0')
		return (0);
	len = 0;
	while (*s != '\0')
	{
		len++;
		++s;
	}
	return (len);
}

size_t	ft_strcat(char *dst, const char *src)
{
	size_t	i;
	size_t	dst_len;
	size_t	src_len;

	dst_len = ft_strlen_gnl(dst);
	src_len = ft_strlen_gnl(src);
	if (!dst)
		return (src_len);
	i = 0;
	while (src[i])
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (dst_len + src_len);
}

/*
returns index of first occurence of (char)c in (const char *)s
return -1 if not found
*/
int	ft_strchr_gnl(const char *s, char c)
{
	int	index;

	index = 0;
	if (!s)
		return (-1);
	if (s[0] == '\0')
		return (-1);
	while (c != *s)
	{
		if (*s == '\0')
			return (-1);
		s++;
		index++;
	}
	return (index);
}

size_t	ft_strcpy(char *dst, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!src || !dst)
		return (0);
	while (src[i] && i < n)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen_gnl(src));
}

int	ft_substr_gnl(char const *s, char **substr, unsigned int start, size_t len)
{
	size_t	sublen;
	size_t	s_len;
	size_t	index;

	index = 0;
	if (!s)
		return (0);
	s_len = ft_strlen_gnl(s);
	sublen = 0;
	if (start < s_len)
		sublen = s_len - start;
	if (sublen > len)
		sublen = len;
	*substr = (char *)malloc((sublen + 1) * sizeof(char));
	if (!(*substr))
		return (0);
	while (index < sublen)
		(*substr)[index++] = '\0';
	ft_strcpy(*substr, s + start, sublen);
	return (1);
}
