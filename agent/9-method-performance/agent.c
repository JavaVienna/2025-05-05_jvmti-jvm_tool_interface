#include "jvmti.h"
#include <sys/un.h>
#include <stdlib.h>
#include <time.h>

// use with 3-stacktraces

typedef struct methodInfo {
    jmethodID method;
    char *methodName;
    struct timespec start_time;
} methodInfo;

int size = 0;
methodInfo map[20000];

methodInfo *findMethod(const jmethodID method) {
    for (int i = 0; i < size; i++) {
        if (map[i].method == method) {
            return &map[i];
        }
    }

    return NULL;
}

double diff(const struct timespec start, const struct timespec end) {
    return ((double) end.tv_sec * 1.0e3 + 1.0e-6 * end.tv_nsec) -
           ((double) start.tv_sec * 1.0e3 + 1.0e-6 * start.tv_nsec);
}

void MethodEntry(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, jmethodID method) {
    char *methodName;
    (*jvmti_env)->GetMethodName(jvmti_env, method, &methodName, NULL, NULL);

    struct timespec start_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    const methodInfo newMethodInfo = {method, methodName, start_time};

    map[size++] = newMethodInfo;
}

void MethodExit(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, jmethodID method) {
    const methodInfo *storedMethod = findMethod(method);
    if (storedMethod != NULL) {
        struct timespec end;
        clock_gettime(CLOCK_MONOTONIC, &end);
        printf("Method '%s' took %.3fms\n", storedMethod->methodName, diff(storedMethod->start_time, end));
    }
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
    capabilities.can_generate_method_entry_events = 1;
    capabilities.can_generate_method_exit_events = 1;

    error = (*environment)->AddCapabilities(environment, &capabilities);
    if (error != JNI_OK) {
        return JNI_ERR;
    }

    (void) memset(&callbacks, 0, sizeof(callbacks));
    callbacks.MethodEntry = (void *) &MethodEntry;
    callbacks.MethodExit = (void *) &MethodExit;

    error = (*environment)->SetEventCallbacks(environment, &callbacks, (jint) sizeof(callbacks));
    if (error != JNI_OK) {
        return JNI_ERR;
    }

    error = (*environment)->SetEventNotificationMode(environment, JVMTI_ENABLE, JVMTI_EVENT_METHOD_ENTRY,
                                                     (jthread) NULL);
    if (error != JNI_OK) {
        return JNI_ERR;
    }

    error = (*environment)->
            SetEventNotificationMode(environment, JVMTI_ENABLE, JVMTI_EVENT_METHOD_EXIT, (jthread) NULL);
    if (error != JNI_OK) {
        return JNI_ERR;
    }

    return JNI_OK;
}
