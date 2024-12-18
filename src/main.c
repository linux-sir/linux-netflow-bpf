#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include "bootstrap.skel.h" // 自动生成的 Skeleton 文件头

int main() {
    struct bootstrap_bpf *skel;
    int err;

    // 加载 eBPF 程序和 Map
    skel = bootstrap_bpf__open_and_load();
    if (!skel) {
        fprintf(stderr, "Failed to open and load BPF program\n");
        return 1;
    }

    // 将 eBPF 程序附加到 tracepoint
    err = bootstrap_bpf__attach(skel);
    if (err) {
        fprintf(stderr, "Failed to attach BPF program: %d\n", err);
        bootstrap_bpf__destroy(skel);
        return 1;
    }

    printf("eBPF program loaded and attached successfully!\n");

    // 获取 Map 的句柄
    struct bpf_map *map = skel->maps.rx_packets;

    // 实时读取 Map 数据
    while (1) {
        __u32 key = 0, next_key;
        __u64 value;

        while (bpf_map__get_next_key(map, &key, &next_key, sizeof(key)) == 0) {
            err = bpf_map__lookup_elem(map, &next_key, sizeof(next_key), &value, sizeof(value), 0);
            if (err) {
                fprintf(stderr, "Failed to lookup element for key %u\n", next_key);
                continue;
            }
            printf("Interface %u: %llu packets\n", next_key, value);
            key = next_key;
        }
        sleep(1);
    }

    // 清理资源
    bootstrap_bpf__destroy(skel);
    return 0;
}
