# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mac <mac@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/20 00:35:07 by zchtaibi          #+#    #+#              #
#    Updated: 2024/12/13 14:36:04 by mac              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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
		libft/ft_atoi.c\
		libft/ft_substr.c\
		libft/ft_strchr.c\
		libft/ft_itoa.c\
		libft/ft_strnstr.c\
		libft/ft_strtrim.c\
		libft/ft_strcmp.c\
		libft/ft_strndup.c\
		srcs/lexer/tokenizer.c\
		srcs/lexer/tokenizer_utils.c\
		srcs/parser/parser.c\
		srcs/parser/parser_utils.c\
		srcs/parser/parser_utils2.c\
		srcs/parser/parser_utils3.c\
		srcs/lexer/errors_checker.c\
		srcs/lexer/validate_input.c\
		srcs/parser/validate_syntax.c\
		srcs/parser/builtins_checker.c\
		srcs/executor/builtins/builtins.c\
		srcs/executor/no-builtins/executor.c\
		srcs/expander/expander.c\
		srcs/executor/builtins/builtins_utils.c\
		srcs/executor/builtins/builtins_utils1.c\
		srcs/executor/builtins/builtins_utils3.c\
		srcs/executor/builtins/builtins_utils4.c\
		libft/ft_isalnum.c\
		libft/ft_isalpha.c\
		libft/ft_strncpy.c\
		srcs/executor/builtins/export_utils.c\
		srcs/executor/builtins/cd_utils.c\
		srcs/executor/no-builtins/executor_utils.c\
		srcs/executor/no-builtins/executor_utils1.c\
		srcs/executor/no-builtins/executor_utils2.c\
		srcs/executor/no-builtins/executor_utils3.c\
		srcs/executor/no-builtins/executor_signals.c\
		srcs/expander/expander_utils.c\
		srcs/expander/expander_utils1.c\
		srcs/expander/expander_utils2.c\
		srcs/parser/redirections_checker.c\
		srcs/executor/builtins/builtins_utils2.c\
		srcs/main_utils.c\
		srcs/clean.c\
		srcs/parser/redirection_checker_utils.c\

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 #-fsanitize=address
LDFLAGS = -lreadline

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all