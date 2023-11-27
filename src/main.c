#include "prompt.c"

int main() {
	char * ligne = prompt();
	while(strcmp(ligne, "exit") != 0) {
		add_history(ligne);
		free(ligne);
		ligne = prompt();
	}
	return 0;
}
