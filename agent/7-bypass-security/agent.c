#include "jvmti.h"
#include "string.h"
#include "stdlib.h"

void Breakpoint(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, jmethodID method, jlocation location) {
    printf("(jvmti) In breakpoint\n");
    (*jvmti_env)->ForceEarlyReturnInt(jvmti_env, thread, 1);
    printf("(jvmti) Forced early return\n");

    // TODO print out secret
}

void VMInit(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread) {
    const jclass class = (*jni_env)->FindClass(jni_env, "io/kay/Main");
    jint methodCounter;
    jmethodID *methods;
    (*jvmti_env)->GetClassMethods(jvmti_env, class, &methodCounter, &methods);

    printf("(jvmti) methodCounter = %d\n", methodCounter);
    for (int i = 0; i < methodCounter; i++) {
        char *methodName;
        (*jvmti_env)->GetMethodName(jvmti_env, methods[i], &methodName, NULL, NULL);
        printf("(jvmti) methodName = %s\n", methodName);
        if (strcmp(methodName, "secretMatches") == 0) {
            printf("(jvmti) Set breakpoint at method secretMatches\n");
            (*jvmti_env)->SetBreakpoint(jvmti_env, methods[i], 0);
        }
        (*jvmti_env)->Deallocate(jvmti_env, (unsigned char *) methodName);
    }
}

JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM *jvm, char *options, void *reserved) {
    jvmtiEnv *jvmti;
    jint result = (*jvm)->GetEnv(jvm, (void **) &jvmti, JVMTI_VERSION);
    if (result != JNI_OK) {
        printf("Unable to access JVMTI\n");
    }

    jvmtiCapabilities capabilities;
    (void) memset(&capabilities, 0, sizeof(jvmtiCapabilities));
    capabilities.can_generate_early_class_hook_events = 1;
    capabilities.can_generate_all_class_hook_events = 1;
    capabilities.can_get_source_file_name = 1;
    capabilities.can_tag_objects = 1;
    capabilities.can_signal_thread = 1;
    capabilities.can_maintain_original_method_order = 1;
    capabilities.can_access_local_variables = 1;
    capabilities.can_generate_breakpoint_events = 1;
    capabilities.can_force_early_return = 1;

    (*jvmti)->AddCapabilities(jvmti, &capabilities);
    (*jvmti)->SetEventNotificationMode(jvmti, JVMTI_ENABLE, JVMTI_EVENT_VM_INIT, (jthread) NULL);
    (*jvmti)->SetEventNotificationMode(jvmti, JVMTI_ENABLE, JVMTI_EVENT_BREAKPOINT, (jthread) NULL);

    jvmtiEventCallbacks eventCallbacks;
    (void) memset(&eventCallbacks, 0, sizeof(eventCallbacks));
    eventCallbacks.VMInit = &VMInit;
    eventCallbacks.Breakpoint = &Breakpoint;

    (*jvmti)->SetEventCallbacks(jvmti, &eventCallbacks, (jint) sizeof(eventCallbacks));

    return JNI_OK;
}
