#include "ft_printf.h"

int		ft_atoi(const char *str)
{
	int		i;
	int		res;
	int		neg;

	i = 0;
	res = 0;
	neg = 1;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' ||
		str[i] == '\r' || str[i] == '\f' || str[i] == '\v')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			neg = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	return (res * neg);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;
	size_t			i;

	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	i = -1;
	while (++i < n)
		(d[i] = s[i]);
	return (dst);
}

void	*ft_memset(void *b, int c, size_t len)
{
	char	*str;
	size_t	i;

	i = 0;
	str = (char *)b;
	while (i < len)
		*(str + i++) = (unsigned char)c;
	return (b);
}

char	*ft_strndup(const char *s1, size_t n)
{
	char	*dup;
	size_t	i;

	if (!(dup = (char *)malloc(n + 1)))
		return (NULL);
	i = -1;
	while (s1[++i] && i < n)
		dup[i] = s1[i];
	dup[i] = '\0';
	return (dup);
}

char	*ft_strdup(const char *s1)
{
	char	*dup;
	size_t	i;

	i = ft_strlen(s1) + 1;
	if (!(dup = (char *)malloc(sizeof(char) * i)))
		return (NULL);
	i = -1;
	while (s1[++i])
		dup[i] = s1[i];
	dup[i] = '\0';
	return (dup);
}

char	*ft_strcat(char *s1, const char *s2)
{
	int		i;
	int		j;

	i = 0;
	j = (int)ft_strlen(s1);
	while (s2[i] != '\0')
		s1[j++] = s2[i++];
	s1[j] = '\0';
	return (s1);
}


size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	if (str && *str)
	{
		while (str[i] != '\0')
			++i;
	}
	return (i);
}
