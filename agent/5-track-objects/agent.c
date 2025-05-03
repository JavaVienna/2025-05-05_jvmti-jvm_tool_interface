#include "jvmti.h"
#include "string.h"
#include "stdbool.h"
#include "stdlib.h"

jlong tag = 42;

bool checkJVMTIError(jvmtiEnv *jvmti, const jvmtiError errNum, const char *str) {
    if (errNum != JVMTI_ERROR_NONE) {
        char *errnum_str;
        errnum_str = NULL;

        (void) (*jvmti)->GetErrorName(jvmti, errNum, &errnum_str);

        printf("JVMTI: ERROR %d %s: %s \n", errNum,
               (errnum_str == NULL ? "Unknown error" : errnum_str), (str == NULL ? "" : str));

        return false;
    }

    return true;
}

jvmtiIterationControl iterateCallback(jlong class_tag, jlong size, jlong *tag_ptr, void *user_data) {
    int *personCounter = user_data;
    (*personCounter)++;
    *tag_ptr = tag;
    return JVMTI_ITERATION_CONTINUE;
}

void VMInit(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread) {
    jclass klass = (*jni_env)->FindClass(jni_env, "Lio/kay/Main;");
    jmethodID methodId = (*jni_env)->GetStaticMethodID(jni_env, klass, "execute", "(Ljava/util/List;)V");

    jlocation start;
    jlocation end;
    (*jvmti_env)->GetMethodLocation(jvmti_env, methodId, &start, &end);
    (*jvmti_env)->SetBreakpoint(jvmti_env, methodId, end - 23); // without
    // (*jvmti_env)->SetBreakpoint(jvmti_env, methodId, end - 26); // with System.gc()

    jint variableCount;
    jvmtiLocalVariableEntry *variables;
    (*jvmti_env)->GetLocalVariableTable(jvmti_env, methodId, &variableCount, &variables);

    printf("(jvmti) Method %p has %d local variables: \n", methodId, variableCount);
    for (int i = 0; i < variableCount; i++) {
        printf("(jvmti) Variable #%d: %s \n", i, variables[i].name);
    }
}

void Breakpoint(jvmtiEnv *jvmti_env,
                JNIEnv *jni_env,
                jthread thread,
                jmethodID method,
                jlocation location) {
    const jclass personClass = (*jni_env)->FindClass(jni_env, "Lio/kay/Main$Person;");

    jint fieldCounter;
    jfieldID *fieldIDs;
    (*jvmti_env)->GetClassFields(jvmti_env, personClass, &fieldCounter, &fieldIDs);

    char *fieldName;
    (*jvmti_env)->GetFieldName(jvmti_env, personClass, fieldIDs[0], &fieldName, NULL, NULL);

    const int personCounter = 0;
    (*jvmti_env)->IterateOverInstancesOfClass(jvmti_env, personClass, JVMTI_HEAP_OBJECT_EITHER, iterateCallback,
                                              &personCounter);
    printf("(jvmti) personCounter: %d\n", personCounter);

    jint objectCounter;
    jobject *objects;
    jlong *tags;
    (*jvmti_env)->GetObjectsWithTags(jvmti_env, 1, &tag, &objectCounter, &objects, &tags);

    for (int i = 0; i < objectCounter; i++) {
        const jobject personName = (*jni_env)->GetObjectField(jni_env, objects[i], fieldIDs[0]);
        jboolean isCopy;
        const char *name = (*jni_env)->GetStringUTFChars(jni_env, personName, &isCopy);

        if (strcmp(name, "Reflected Person") == 0) {
            const jobject newString = (*jni_env)->NewStringUTF(jni_env, "Agent edit for Reflected");
            (*jni_env)->SetObjectField(jni_env, objects[i], fieldIDs[0], newString);
        }
        if (strcmp(name, "Mister X") == 0) {
            const jobject newString = (*jni_env)->NewStringUTF(jni_env, "X edited by agent");
            (*jni_env)->SetObjectField(jni_env, objects[i], fieldIDs[0], newString);
        }
        (*jvmti_env)->SetTag(jvmti_env, objects[i], 0);
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
    capabilities.can_generate_breakpoint_events = 1;
    capabilities.can_maintain_original_method_order = 1;
    capabilities.can_access_local_variables = 1;

    jvmtiError error = (*jvmti)->AddCapabilities(jvmti, &capabilities);
    checkJVMTIError(jvmti, error, "Unable to set Capabilities");

    checkJVMTIError(jvmti, error, "Cannot set gc finish Event Notification");
    error = (*jvmti)->SetEventNotificationMode(jvmti, JVMTI_ENABLE, JVMTI_EVENT_VM_INIT, (jthread) NULL);
    checkJVMTIError(jvmti, error, "Cannot set vm init Event Notification");
    error = (*jvmti)->SetEventNotificationMode(jvmti, JVMTI_ENABLE, JVMTI_EVENT_BREAKPOINT, (jthread) NULL);
    checkJVMTIError(jvmti, error, "Cannot set breakpoint Event Notification");

    jvmtiEventCallbacks eventCallbacks;
    (void) memset(&eventCallbacks, 0, sizeof(eventCallbacks));
    eventCallbacks.VMInit = &VMInit;
    eventCallbacks.Breakpoint = &Breakpoint;

    error = (*jvmti)->SetEventCallbacks(jvmti, &eventCallbacks, (jint) sizeof(eventCallbacks));
    checkJVMTIError(jvmti, error, "Cannot set Event Callbacks.");

    return JNI_OK;
}
