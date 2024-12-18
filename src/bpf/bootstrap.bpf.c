#include <vmlinux.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_core_read.h>

// 定义一个 BPF Map，存储每个网络接口接收的包计数
struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __uint(max_entries, 256);
    __type(key, __u32);   // 网络接口索引
    __type(value, __u64); // 接收的包数量
} rx_packets SEC(".maps");

SEC("tracepoint/net/netif_receive_skb")
int count_packets(struct trace_event_raw_net_dev_template *ctx) {
    struct sk_buff *skb;
    __u32 ifindex;


    /***
     * int netif_receive_skb(struct sk_buff *skb)
     * */
    // 从 tracepoint 参数中提取 `skb` 指针
    skb = (struct sk_buff *)ctx->skbaddr;

    // 读取网络接口索引（`skb->dev->ifindex`）
    ifindex = BPF_CORE_READ(skb, dev, ifindex);

    // 更新 BPF Map
    __u64 *value = bpf_map_lookup_elem(&rx_packets, &ifindex);
    if (value) {
        __sync_fetch_and_add(value, 1); // 增加包计数
    } else {
        __u64 init_val = 1;
        bpf_map_update_elem(&rx_packets, &ifindex, &init_val, BPF_ANY);
    }

    return 0;
}

char LICENSE[] SEC("license") = "GPL";
