#include "jvmti.h"
#include <sys/un.h>
#include <stdlib.h>

JavaVM *global_jvm;
jvmtiEnv *environment;

void MethodEntry(jvmtiEnv *jvmti_env, JNIEnv* jni_env, jthread thread, jmethodID method) {
    printf("methodId entry of %p\n", method);
}

void installCallback(const jvmtiEvent event_type) {
    const jvmtiError error = (*environment)->SetEventNotificationMode(environment,
                                                                      JVMTI_ENABLE,
                                                                      event_type,
                                                                      (jthread) NULL);
    if (error != JNI_OK) {
        printf("Error registering callback of event type %d\n", event_type);
    }
}

void installCallbacks() {
    // events to receive a callback for
    const jvmtiEvent eventsToInstall[] = {
        JVMTI_EVENT_METHOD_ENTRY
    };

    const long arraySize = *(&eventsToInstall + 1) - eventsToInstall;
    for (long i = 0; i < arraySize; i++) {
        installCallback(eventsToInstall[i]);
    }
}

// is called when the jvm has been initialized
void VMInit(jvmtiEnv *jvmti_env, JNIEnv *jni_env) {
    (*jni_env)->GetJavaVM(jni_env, &global_jvm);

    printf("Installing JVMTI callbacks\n");
    installCallbacks();
}

// is called when java agent is loaded
jint Agent_OnLoad(JavaVM *vm, char *options, void *reserved) {
    jvmtiEventCallbacks callbacks;
    jvmtiError error;
    jint result;

    // get JVMTI environment
    result = (*vm)->GetEnv(vm, (void **) &environment, JVMTI_VERSION_21);
    if (result != 0) {
        return JNI_ERR;
    }

    // Clear and then enable capabilities.
    jvmtiCapabilities capabilities;
    (void) memset(&capabilities, 0, sizeof(capabilities));
    capabilities.can_generate_method_entry_events = 1;

    error = (*environment)->AddCapabilities(environment, &capabilities);
    if (error != JNI_OK) {
        return JNI_ERR;
    }

    // Link callbacks
    (void) memset(&callbacks, 0, sizeof(callbacks));
    callbacks.VMInit = (void *) &VMInit;
    callbacks.MethodEntry = (void *) &MethodEntry;

    error = (*environment)->SetEventCallbacks(environment, &callbacks, (jint) sizeof(callbacks));
    if (error != JNI_OK) {
        return JNI_ERR;
    }

    // Set notification on vm init
    error = (*environment)->SetEventNotificationMode(environment, JVMTI_ENABLE,
                                                     JVMTI_EVENT_VM_INIT,
                                                     (jthread) NULL);
    if (error != JNI_OK) {
        return JNI_ERR;
    }

    // Tell JVM that agent loaded successfully
    return JNI_OK;
}

// is called when agent is unloaded
void Agent_OnUnload(JavaVM *vm) {
    printf("\nAgent Unloaded\n");
}
