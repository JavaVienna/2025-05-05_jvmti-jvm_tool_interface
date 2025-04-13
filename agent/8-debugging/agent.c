#include "jvmti.h"
#include <sys/un.h>
#include <stdlib.h>
#include <time.h>

// use 4-stacktraces for this agent

void VMInit(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread) {
    jclass clazz = (*jni_env)->FindClass(jni_env, "Lio/kay/Main;");
    jint methodCounter;
    jmethodID* methods;
    (*jvmti_env)->GetClassMethods(jvmti_env, clazz, &methodCounter, &methods);

    for (int i = 0; i < methodCounter; i++) {
        char* methodName;
        (*jvmti_env)->GetMethodName(jvmti_env, methods[i], &methodName, NULL, NULL);
        if (strcmp(methodName, "main") == 0) {
            (*jvmti_env)->SetBreakpoint(jvmti_env, methods[i], 0);
            return;
        }
    }
}

void Breakpoint(jvmtiEnv *jvmti_env,
                JNIEnv *jni_env,
                jthread thread,
                jmethodID method,
                jlocation location) {
    // only now activate single step events
    jvmtiError error = (*jvmti_env)->SetEventNotificationMode(jvmti_env, JVMTI_ENABLE, JVMTI_EVENT_SINGLE_STEP,
                                                     thread);
    if (error != JNI_OK) {
        printf("Error activating event\n");
    }
}

void SingleStep(jvmtiEnv *jvmti_env,
                JNIEnv *jni_env,
                jthread thread,
                jmethodID method,
                jlocation location) {
    printf("Currently in thread '%p', method '%p', and location '%ld'\n", thread, method, location);
}

jint Agent_OnLoad(JavaVM *vm, char *options, void *reserved) {
    jvmtiEventCallbacks callbacks;
    jvmtiError error;
    jint result;
    jvmtiEnv *environment;

    result = (*vm)->GetEnv(vm, (void **) &environment, JVMTI_VERSION_21);
    if (result != 0) {
        return JNI_ERR;
    }

    jvmtiCapabilities capabilities;
    (void) memset(&capabilities, 0, sizeof(capabilities));
    capabilities.can_generate_breakpoint_events = 1;
    capabilities.can_generate_single_step_events = 1;
    capabilities.can_generate_early_vmstart = 1;

    error = (*environment)->AddCapabilities(environment, &capabilities);
    if (error != JNI_OK) {
        return JNI_ERR;
    }

    (void) memset(&callbacks, 0, sizeof(callbacks));
    callbacks.Breakpoint = (void *) &Breakpoint;
    callbacks.SingleStep = (void *) &SingleStep;
    callbacks.VMInit = (void *) &VMInit;

    error = (*environment)->SetEventCallbacks(environment, &callbacks, (jint) sizeof(callbacks));
    if (error != JNI_OK) {
        return JNI_ERR;
    }

    error = (*environment)->SetEventNotificationMode(environment, JVMTI_ENABLE, JVMTI_EVENT_VM_INIT,
                                                     (jthread) NULL);
    if (error != JNI_OK) {
        return JNI_ERR;
    }
    error = (*environment)->SetEventNotificationMode(environment, JVMTI_ENABLE, JVMTI_EVENT_BREAKPOINT,
                                                     (jthread) NULL);
    if (error != JNI_OK) {
        return JNI_ERR;
    }

    return JNI_OK;
}
