#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *home = getenv("HOME");
	char *pwd = getcwd();

	if (home)
		printf("Votre HOME : %s\n", home);
	else
		printf("La variable HOME n'existe pas\n");
	
	if ()
		printf("Votre HOME : %s\n", home);
	else
		printf("La variable HOME n'existe pas\n");
    return 0;
}
