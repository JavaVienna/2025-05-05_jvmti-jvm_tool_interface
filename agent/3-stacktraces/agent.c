#include "jvmti.h"
#include "string.h"
#include "stdbool.h"
#include "stdlib.h"

bool checkJVMTIError(jvmtiEnv *jvmti, const jvmtiError errNum, const char *str) {
    if (errNum != JVMTI_ERROR_NONE) {
        char *errnum_str;
        errnum_str = NULL;

        (void) (*jvmti)->GetErrorName(jvmti, errNum, &errnum_str);

        printf("(jvmti) ERROR %d %s: %s \n", errNum,
               (errnum_str == NULL ? "Unknown error" : errnum_str), (str == NULL ? "" : str));

        return false;
    }

    return true;
}

void JNICALL callbackExceptionEvent(jvmtiEnv *jvmti, JNIEnv *jni_env, jthread thread, jmethodID method,
                                    jlocation location,
                                    jobject exception, jmethodID catchMethod, jlocation catchLocation) {
    if ((*jni_env)->ExceptionCheck(jni_env) == JNI_FALSE) {
        printf("(jvmti) JVMTI callback is called, but JNI tells us, that no exception is pending\n\n");
    }
    // JVMTI is notified about exceptions before the JNI is aware of it, so clearing the exception is impossible right now
    (*jni_env)->ExceptionClear(jni_env);

    jvmtiFrameInfo frames[10];
    jint count, entryCountPtr, methodArgumentSize;
    char *sig, *gsig;
    jclass declaringClazz;
    jvmtiLocalVariableEntry *localVariableEntry;

    jvmtiError error = (*jvmti)->GetStackTrace(jvmti, thread, 0, 5, frames, &count);

    if (checkJVMTIError(jvmti, error, "Cannot Get Frame") && count > 0) {
        char *methodName, *className;

        for (int i = count - 1; i >= 0; i--) {
            error = (*jvmti)->GetMethodName(jvmti, frames[i].method, &methodName, &sig, &gsig);
            checkJVMTIError(jvmti, error, "Cannot get Method Name");

            error = (*jvmti)->GetArgumentsSize(jvmti, frames[i].method, &methodArgumentSize);
            checkJVMTIError(jvmti, error, "Cannot get Method Argument Size");

            error = (*jvmti)->GetMethodDeclaringClass(jvmti, frames[i].method, &declaringClazz);
            checkJVMTIError(jvmti, error, "Cannot get Method Declaring Class");

            error = (*jvmti)->GetClassSignature(jvmti, declaringClazz, &className, NULL);
            checkJVMTIError(jvmti, error, "Cannot get Class Signature");

            error = (*jvmti)->GetLocalVariableTable(jvmti, frames[i].method, &entryCountPtr, &localVariableEntry);
            checkJVMTIError(jvmti, error, "Cannot Get Local Variable Table");

            printf("(jvmti) Got Exception in Method '%s' of class '%s' with %d parameters\n", methodName, className,
                   methodArgumentSize);
            for (int j = 0; j < entryCountPtr; ++j) {
                jobject localVarPtr;
                error = (*jvmti)->GetLocalObject(jvmti, thread, i, localVariableEntry[j].slot, &localVarPtr);
                checkJVMTIError(jvmti, error, "Cannot Get Local Object");

                jboolean isCopy;
                const char *result = (*jni_env)->GetStringUTFChars(jni_env, localVarPtr, &isCopy);

                printf("(jvmti) method contains local variable '%s' of type '%s' with value '%s' \n",
                       localVariableEntry[j].name, localVariableEntry[j].signature, result);
            }
            printf("\n");
        }

        const jclass classOfException = (*jni_env)->GetObjectClass(jni_env, exception);
        (*jvmti)->GetClassSignature(jvmti, classOfException, &sig, &gsig);
        printf("(jvmti) Exception is of class %s", sig);

        const jmethodID methodId = (*jni_env)->GetMethodID(jni_env, classOfException, "getMessage",
                                                           "()Ljava/lang/String;");
        const jobject exceptionMessage = (*jni_env)->CallObjectMethod(jni_env, exception, methodId);
        jboolean isCopy;
        const char *result = (*jni_env)->GetStringUTFChars(jni_env, exceptionMessage, &isCopy);
        printf(" with message '%s'\n", result);
    }

    // printf("(jvmti) Now we skip catching or handling the exception by the application code...\n");
    // error = (*jvmti)->ForceEarlyReturnInt(jvmti, thread, 1);
    // checkJVMTIError(jvmti, error, "Cannot ForceEarlyReturn");
}

JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM *jvm, char *options, void *reserved) {
    jvmtiEnv *jvmti;
    jint result = (*jvm)->GetEnv(jvm, (void **) &jvmti, JVMTI_VERSION);
    if (result != JNI_OK) {
        printf("Unable to access JVMTI\n");
    }

    jvmtiCapabilities capabilities;
    (void) memset(&capabilities, 0, sizeof(jvmtiCapabilities));
    capabilities.can_generate_exception_events = 1;
    capabilities.can_access_local_variables = 1;
    capabilities.can_get_constant_pool = 1;
    capabilities.can_get_bytecodes = 1;
    capabilities.can_force_early_return = 1;

    jvmtiError error = (*jvmti)->AddCapabilities(jvmti, &capabilities);
    checkJVMTIError(jvmti, error, "Unable to set Capabilities");

    error = (*jvmti)->SetEventNotificationMode(jvmti, JVMTI_ENABLE, JVMTI_EVENT_EXCEPTION, (jthread) NULL);
    checkJVMTIError(jvmti, error, "Cannot set Exception Event Notification");

    jvmtiEventCallbacks eventCallbacks;
    (void) memset(&eventCallbacks, 0, sizeof(eventCallbacks));
    eventCallbacks.Exception = &callbackExceptionEvent;

    error = (*jvmti)->SetEventCallbacks(jvmti, &eventCallbacks, (jint) sizeof(eventCallbacks));
    checkJVMTIError(jvmti, error, "Cannot set Event Callbacks.");

    return JNI_OK;
}
