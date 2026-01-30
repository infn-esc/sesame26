---
title: Atomics and Mutex 
layout: main 
category: am 
---

# Atomics 
in hands-on/memory/atomics_mutex
Modify the file atomics.cc 

You have a producer worker (thread) filling up a vector and then a consumer worker reading the vector and summing up the entry of the vector. 
Try compiling with different optimization strategy and check the results. 
Fix it using atomics ! 


# Mutex 
in hands-on/memory/atomics_mutex
Modify the file mutex.cc 

Perform the counting of even and odd numbers in parallel with std::thread! Use mutexes to ensure correcteness (you can also try with atomics after!)
