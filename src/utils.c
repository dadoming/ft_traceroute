#include "../include/utils.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		((char *)b)[i] = (unsigned char)c;
		++i;
	}
	return (b);
}


// Calculate checksum for the ICMP packet
unsigned short calculate_checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2) {
        sum += *buf++;
    }
    if (len == 1) {
        sum += *(unsigned char *) buf;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

int ft_isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

long	ft_atol(const char *str)
{
	unsigned long	num;
	signed char		sign;
	size_t			i;

	sign = 1;
	num = 0;
	i = 0;
	while ((str[i] == ' ' || str[i] == '\n'
			|| str[i] == '\t' || str[i] == '\v'
			|| str[i] == '\r' || str[i] == '\f') && str[i])
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] != '\0')
	{
		if (!ft_isdigit(str[i]))
			return (num * sign);
		num = (num * 10) + str[i] - '0';
		i++;
	}
	return (num * sign);
}
