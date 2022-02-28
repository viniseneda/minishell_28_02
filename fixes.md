erro do seg fault com espaço:
minishell.c linha 27, adicionar essa condição no if:
	if (token_list != NULL && token_list->operator == 2)

arquivos com modificações
	builtin_cd_pwd.c
	builtin_echo.c
	builtins.c
	dict_setup.c
	execs.c
	list.c
	minishell.c
	minishell.h
