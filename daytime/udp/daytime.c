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

    int sfd, bytes, ret;
    char host[128], service[7], buf[16];
    time_t t;
    struct tm *tm;
    char buffer[1000];

    host[127] = 0;
    service[6] = 0;
    if ((sfd = create_inet_server_socket("127.0.0.1", "9999", LIBSOCKET_UDP, LIBSOCKET_BOTH, 0)) == -1) {
        fprintf(stderr, "create_inet_server_socket(): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while (1) {
        memset(buf, 0, 16);

        if ((ret = bytes = recvfrom_inet_dgram_socket(sfd, buf, 15, host, 127, service, 6, 0, LIBSOCKET_NUMERIC)) < 0) {
            fprintf(stderr, "recvfrom_inet_dgram_socket(): %s\n", strerror(errno));
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

        if ((ret = sendto_inet_dgram_socket(sfd, buffer, strlen(buffer), host, service, 0)) < 0) {
            fprintf(stderr, "sendto_inet_dgram_socket(): %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    if ((ret = destroy_inet_socket(sfd)) == -1) {
        fprintf(stderr, "destroy_inet_socket(): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
