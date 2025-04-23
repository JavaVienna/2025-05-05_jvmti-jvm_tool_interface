# JVMTI Talk

## modules

1-npe: JVMTI has enabled devs to improved debugging in the past (pre JDK13)

2-simple: Just a simple example to show that jvmti works on bytecode, not sourcecode

3-oom: JVMTI can help end the program when something goes wrong

4-stacktraces: JVMTI can help to improve insights of exceptions

5-tack-objects: Shows that with JVMTI you can count objects, but also those who still needs to be garbage collected

6-bypass-security: A showcase how you can access restricted logic of java applications

## open ideas

show that obfuscation helps to hide logic
bytecode instrumentation? must be done manually

https://github.com/AndroidAdvanceWithGeektime/JVMTI_Sample | quite complex

https://github.com/HeapStats/heapstats | quite complex

https://github.com/headius/hprof2 | probably better to simply check out

https://github.com/kylixs/flare-profiler/tree/master/flare-agent/src | quite complex

https://github.com/xingfengwxx/MemoryMonitor/blob/master/app/src/main/cpp/native-lib.cpp -> can be used with oom slow-death to track memory allocations and so on

https://github.com/toolfactory/narcissus/blob/main/src/main/c/narcissus.c | to bypass all jdk security measures

bookmarks

github search for certain jvmti usages

what other tools use jvmti (and how)
