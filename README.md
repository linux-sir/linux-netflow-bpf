# 简介



## 依赖项

### 依赖包

```
sudo apt install clang llvm libelf-dev libbpf-dev gcc make
```
### 编译安装libbpf


### 编译安装bpftool 

### 确认内核已经开启BPF相关配置项



### 生成 vmlinux.h

```
bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
```


# 基于Linux的网络流量相关开发库

```
project/
├── Makefile                # 编译脚本
├── vmlinux.h               # 内核 BTF 信息生成的头文件
├── src/
│   ├── bpf/
│   │   └── bootstrap.bpf.c # eBPF 程序
│   ├── main.c              # 用户态程序
```
