#include <unistd.h>
#include <stdio.h>
#include <sys/poll.h>
#include <string.h>
#include <fcntl.h>
#include "color.h"
#include <sys/wait.h>
#include <errno.h>


void close_pipe (int fd[2]);
void usage(int status);
void error(const char * error);
ssize_t write_all(int fd, const void *buf, size_t n);
void handle_stream(struct pollfd *pfd,
                   const char *output_color,
                   int output_fd);





int 
main (int argc, char * argv[]) 
{
    const char * stderr_color = COLOR_RED;
    const char * stdout_color = COLOR_GREEN;


    int c;
    while ((c = getopt(argc, argv, "+e:o:h")) != -1)
    {
        switch (c)
        {
            case 'e' : 
                stderr_color = color_from_name(optarg);
                if (stderr_color == NULL)
                    usage(1);
                break;
            case 'o' : 
                stdout_color = color_from_name(optarg);
                if (stdout_color == NULL)
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

    if (optind >= argc)
        usage(1);

    int stdout_fd[2];
    int stderr_fd[2];
    if (pipe(stdout_fd) == -1)
        error("pipe");

    if (pipe(stderr_fd) == -1) {
        close_pipe(stdout_fd);
        error("pipe");
    }




    pid_t pid = fork();
    if (pid == -1)
        error("fork");

    else if (pid != 0) // parent process
    {

        close(stdout_fd[1]);
        close(stderr_fd[1]);
        close(STDIN_FILENO);
        struct pollfd pfd[2] = 
        {
            {.fd = stdout_fd[0], .events = POLLIN},
            {.fd = stderr_fd[0], .events = POLLIN}
        };
        int flags;

        flags = fcntl(stdout_fd[0], F_GETFL);
        if (flags == -1)
            error("fcntl");
        if (fcntl(stdout_fd[0], F_SETFL, flags | O_NONBLOCK) == -1)
            error("fcntl");

        flags = fcntl(stderr_fd[0], F_GETFL);
        if (flags == -1)
            error("fcntl");
        if (fcntl(stderr_fd[0], F_SETFL, flags | O_NONBLOCK) == -1)
            error("fcntl");
        while (pfd[0].fd != -1 || pfd[1].fd != -1)
        {
            if (poll(pfd, 2, -1) == -1)
                error("poll");
            

            if (pfd[0].revents & (POLLIN | POLLHUP))
                handle_stream(&pfd[0], stdout_color, STDOUT_FILENO);

            if (pfd[1].revents & (POLLIN | POLLHUP))
                handle_stream(&pfd[1], stderr_color, STDERR_FILENO);
        }
        close_pipe(stdout_fd);
        close_pipe(stderr_fd);


        int status;
        if (waitpid(pid, &status, 0) == -1)
            error("waitpid");

        if (WIFEXITED(status))
            _exit (WEXITSTATUS(status));

        if (WIFSIGNALED(status))
            _exit (128 + WTERMSIG(status));
    }



    else
    {
        close(stdout_fd[0]);
        close(stderr_fd[0]);


        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        if (dup2(stdout_fd[1], STDOUT_FILENO) == -1 || dup2(stderr_fd[1], STDERR_FILENO) == -1)
            error("dup2");

        close_pipe(stdout_fd);
        close_pipe(stderr_fd);

        execvp(argv[optind], &argv[optind]);
            
        error("execvp");
        close_pipe(stdout_fd);
        close_pipe(stderr_fd);

    }
}










void error(const char * error_ot)
{
    perror(error_ot);
    _exit(1);
}



void close_pipe (int fd[2])
{
    for (int i = 0 ; i < 2; i++)
        close(fd[i]);
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
handle_stream(struct pollfd *pfd,
              const char *output_color,
              int output_fd)
{
    char buffer[256];

    for (;;) {
        ssize_t n = read(pfd->fd, buffer, sizeof(buffer));

        if (n > 0) {
            size_t color_len = strlen(output_color);
            size_t reset_len = sizeof(COLOR_RESET) - 1;

            if (write_all(output_fd, output_color, color_len) == -1)
                error("write");

            if (write_all(output_fd, buffer, (size_t)n) == -1)
                error("write");

            if (write_all(output_fd, COLOR_RESET, reset_len) == -1)
                error("write");

            continue;
        }

        if (n == 0) {
            close(pfd->fd);
            pfd->fd = -1;
            return;
        }

        if (errno == EINTR)
            continue;

        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return;

        error("read");
    }
}



void
usage(int status)
{
    fprintf(stderr,
        "usage: clout [-h] [-e color] [-o color] command [argument ...]\n"
        "\n"
        "Colorize command output streams.\n"
        "\n"
        "Options:\n"
        "  -e color    Set stderr color (default: red).\n"
        "  -o color    Set stdout color (default: green).\n"
        "  -h          Display this help and exit.\n"
        "\n"
        "Colors:\n"
        "  black       red         green       yellow\n"
        "  blue        purple      cyan        white\n"
        "  gray        bright-red  bright-green bright-yellow\n"
        "  bright-blue bright-purple bright-cyan bright-white\n"
        "\n"
        "Examples:\n"
        "  clout -e red -o green ls -l\n"
        "  clout -e yellow grep pattern file\n"
        "  clout -o cyan printf 'hello\\n'\n"
        "\n"
        "Exit status:\n"
        "  Returns the command's exit status, or 128 + signal number\n"
        "  if the command terminates due to a signal.\n"
    );

    _exit(status);
}