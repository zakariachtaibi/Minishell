NAME = minishell

SRCS = srcs/main.c \
		libft/ft_isdigit.c \
        libft/ft_putchar_fd.c \
        libft/ft_putendl_fd.c \
        libft/ft_putnbr_fd.c \
        libft/ft_putstr_fd.c \
        libft/ft_split.c \
		libft/ft_strjoin.c \
		libft/ft_strlcpy.c \
        libft/ft_strdup.c \
        libft/ft_strlen.c \
        libft/ft_strncmp.c \
		libft/ft_memcpy.c \
		libft/ft_strlcat.c \
		libft/ft_memset.c \
		srcs/lexer/tokenizer.c\
		srcs/parser/parser.c\
		libft/ft_strchr.c\


OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all