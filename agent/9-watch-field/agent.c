#include "jvmti.h"
#include <sys/un.h>
#include <stdlib.h>
#include <time.h>

// use with 8-quarkus-app

void ClassPrepare(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, const jclass klass) {
    char *sig;
    (*jvmti_env)->GetClassSignature(jvmti_env, klass, &sig, NULL);

    if (strcmp(sig, "Lio/kay/ProviderController;") == 0) {
        jint fieldCounter;
        jfieldID *fields;
        (*jvmti_env)->GetClassFields(jvmti_env, klass, &fieldCounter, &fields);

        for (int i = 0; i < fieldCounter; i++) {
            char *fieldName;
            (*jvmti_env)->GetFieldName(jvmti_env, klass, fields[i], &fieldName, NULL, NULL);
            if (strcmp(fieldName, "someNumber") == 0) {
                (*jvmti_env)->SetFieldAccessWatch(jvmti_env, klass, fields[i]);
                (*jvmti_env)->SetFieldModificationWatch(jvmti_env, klass, fields[i]);
                return;
            }
        }
    }

    (*jvmti_env)->Deallocate(jvmti_env, (unsigned char *)sig);
}

int currentlyAccessed = 0;
void FieldAccess(jvmtiEnv *jvmti_env,
                 JNIEnv *jni_env,
                 jthread thread,
                 jmethodID method,
                 jlocation location,
                 jclass field_klass,
                 jobject object,
                 jfieldID field) {
    if (currentlyAccessed != 0) {
        return;
    }
    currentlyAccessed = 1;

    char *methodName;
    (*jvmti_env)->GetMethodName(jvmti_env, method, &methodName, NULL, NULL);
    const jint value = (*jni_env)->GetIntField(jni_env, object, field);
    printf("Access to field 'someNumber' occurred in method '%s' has value '%d'\n", methodName, value);

    (*jvmti_env)->Deallocate(jvmti_env, (unsigned char*) methodName);
    currentlyAccessed = 0;
}

int currentlyModified = 0;
void FieldModification(jvmtiEnv *jvmti_env,
                       JNIEnv *jni_env,
                       jthread thread,
                       jmethodID method,
                       jlocation location,
                       jclass field_klass,
                       jobject object,
                       jfieldID field,
                       char signature_type,
                       jvalue new_value) {
    if (currentlyModified != 0) {
        return;
    }
    currentlyModified = 1;

    char *methodName;
    (*jvmti_env)->GetMethodName(jvmti_env, method, &methodName, NULL, NULL);
    const jint value = (*jni_env)->GetIntField(jni_env, object, field);
    printf("Modification of field 'someNumber' occurred in method '%s' changes value from '%d' to '%d'\n", methodName, value, new_value.i);

    (*jvmti_env)->Deallocate(jvmti_env, (unsigned char*) methodName);
    currentlyModified = 0;
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
    capabilities.can_generate_field_access_events = 1;
    capabilities.can_generate_field_modification_events = 1;

    error = (*environment)->AddCapabilities(environment, &capabilities);
    if (error != JNI_OK) {
        return JNI_ERR;
    }

    (void) memset(&callbacks, 0, sizeof(callbacks));
    callbacks.FieldModification = (void *) &FieldModification;
    callbacks.FieldAccess = (void *) &FieldAccess;
    callbacks.ClassPrepare = (void *) &ClassPrepare;

    error = (*environment)->SetEventCallbacks(environment, &callbacks, (jint) sizeof(callbacks));
    if (error != JNI_OK) {
        return JNI_ERR;
    }

    error = (*environment)->SetEventNotificationMode(environment, JVMTI_ENABLE, JVMTI_EVENT_FIELD_ACCESS,
                                                     (jthread) NULL);
    if (error != JNI_OK) {
        return JNI_ERR;
    }
    error = (*environment)->SetEventNotificationMode(environment, JVMTI_ENABLE, JVMTI_EVENT_FIELD_MODIFICATION,
                                                     (jthread) NULL);
    if (error != JNI_OK) {
        return JNI_ERR;
    }
    error = (*environment)->SetEventNotificationMode(environment, JVMTI_ENABLE, JVMTI_EVENT_CLASS_PREPARE,
                                                     (jthread) NULL);
    if (error != JNI_OK) {
        return JNI_ERR;
    }

    return JNI_OK;
}
