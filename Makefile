# 项目名称
TARGET := bootstrap

# 路径配置
SRC_DIR := src
BPF_DIR := $(SRC_DIR)/bpf
BPFOBJ := $(BPF_DIR)/$(TARGET).bpf.o
BPFSKEL := $(BPF_DIR)/$(TARGET).skel.h
USER_SRC := $(SRC_DIR)/main.c

# 工具配置
CLANG := clang
CC := gcc
VMLINUX := vmlinux.h

# 编译参数
BPF_CFLAGS := -g -O2 -target bpf -D__TARGET_ARCH_x86 -I/usr/include/bpf -I.
USER_CFLAGS := -g -O2 -Wall -I/usr/include/bpf
USER_LDFLAGS := -lelf -lz -lbpf

# 默认目标
all: $(TARGET)

# 编译 eBPF 程序
$(BPFOBJ): $(BPF_DIR)/$(TARGET).bpf.c $(VMLINUX)
	$(CLANG) $(BPF_CFLAGS) -c $< -o $@

# 生成 Skeleton 文件
$(BPFSKEL): $(BPFOBJ)
	bpftool gen skeleton $(BPFOBJ) > $@

# 编译用户态程序
$(TARGET): $(USER_SRC) $(BPFSKEL)
	$(CC) $(USER_CFLAGS) -I$(BPF_DIR) $(USER_SRC) -o $@ $(USER_LDFLAGS)

# 清理目标文件
clean:
	rm -f $(BPF_DIR)/*.o $(BPFSKEL) $(TARGET)

.PHONY: all clean
