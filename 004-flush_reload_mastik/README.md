In this exercise, we shall be using the Mastik framework to perform a simple Flush+Reload test.


## Build Mastik

Executing `bash flush_reload.sh` is sufficient to run the experiment.

The script `flush_reload.sh` automatically builds Mastik through the following steps:

1. ./configure

2. make

3. sudo make install


## Testing a Flush+Reload proof-of-concept

The script `flush_reload.sh` also runs a simple flush-reload demo

```c
  void *ptr = map_offset("FR-1-file-access.c", 0);
  fr_monitor(fr, ptr);

  uint16_t res[1];
  fr_probe(fr, res);

  for (;;) {
    fr_probe(fr, res);
    if (res[0] < 100)
      printf("FR-1-file-access.c  accessed\n");
    delayloop(10000);
  }
```

This code first maps the offset of the some data (i.e. the file `FR-1-file-access.c`), and stores the
same into a `void* ptr`. Then we probe the pointer and monitor access latency to establish the flush+reload
channel
