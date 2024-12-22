#include <vmlinux.h>
#include <bpf/bpf_helpers.h>


// 手动补充常用宏定义
#define ETH_P_IP 0x0800      // 用于 IP 协议的以太网协议类型
#define XDP_PASS 1           // 表示继续传递数据包
#define XDP_DROP 2           // 表示丢弃数据包


// 手动实现 htons (Host to Network Short)
static inline __u16 bpf_htons(__u16 hostshort) {
    return (hostshort << 8) | (hostshort >> 8);
}

// 手动实现 htonl (Host to Network Long)
static inline __u32 bpf_htonl(__u32 hostlong) {
    return ((hostlong << 24) & 0xFF000000) |
           ((hostlong << 8) & 0x00FF0000) |
           ((hostlong >> 8) & 0x0000FF00) |
           ((hostlong >> 24) & 0x000000FF);
}


// 定义 BPF 程序
SEC("xdp")
int xdp_filter_prog(struct __sk_buff *skb) {

     bpf_printk("Packet received on interface \n");
    /** 
    struct ethhdr eth;
    int ret;

    ret = bpf_skb_load_bytes(skb, 0, &eth, sizeof(struct ethhdr));
    if (ret < 0) {
        return XDP_DROP;
    }

    if (eth.h_proto == bpf_htons(ETH_P_IP)) {
        struct iphdr ip;
        
        ret = bpf_skb_load_bytes(skb, sizeof(struct ethhdr), &ip, sizeof(struct iphdr));
        if (ret < 0) {
            return XDP_DROP;
        }

        if (ip.protocol == IPPROTO_TCP && ip.saddr == bpf_htonl(0xC0A80001)) {
            return XDP_DROP;
        }
    }

**/
    return XDP_PASS;
}

/** 
int xdp_filter_prog(struct __sk_buff *skb) {
    struct ethhdr eth;
    int ret;

    // 使用 bpf_skb_load_bytes 读取以太网头部数据
    ret = bpf_skb_load_bytes(skb, 0, &eth, sizeof(struct ethhdr));
    if (ret < 0) {
        return XDP_DROP;  // 读取失败，丢弃数据包
    }

    // 检查以太网协议类型是否为 IP（ETH_P_IP = 0x0800）
    if (eth.h_proto == bpf_htons(ETH_P_IP)) {
        struct iphdr ip;
        
        // 使用 bpf_skb_load_bytes 读取 IP 头部数据
        ret = bpf_skb_load_bytes(skb, sizeof(struct ethhdr), &ip, sizeof(struct iphdr));
        if (ret < 0) {
            return XDP_DROP;  // 读取 IP 头部失败，丢弃数据包
        }

        // 检查是否是 TCP 包，且源 IP 是否为特定地址（192.168.0.1）
        if (ip.protocol == IPPROTO_TCP && ip.saddr == bpf_htonl(0xC0A80001)) {
            return XDP_DROP;  // 丢弃来自 192.168.0.1 的 TCP 数据包
        }
    }

    return XDP_PASS;  // 其他数据包允许通过
}
**/

char LICENSE[] SEC("license") = "GPL";