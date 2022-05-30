#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <libsocket/libinetsocket.h>

#include <uv/unix.h>

int main(void)
{
    int sfd, bytes, ret;
    char host[128], service[7], buf[16];

    host[127] = 0;
    service[6] = 0;
    if ((sfd = create_inet_server_socket("127.0.0.1", "9999", LIBSOCKET_UDP, LIBSOCKET_BOTH, 0)) == -1) {
        fprintf(stderr, "create_inet_server_socket(): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while (1) {
        memset(buf, 0, 16);

        if ((ret = bytes = recvfrom_inet_dgram_socket(sfd, buf, 15, host, 127, service, 6, 0, LIBSOCKET_NUMERIC)) < 0) {
            fprintf(stderr, "recvfrom_inet_dgram_socket: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        if ((ret = sendto_inet_dgram_socket(sfd, buf, bytes, host, service, 0)) < 0) {
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
