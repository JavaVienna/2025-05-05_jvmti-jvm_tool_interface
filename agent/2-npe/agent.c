#include "jvmti.h"
#include "classfile_constants.h"
#include <sys/un.h>
#include <stdlib.h>

JavaVM *global_jvm;
jvmtiEnv *environment;

static jclass nullPointerException = NULL;
static jfieldID detailMessage = NULL;

// boilerplate to fetch the bytecode operation
const char* get_exception_message(unsigned char bytecode) {
    switch (bytecode) {
        case JVM_OPC_iaload: return "Load from null int array at bci %d";
        case JVM_OPC_laload: return "Load from null long array at bci %d";
        case JVM_OPC_faload: return "Load from null float array at bci %d";
        case JVM_OPC_daload: return "Load from null double array at bci %d";
        case JVM_OPC_aaload: return "Load from null Object array at bci %d";
        case JVM_OPC_baload: return "Load from null byte/boolean array at bci %d";
        case JVM_OPC_caload: return "Load from null char array at bci %d";
        case JVM_OPC_saload: return "Load from null short array at bci %d";

        case JVM_OPC_iastore: return "Store into null int array at bci %d";
        case JVM_OPC_lastore: return "Store into null long array at bci %d";
        case JVM_OPC_fastore: return "Store into null float array at bci %d";
        case JVM_OPC_dastore: return "Store into null double array at bci %d";
        case JVM_OPC_aastore: return "Store into null Object array at bci %d";
        case JVM_OPC_bastore: return "Store into null byte/boolean array at bci %d";
        case JVM_OPC_castore: return "Store into null char array at bci %d";
        case JVM_OPC_sastore: return "Store into null short array at bci %d";

        case JVM_OPC_arraylength: return "Get .length of null array";

        case JVM_OPC_getfield: return "Get field '%s' of null object at bci %d";
        case JVM_OPC_putfield: return "Put field '%s' of null object at bci %d";

        case JVM_OPC_invokevirtual: // fall through
        case JVM_OPC_invokespecial: // fall through
        case JVM_OPC_invokeinterface: return "Called method '%s' on null object at bci %d";

        case JVM_OPC_monitorenter: // fall through
        case JVM_OPC_monitorexit: return "Synchronized on null monitor at bci %d";

        default: return NULL;
    }
}

unsigned char get_u2(const unsigned char* bytes) {
    return bytes[0] << 8 | bytes[1];
}

unsigned char* get_cpool_at(unsigned char* cpool, unsigned char index) {
    // Length in bytes of a constant pool item with the given tag
    static unsigned char cp_item_size[] = {0, 3, 0, 5, 5, 9, 9, 3, 3, 5, 5, 5, 5, 4, 3, 5, 5, 3, 3};

    for (unsigned int i = 1; i < index; i++) {
        unsigned char tag = cpool[0];
        cpool += tag == JVM_CONSTANT_Utf8 ? 3 + get_u2(cpool + 1) : cp_item_size[tag];
    }

    return cpool;
}

char* get_name_from_cpool(jvmtiEnv* jvmti, jmethodID method, const unsigned char* bytecodes) {
    jclass holder;
    (*jvmti)->GetMethodDeclaringClass(jvmti, method, &holder);

    jint cpool_count;
    jint cpool_bytes;
    unsigned char* cpool;
    if ((*jvmti)->GetConstantPool(jvmti, holder, &cpool_count, &cpool_bytes, &cpool) != 0) {
        return strdup("<unknown>");
    }

    unsigned char* ref = get_cpool_at(cpool, get_u2(bytecodes + 1));       // CONSTANT_Fieldref / Methodref
    unsigned char* name_and_type = get_cpool_at(cpool, get_u2(ref + 3));   // CONSTANT_NameAndType
    unsigned char* name = get_cpool_at(cpool, get_u2(name_and_type + 1));  // CONSTANT_Utf8

    size_t name_length = get_u2(name + 1);
    char* result = (char*) malloc(name_length + 1);
    memcpy(result, name + 3, name_length);
    result[name_length] = 0;

    (*jvmti)->Deallocate(jvmti, cpool);
    return result;
}

void Exception(jvmtiEnv *jvmti_env,
               JNIEnv* jni_env,
               jthread thread,
               jmethodID method,
               jlocation location,
               jobject exception,
               jmethodID catch_method,
               jlocation catch_location) {

    if (nullPointerException == NULL || detailMessage == NULL ||
        !(*jni_env)->IsInstanceOf(jni_env, exception, nullPointerException)) {
        return;
    }

    jint bytecode_count;
    unsigned char* bytecodes;
    if ((*jvmti_env)->GetBytecodes(jvmti_env, method, &bytecode_count, &bytecodes) != 0) {
        return;
    }

    if (location >= 0 && location < bytecode_count) {
        const char* message = get_exception_message(bytecodes[location]);
        if (message != NULL) {
            char buf[400];
            if (strstr(message, "%s") != NULL) {
                char* name = get_name_from_cpool(jvmti_env, method, bytecodes + location);
                snprintf(buf, sizeof(buf), message, name, (int) location);
                free(name);
            } else {
                sprintf(buf, message, (int) location);
            }
            (*jni_env)->SetObjectField(jni_env, exception, detailMessage, (*jni_env)->NewStringUTF(jni_env, buf));
        }
    }

    (*jvmti_env)->Deallocate(jvmti_env, bytecodes);
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
        JVMTI_EVENT_EXCEPTION
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

    jclass localNPE = (*jni_env)->FindClass(jni_env, "java/lang/NullPointerException");
    nullPointerException = (jclass) (*jni_env)->NewGlobalRef(jni_env, localNPE);

    jclass Throwable = (*jni_env)->FindClass(jni_env, "java/lang/Throwable");
    detailMessage = (*jni_env)->GetFieldID(jni_env, Throwable, "detailMessage", "Ljava/lang/String;");
}

// is called when java agent is loaded
jint Agent_OnLoad(JavaVM *vm, char *options, void *reserved) {
    jvmtiEventCallbacks callbacks;
    jvmtiError error;
    jint result;

    // get JVMTI environment
    result = (*vm)->GetEnv(vm, (void **) &environment, JVMTI_VERSION_11);
    if (result != 0) {
        return JNI_ERR;
    }

    // Clear and then enable capabilities.
    jvmtiCapabilities capabilities;
    (void) memset(&capabilities, 0, sizeof(capabilities));
    capabilities.can_generate_exception_events = 1;
    capabilities.can_get_bytecodes = 1;
    capabilities.can_get_constant_pool = 1;

    error = (*environment)->AddCapabilities(environment, &capabilities);
    if (error != JNI_OK) {
        return JNI_ERR;
    }

    // Link callbacks
    (void) memset(&callbacks, 0, sizeof(callbacks));
    callbacks.VMInit = (void *) &VMInit;
    callbacks.Exception = (void *) &Exception;

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
