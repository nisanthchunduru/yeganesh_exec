#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define YG_CMD "yeganesh"
#define YG_PATH ({ \
	char *yg_path; \
	asprintf(&yg_path, "%s%s%s%s", "/home/", getenv("USER"), \
			"/.cabal/bin/", YG_CMD); \
	yg_path; })
#define BUF_SIZE 64

int main()
{
	int pipefd[2];
	if (pipe(pipefd) == -1)
		exit(EXIT_FAILURE);

	switch (fork()) {
	case -1:
		exit(EXIT_FAILURE);
	case 0:
		close(pipefd[0]);

		if (dup2(pipefd[1], 1) == -1)
			exit(EXIT_FAILURE);
		execlp(YG_PATH, YG_CMD, "-x", NULL);
		close(pipefd[1]);
		exit(EXIT_FAILURE);
	default:
		close(pipefd[1]);
		
		int status;
		wait(&status);
		if (!(WIFEXITED(status) && (WEXITSTATUS(status) == EXIT_SUCCESS)))
			exit(EXIT_FAILURE);

		char buf[BUF_SIZE] = "";
		if (read(pipefd[0], buf, BUF_SIZE) < 1)
			exit(EXIT_FAILURE);
		buf[BUF_SIZE-1] = '\0';
		close(pipefd[0]);

		execlp(buf, buf, NULL);
		exit(EXIT_FAILURE);
	}
}
