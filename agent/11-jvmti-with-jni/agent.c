#include "jvmti.h"
#include "io_kay_Main.h"
#include <string.h>

// use 8-quarkus-app for this agent

jvmtiEnv *jvmti;

// from https://github.com/JetBrains/android/blob/3ba401cb8504cd73d1dd9def77f9bd1f809e9004/bleak/resources/com/android/tools/idea/bleak/agents/jniBleakHelper.cpp#L10
JNIEXPORT void JNICALL Java_io_kay_ProviderController_nativeMethod(JNIEnv *env, jobject obj) {
    printf("Suspending all threads\n");
    jint nthreads;
    jthread *threads;
    (*jvmti)->GetAllThreads(jvmti, &nthreads, &threads);

    for (int i = 0; i < nthreads; i++) {
        (*jvmti)->SuspendThread(jvmti, threads[i]);
    }
}

jint Agent_OnLoad(JavaVM *vm, char *options, void *reserved) {
    jvmtiEnv *environment;

    jint result = (*vm)->GetEnv(vm, (void **) &environment, JVMTI_VERSION_21);
    if (result != 0) {
        return JNI_ERR;
    }

    jvmti = environment;

    jvmtiCapabilities capabilities;
    (void) memset(&capabilities, 0, sizeof(capabilities));
    capabilities.can_generate_breakpoint_events = 1;

    const jvmtiError error = (*environment)->AddCapabilities(environment, &capabilities);
    if (error != JNI_OK) {
        return JNI_ERR;
    }

    return JNI_OK;
}
