# JVMTI Talk

## modules

1-npe: JVMTI has enabled devs to improved debugging in the past (pre JDK13)

2-simple: Just a simple example to show that jvmti works on bytecode, not sourcecode

3-oom: JVMTI can help end the program when something goes wrong

4-stacktraces: JVMTI can help to improve insights of exceptions

5-tack-objects: Shows that with JVMTI you can count objects, but also those who still needs to be garbage collected

6-bypass-security: A showcase how you can access restricted logic of java applications

## open ideas
Wasn't able to implement faketime and heap walker of https://github.com/odnoklassniki/jvmti-tools

watch fields
understand raw monitor
kotlin
show that obfuscation helps to hide logic
bytecode instrumentation

https://github.com/AndroidAdvanceWithGeektime/JVMTI_Sample | quite complex

https://github.com/HeapStats/heapstats | quite complex

https://github.com/headius/hprof2 | probably better to simply check out

https://github.com/kylixs/flare-profiler/tree/master/flare-agent/src | quite complex

https://github.com/xingfengwxx/MemoryMonitor/blob/master/app/src/main/cpp/native-lib.cpp -> can be used with oom slow-death to track memory allocations and so on

https://github.com/karlbennett/count-agent

https://github.com/ali-ghanbari/timer/blob/master/src/main/c/timer.c

https://github.com/Devexperts/aprof

https://github.com/jon-bell/bytecode-examples

https://github.com/check-leak/check-leak/blob/d294635500df872cb9fb01f34b805cd19bb0d3ac/check-leak/src/main/c/agent.c#L385 

https://github.com/sunwu51/notebook/blob/d5941f316ffeb972a3c40e49181fa4191189e95e/23.12/jni/MyJVMTI.c#L34

https://github.com/toolfactory/narcissus/blob/main/src/main/c/narcissus.c

google search

bookmarks

github search for certain jvmti usages

what other tools use jvmti (and how)
