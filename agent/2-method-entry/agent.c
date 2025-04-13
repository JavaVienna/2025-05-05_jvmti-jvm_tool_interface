#include "jvmti.h"
#include <sys/un.h>

// The JVM calls this method each time a new bytecode method is called
void MethodEntry(jvmtiEnv *jvmti_env, JNIEnv* jni_env, jthread thread, jmethodID method) {
    printf("methodId entry of %p\n", method);
}

// is called when java agent is loaded
jint Agent_OnLoad(JavaVM *vm, char *options, void *reserved) {
    jvmtiEventCallbacks callbacks;
    jvmtiEnv *environment; // this is the API to interact with the JVM

    // get JVMTI environment
    jint result = (*vm)->GetEnv(vm, (void **) &environment, JVMTI_VERSION_21);
    if (result != JNI_OK) {
        return JNI_ERR;
    }

    // Clear and then enable capabilities
    jvmtiCapabilities capabilities;
    (void) memset(&capabilities, 0, sizeof(capabilities));
    capabilities.can_generate_method_entry_events = 1;
    jvmtiError error = (*environment)->AddCapabilities(environment, &capabilities);
    if (error != JNI_OK) {
        return JNI_ERR;
    }

    // Link callbacks
    (void) memset(&callbacks, 0, sizeof(callbacks));
    callbacks.MethodEntry = (void *) &MethodEntry;

    error = (*environment)->SetEventCallbacks(environment, &callbacks, (jint) sizeof(callbacks));
    if (error != JNI_OK) {
        return JNI_ERR;
    }

    // Enable event sending on the JVM
    error = (*environment)->SetEventNotificationMode(environment, JVMTI_ENABLE,
                                                     JVMTI_EVENT_METHOD_ENTRY,
                                                     (jthread) NULL);
    if (error != JNI_OK) {
        return JNI_ERR;
    }

    // Tell JVM that agent loaded successfully
    return JNI_OK;
}

// is called when agent is unloaded
void Agent_OnUnload(JavaVM *vm) {
    printf("Agent Unloaded\n");
}
