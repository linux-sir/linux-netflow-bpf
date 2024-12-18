#include <vmlinux.h>
#include <bpf/bpf_helpers.h>

#define TC_ACT_OK		0
// 一个简单的 tc 程序，用于记录流量
SEC("tc")
int tc_filter_prog(struct __sk_buff *skb) {
    bpf_printk("Packet received on interface %d\n", skb->ifindex);
    return TC_ACT_OK; // 允许数据包通过
}

char LICENSE[] SEC("license") = "GPL";
