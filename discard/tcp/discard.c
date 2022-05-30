#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <locale.h>
#include <unistd.h>

#include <libsocket/libinetsocket.h>

int main(void)
{
    if (setlocale(LC_ALL, "") == NULL) {
        fprintf(stderr, "locale(): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    int sfd, cfd, bytes, ret;
    char host[128], service[7], buf[16];

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

    while (1) {
        memset(buf, 0, 16);

        if ((ret = bytes = read(cfd, buf, 16)) == -1) {
            fprintf(stderr, "read(): %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    if ((ret = destroy_inet_socket(sfd)) == -1) {
        fprintf(stderr, "destroy_inet_socket(): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

