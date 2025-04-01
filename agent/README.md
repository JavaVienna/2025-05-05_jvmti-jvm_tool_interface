# JVMTI Agents

Documentation: https://docs.oracle.com/en/java/javase/21/docs/specs/jvmti.html

JVMTI agents can be attached to Java applications at startup.
They interact with the JVM and can gather information as well as change values of the program.

## Sources & Acknowledgement

2. NPE has been ported from https://github.com/odnoklassniki/jvmti-tools/blob/master/richNPE/richNPE.cpp
3. jvmquake is from https://github.com/Netflix-Skunkworks/jvmquake
4. stacktraces is from https://github.com/sachin-handiekar/jvmti-examples/blob/master/GetAllStackTraces/Native/library.cpp