#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bpf/rx_packets.skel.h"  // Skeleton for rx_packets
#include "bpf/tc_filter.skel.h"  // Skeleton for tc_filter

int main() {
    // 加载和附加 rx_packets 程序
    struct rx_packets_bpf *rx_skel = rx_packets_bpf__open_and_load();
    if (!rx_skel) {
        fprintf(stderr, "Failed to load rx_packets program\n");
        return 1;
    }
    if (rx_packets_bpf__attach(rx_skel)) {
        fprintf(stderr, "Failed to attach rx_packets program\n");
        rx_packets_bpf__destroy(rx_skel);
        return 1;
    }
    printf("rx_packets program loaded and attached successfully\n");

    // 加载和附加 tc_filter 程序
    struct tc_filter_bpf *tc_skel = tc_filter_bpf__open_and_load();
    if (!tc_skel) {
        fprintf(stderr, "Failed to load tc_filter program\n");
        return 1;
    }
    if (tc_filter_bpf__attach(tc_skel)) {
        fprintf(stderr, "Failed to attach tc_filter program\n");
        tc_filter_bpf__destroy(tc_skel);
        return 1;
    }
    printf("tc_filter program loaded and attached successfully\n");

    // 保持程序运行
    while (1) {
        sleep(10);
    }

    // 清理资源
    rx_packets_bpf__destroy(rx_skel);
    tc_filter_bpf__destroy(tc_skel);
    return 0;
}
