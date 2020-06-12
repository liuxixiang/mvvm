#include <jni.h>
#include <string>

std::string jstring2str(JNIEnv *env1, jstring jstr);
jobject getApplication(JNIEnv *env);
void check_signature(JNIEnv *env, jobject context);
jstring get_cuid(JNIEnv *env, jobject context);
std::string get_device_id(JNIEnv *env, jobject context);
int base64_encode(const char *in_str, int in_len, char *out_str);
