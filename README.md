# HeapProfile
Command line application for measuring heap allocation and free times

## What does it do?
The profiler makes a series of allocations of varying sizes, and then frees half of the allocations it made, before allocating a series of blocks of the same size as the originals. 

### Example output (Windows)
Alloc 20000 * 32K = 75ms  
Alloc 20000 * 64K = 77ms  
Alloc 20000 * 128K = 88ms  
Alloc 20000 * 256K = 112ms  
Alloc 20000 * 384K = 131ms  
Alloc 20000 * 512K = 112ms  
Freed 10000 blocks (of size 32K) = 25ms  
Freed 10000 blocks (of size 64K) = 3660ms  
Freed 10000 blocks (of size 128K) = 9766ms  
Freed 10000 blocks (of size 256K) = 17742ms  
Freed 10000 blocks (of size 384K) = 24041ms  
Freed 10000 blocks (of size 512K) = 40553ms  
Alloc 20000 * 32K = 2101ms  
Alloc 20000 * 64K = 34728ms  
Alloc 20000 * 128K = 66930ms  
Alloc 20000 * 256K = 92952ms  
Alloc 20000 * 384K = 28557ms  
Alloc 20000 * 512K = 16636ms  

### macOS Mojave (MacBook Pro 2017)
Alloc 20000 * 32K = 2ms  
Alloc 20000 * 64K = 4ms  
Alloc 20000 * 128K = 29ms  
Alloc 20000 * 256K = 28ms  
Alloc 20000 * 384K = 30ms  
Alloc 20000 * 512K = 28ms  
Freed 10000 blocks (of size 32K) = 21ms  
Freed 10000 blocks (of size 64K) = 27ms  
Freed 10000 blocks (of size 128K) = 20ms  
Freed 10000 blocks (of size 256K) = 19ms  
Freed 10000 blocks (of size 384K) = 19ms  
Freed 10000 blocks (of size 512K) = 18ms  
Alloc 20000 * 32K = 7ms  
Alloc 20000 * 64K = 35ms  
Alloc 20000 * 128K = 26ms  
Alloc 20000 * 256K = 574ms  
Alloc 20000 * 384K = 1876ms  
Alloc 20000 * 512K = 1965ms  
