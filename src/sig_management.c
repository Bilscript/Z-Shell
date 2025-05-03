/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_management.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 12:25:19 by slebik            #+#    #+#             */
/*   Updated: 2025/05/03 13:25:04 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Configuration des signaux pour le mode normal (readline)
void	setup_signals(void)
{
   struct sigaction	sa;

   ft_memset(&sa, 0, sizeof(sa));
   sa.sa_handler = handle_sigint;
   sigaction(SIGINT, &sa, NULL);
   signal(SIGQUIT, SIG_IGN);
}

// Configuration des signaux pour le mode d'exécution de commande+
void	setup_exec_signals(void)
{
   signal(SIGINT, SIG_DFL);
   signal(SIGQUIT, SIG_DFL);
}

// Configuration des signaux pour le mode heredoc
void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = heredoc_sigint_handler;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

// Réinitialisation des signaux à leur état par défaut
void	reset_signals(void)
{
   signal(SIGINT, SIG_DFL);
   signal(SIGQUIT, SIG_DFL);
}