#include <stdlib.h>

extern char **environ;

static char *defaultEnv[] = {"PATH=/bin:/usr/bin","IFS= \t\n",0};

static void cleanEnv() {
	int i;
	char *b, *p;

	i = -1;
	while (environ[++i] != 0);

	while (i--) environ[i] = 0;
	
	while (defaultEnv[i] ) putenv(defaultEnv[i++]);

}

int main (int argc, char **argv) {

	cleanEnv();
}
