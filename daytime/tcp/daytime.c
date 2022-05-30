#include <errno.h>

#include <libsocket/libinetsocket.h>
#include <locale.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include <unistd.h>

int main(void)
{
    if (setlocale(LC_ALL, "") == NULL) {
        fprintf(stderr, "setlocale(): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    int sfd, cfd, bytes, ret;
    char host[128], service[7], buf[16];
    time_t t;
    struct tm *tm;
    char buffer[1000];

    host[127] = 0;
    service[6] = 0;
    if ((sfd = create_inet_server_socket("127.0.0.1", "9999", LIBSOCKET_TCP, LIBSOCKET_BOTH, 0)) == -1) {
        fprintf(stderr, "create_inet_server_socket(): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }


    if ((cfd = accept_inet_stream_socket(sfd, 0, 0, 0, 0, 0, 0)) == -1) {
        fprintf(stderr, "accept_inet_stream_socket(): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while(1) {
        memset(buf, 0, 16);

        if ((ret = bytes = read(cfd, buf, 16)) == -1) {
            fprintf(stderr, "read(): %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        t = time(NULL);
        if ((tm = localtime(&t))== NULL) {
            fprintf(stderr, "localtime(): %s\n", strerror(errno));
            continue;
        }

        if (strftime(buffer, sizeof(buffer), "%A, %B %d, %Y %T-%Z", tm) == 0) {
            fprintf(stderr, "strftime(): %s\n", strerror(errno));
            continue;
        }

        if ((ret = write(cfd, buffer, strlen(buffer))) == -1) {
            fprintf(stderr, "write(): %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }


    if ((ret = destroy_inet_socket(sfd)) == -1) {
        fprintf(stderr, "destroy_inet_socket(): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}