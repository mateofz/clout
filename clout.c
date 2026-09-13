#include <sys/types.h>
#include <stddef.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include "color.h"


ssize_t write_all(int fd, const void *buf, size_t n);
void usage(int status);
void error(const char * errort);


int 
main (int argc, char * argv[]) 
{
    int c;
    const char * stderror_color = COLOR_RED;


    while ((c = getopt(argc, argv, "+e:h")) != -1)
    {
        switch (c)
        {
            case 'e' : 
                stderror_color = color_from_name(optarg);
                if (stderror_color == NULL)
                    usage(1);
                break;
            case 'h' :
                usage(0);
                break;
            case '?':
                usage(1);
                break;
        }
    }

    if (argv[optind] == NULL)
        usage(1);



    int pipe_fd_err[2];
    if (pipe(pipe_fd_err) == -1)
        error("pipe");

    pid_t pid = fork();
    if (pid == -1)
        error("fork");


    if (pid != 0) // parent
    {
        close(pipe_fd_err[1]);

        ssize_t n;
        char buffer[255];
        while ((n = read(pipe_fd_err[0], buffer, sizeof(buffer))) > 0)
        {
            ssize_t len = strlen(stderror_color);
            if (write_all(STDERR_FILENO, stderror_color,len) != len)
                error("write");
            if (write_all(STDERR_FILENO, buffer, n) != n)
                error("write");

            len = sizeof(COLOR_RESET) - 1;
            if (write_all(STDERR_FILENO, COLOR_RESET,len) != len)
                error("write");

        }
        if (n == -1)
            error("read");

        close(pipe_fd_err[0]);

        int status;
        if (waitpid(pid, &status, 0) == -1)
            error("waitpid");

        if (WIFEXITED(status))
            return WEXITSTATUS(status);

        if (WIFSIGNALED(status))
            return 128 + WTERMSIG(status);

        return 1;

    }
    else
    {
        close(pipe_fd_err[0]);
        if (dup2(pipe_fd_err[1], STDERR_FILENO) == -1)
            error("dup2");

        close(pipe_fd_err[1]);

        if (execvp(argv[optind], &argv[optind]) == -1)
            error("execvp");

    }
}




void error(const char * errort)
{
    perror(errort);
    _exit(1);
}


ssize_t
write_all(int fd, const void *buf, size_t n)
{
    size_t written = 0;

    while (written < n) {
        ssize_t ret = write(fd,
                            (const char *)buf + written,
                            n - written);

        if (ret <= 0)
            return -1;

        written += ret;
    }

    return written;
}


void
usage(int status)
{
    fprintf(stderr,
        "usage: clout [-e color] command [argument ...]\n"
        "\n"
        "Color stderr output.\n"
        "\n"
        "Options:\n"
        "    -e color    set stderr color\n"
        "    -h          show this help\n"
        "\n"
        "Colors:\n"
        "    black red green yellow blue purple cyan white\n"
    );

    _exit(status);
}