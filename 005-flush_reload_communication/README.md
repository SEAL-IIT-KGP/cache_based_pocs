This exercise is to demonstrate a very simple Flush+Reload covert channel.

## Build

Perform `make all` to assemble `fp.S` and link against `leak.c` to generate `leak` binary.

## Run

First execute `echo 16 | sudo tee  /proc/sys/vm/nr_hugepages 1>/dev/null`

Execute `./leak` to run.

## Explanation

The receiver first setups the reload buffer through `mmap` syscall, such as:

```c
unsigned char *reload_buf   = (unsigned char *) mmap(NULL, LEAK_SIZE*STRIDE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE | MAP_POPULATE | MAP_HUGETLB, -1, 0);
```

And then flushes the buffer through `flush(reload_buf);`.

Then the sender is invoked, which tries to leak "This is a test to verify that is leaks" through:

```asm
fp_leak:
   movdqu  [fp_regs]   , xmm0
   movdqu  [fp_regs+64], xmm1
   LEAK    rdi, rsi

fp_leak_arch:
   movdqu  xmm0, [fp_regs]
   movdqu  xmm1, [fp_regs+64]
   ret
```

Where the `LEAK` macro expands as follow:

```asm
movzx   rax, byte [%2]
and     rax, LEAK_MASK
shl     rax, STRIDE_LOG
mov     rax, qword [%1+rax]
```

Where `[%2]` is the byte to leak. We then extract the last 8-bits of `rax` by `and` with `LEAK_MASK = 0xff`.
Then, the extracted byte is left shifted `10` bits to match the alignment of flush-reload buffer size. Finally,
we `mov` the left-shifted byte in `rax` into the indirectly addressed reload buffer `[%1+rax]`. Here, `%1` is the
base address of the reload buffer, and `rax` is the offset.

## Expected output:

```
0x0000000000402058 :
	00001000: 54 (T)
0x0000000000402059 :
	00001000: 68 (h)
0x000000000040205a :
	00001000: 69 (i)
0x000000000040205b :
	00001000: 73 (s)
0x000000000040205c :
	00001000: 20 ( )
0x000000000040205d :
	00001000: 69 (i)
0x000000000040205e :
	00001000: 73 (s)
0x000000000040205f :
	00001000: 20 ( )
0x0000000000402060 :
	00001000: 61 (a)
0x0000000000402061 :
	00001000: 20 ( )
0x0000000000402062 :
	00001000: 74 (t)
0x0000000000402063 :
	00001000: 65 (e)
0x0000000000402064 :
	00001000: 73 (s)
0x0000000000402065 :
	00001000: 74 (t)
0x0000000000402066 :
	00001000: 20 ( )
0x0000000000402067 :
	00001000: 74 (t)
0x0000000000402068 :
	00001000: 6f (o)
0x0000000000402069 :
	00001000: 20 ( )
0x000000000040206a :
	00001000: 76 (v)
0x000000000040206b :
	00001000: 65 (e)
0x000000000040206c :
	00001000: 72 (r)
0x000000000040206d :
	00000999: 69 (i)
0x000000000040206e :
	00001000: 66 (f)
0x000000000040206f :
	00001000: 79 (y)
0x0000000000402070 :
	00001000: 20 ( )
0x0000000000402071 :
	00001000: 74 (t)
0x0000000000402072 :
	00001000: 68 (h)
0x0000000000402073 :
	00001000: 61 (a)
0x0000000000402074 :
	00001000: 74 (t)
0x0000000000402075 :
	00001000: 20 ( )
0x0000000000402076 :
	00001000: 69 (i)
0x0000000000402077 :
	00001000: 73 (s)
0x0000000000402078 :
	00001000: 20 ( )
0x0000000000402079 :
	00000999: 6c (l)
0x000000000040207a :
	00001000: 65 (e)
0x000000000040207b :
	00001000: 61 (a)
0x000000000040207c :
	00001000: 6b (k)
0x000000000040207d :
	00001000: 73 (s)
```
