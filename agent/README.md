# JVMTI Agents

JVMTI Documentation: https://docs.oracle.com/en/java/javase/21/docs/specs/jvmti.html

TL;DR:
JVMTI agents can be attached to Java applications at startup.
They interact with the JVM and can gather information about the running program as well as change values and the
execution flow of the program.

Because this project also accesses some functions with JNI, here is the documentation of
JNI: https://docs.oracle.com/en/java/javase/21/docs/specs/jni/functions.html

## Sources & Acknowledgement

* 1-npe has been ported from https://github.com/odnoklassniki/jvmti-tools/blob/master/richNPE/richNPE.cpp
* 3-stacktraces is
  from https://github.com/sachin-handiekar/jvmti-examples/blob/master/GetAllStackTraces/Native/library.cpp
  and https://github.com/cretz/stackparam/blob/master/src/lib.rs
* 4-jvmquake is from https://github.com/Netflix-Skunkworks/jvmquake