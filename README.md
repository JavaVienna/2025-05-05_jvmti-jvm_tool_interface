# JVMTI Talk

## modules

* 1-npe: JVMTI has enabled devs to improved debugging in the past (pre JDK13)
* 2-simple: Just a simple example to show that jvmti works on bytecode, not sourcecode
* 2-simple-kotlin: Same but with Kotlin and to show that Kotlin compiles differently
* 3-oom: JVMTI can help end the program when something goes wrong
* 4-stacktraces: JVMTI can help to improve insights of exceptions
* 5-tack-objects: Shows that with JVMTI you can count objects, but also those who still needs to be garbage collected.
  Additionally, you can also modify values of certain objects
* 6-track-threads: Shows that threads and monitors can be tracked with JVMTI
* 7-bypass-security: A showcase how you can access restricted logic of (local) java applications
* 8-quarkus-app: A quarkus application where the access & modification of a field is being watched

Continuing with modules from the folder `agent`:

* 9-method-performance: Track how long the execution of a method takes
* 10-debugging: Write your own custom debugger with JVMTI

See the README of the folder agent as well, because there are some more examples where java apps have been re-used.

## JVMTI programs

Async-Profiler: https://github.com/async-profiler/async-profiler

Replay-Agent: https://github.com/Kartikvk1996/RR4J
