#include "jvmti.h"
#include <sys/un.h>
#include <stdlib.h>
#include <time.h>

void ThreadStart(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread) {
    jvmtiThreadInfo thread_info;
    (*jvmti_env)->GetThreadInfo(jvmti_env, thread, &thread_info);

    printf("ThreadStart of %s\n", thread_info.name);
    (*jvmti_env)->Deallocate(jvmti_env, (unsigned char *) thread_info.name);
}

void ThreadEnd(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread) {
    jvmtiThreadInfo thread_info;
    (*jvmti_env)->GetThreadInfo(jvmti_env, thread, &thread_info);

    printf("ThreadEnd of %s\n", thread_info.name);
    (*jvmti_env)->Deallocate(jvmti_env, (unsigned char *) thread_info.name);
}

void MonitorWait(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, jobject object, jlong timeout) {
    printf("MonitorWait on object %p\n", object);
}

void MonitorWaited(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, jobject object, jboolean timed_out) {
    printf("MonitorWaitedon object %p\n", object);
}

void MonitorContendedEnter(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, jobject object) {
    jvmtiThreadInfo thread_info;
    (*jvmti_env)->GetThreadInfo(jvmti_env, thread, &thread_info);

    printf("MonitorContendedEnter of %s on object %p\n", thread_info.name, object);
    (*jvmti_env)->Deallocate(jvmti_env, (unsigned char *) thread_info.name);
}

void MonitorContendedEntered(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, jobject object) {
    jvmtiThreadInfo thread_info;
    (*jvmti_env)->GetThreadInfo(jvmti_env, thread, &thread_info);

    printf("MonitorContendedEntered of %s on object %p\n", thread_info.name, object);
    (*jvmti_env)->Deallocate(jvmti_env, (unsigned char *) thread_info.name);
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
    capabilities.can_signal_thread = 1;
    capabilities.can_generate_monitor_events = 1;

    error = (*environment)->AddCapabilities(environment, &capabilities);
    if (error != JNI_OK) {
        return JNI_ERR;
    }

    (void) memset(&callbacks, 0, sizeof(callbacks));
    callbacks.ThreadStart = (void *) &ThreadStart;
    callbacks.ThreadEnd = (void *) &ThreadEnd;
    callbacks.MonitorWait = (void *) &MonitorWait;
    callbacks.MonitorWaited = (void *) &MonitorWaited;
    callbacks.MonitorContendedEnter = (void *) &MonitorContendedEnter;
    callbacks.MonitorContendedEntered = (void *) &MonitorContendedEntered;

    error = (*environment)->SetEventCallbacks(environment, &callbacks, (jint) sizeof(callbacks));
    if (error != JNI_OK) {
        return JNI_ERR;
    }

    error = (*environment)->SetEventNotificationMode(environment, JVMTI_ENABLE, JVMTI_EVENT_THREAD_START,
                                                     (jthread) NULL);
    if (error != JNI_OK) {
        return JNI_ERR;
    }
    error = (*environment)->SetEventNotificationMode(environment, JVMTI_ENABLE, JVMTI_EVENT_THREAD_END,
                                                     (jthread) NULL);
    if (error != JNI_OK) {
        return JNI_ERR;
    }
    error = (*environment)->SetEventNotificationMode(environment, JVMTI_ENABLE, JVMTI_EVENT_MONITOR_WAIT,
                                                     (jthread) NULL);
    if (error != JNI_OK) {
        return JNI_ERR;
    }
    error = (*environment)->SetEventNotificationMode(environment, JVMTI_ENABLE, JVMTI_EVENT_MONITOR_WAITED,
                                                     (jthread) NULL);
    if (error != JNI_OK) {
        return JNI_ERR;
    }
    error = (*environment)->SetEventNotificationMode(environment, JVMTI_ENABLE, JVMTI_EVENT_MONITOR_CONTENDED_ENTER,
                                                     (jthread) NULL);
    if (error != JNI_OK) {
        return JNI_ERR;
    }
    error = (*environment)->SetEventNotificationMode(environment, JVMTI_ENABLE, JVMTI_EVENT_MONITOR_CONTENDED_ENTERED,
                                                     (jthread) NULL);
    if (error != JNI_OK) {
        return JNI_ERR;
    }

    return JNI_OK;
}
