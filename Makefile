# 项目名称
TARGET := main

# 路径配置
SRC_DIR := src
BPF_DIR := $(SRC_DIR)/bpf

# eBPF 程序及其生成的文件
BPF_PROGS := rx_packets tc_filter xdp_filter
BPFOBJS := $(patsubst %, $(BPF_DIR)/%.bpf.o, $(BPF_PROGS))
BPFSKELS := $(patsubst %, $(BPF_DIR)/%.skel.h, $(BPF_PROGS))

# 工具配置
CLANG := clang
CC := gcc
VMLINUX := vmlinux.h


KERNEL_HEADERS := kernel_headers/include

# 编译参数
BPF_CFLAGS := -g -O2 -target bpf -D__TARGET_ARCH_x86  -I/usr/include/bpf -I.
USER_CFLAGS := -g -O2 -Wall -I/usr/include/bpf
USER_LDFLAGS := -lelf -lz -lbpf

# 默认目标
all: $(TARGET)

# 编译每个 eBPF 程序
$(BPF_DIR)/%.bpf.o: $(BPF_DIR)/%.bpf.c $(VMLINUX)
	$(CLANG) $(BPF_CFLAGS) -c $< -o $@

# 生成每个 Skeleton 文件
$(BPF_DIR)/%.skel.h: $(BPF_DIR)/%.bpf.o
	bpftool gen skeleton $< > $@

# 编译用户态程序
$(TARGET): $(SRC_DIR)/main.c $(BPFSKELS)
	$(CC) $(USER_CFLAGS) $< -o $@ $(USER_LDFLAGS)

# 清理构建环境
clean:
	rm -f $(BPFOBJS) $(BPFSKELS) $(TARGET)

.PHONY: all clean
