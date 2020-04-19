#include "ft_printf.h"

void			get_flags(const char **fmt, t_flags *flags)
{
	flags->space &= 0;
	flags->plus &= 0;
	flags->zero &= 0;
	flags->minus &= 0;
	flags->pound &= 0;
	while (*(*fmt))
	{
		if (*(*fmt) == ' ')
			flags->space |= 1;
		else if (*(*fmt) == '+')
			flags->plus |= 1;
		else if (*(*fmt) == '0')
			flags->zero |= 1;
		else if (*(*fmt) == '-')
			flags->minus |= 1;
		else if (*(*fmt) == '#')
			flags->pound |= 1;
		else
			break ;
		++(*fmt);
	}
}

int				get_value(const char **fmt, va_list ap, int init_value)
{
	int		number;

	number = init_value;
	if (*(*fmt) == '*')
	{
		number = va_arg(ap, int);
		++(*fmt);
	}
	else if ((init_value ? *(*fmt) >= '0' : *(*fmt) > '0') && *(*fmt) <= '9')
	{
		number = atoi(*fmt);
		while (*(*fmt) && *(*fmt) >= '0' && *(*fmt) <= '9')
			++(*fmt);
	}
	return (number);
}

void			get_length(const char **fmt, t_flags *flags)
{
	flags->length = 0;
	if (*(*fmt) == 'L')
		flags->length = (int)'L';
	else if (*(*fmt) == 'h' && *(*fmt + 1) != 'h')
		flags->length = (int)'h';
	else if (*(*fmt) == 'h' && *(*fmt + 1) == 'h')
		flags->length = (int)('h' + 'h');
	else if (*(*fmt) == 'l' && *(*fmt + 1) != 'l')
		flags->length = (int)'l';
	else if (*(*fmt) == 'l' && *(*fmt + 1) == 'l')
		flags->length = (int)('l' + 'l');
	else if (*(*fmt) == 'z')
		flags->length = (int)'z';
	if (flags->length > 0)
		*fmt += (flags->length >= (int)('h' + 'h')) ? 2 : 1;
}

void			get_specifier(const char **fmt, t_flags *flags)
{
	char	*specs;
	int		i;

	flags->specifier = 0;
	specs = "%dicspuxX";
	i = -1;
	while (specs[++i])
	{
		if (*(*fmt) == specs[i])
		{
			flags->specifier = specs[i];
			break ;
		}
	}
	if (flags->specifier)
		++(*fmt);
	if (flags->specifier == 'p')
		flags->length = 'z';
}
