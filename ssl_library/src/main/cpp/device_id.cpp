#include "device_id.h"
#include <string.h>
#include <stdlib.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

static const char HexCode[] = "0123456789abcdef";
// 签名的md5值
static const char *md5final = "66372629c89a0ea011333ba5450d9745";

jbyteArray jstring2jbyteArray(JNIEnv *env1, jstring jstr) {
    jclass clsstring = env1->FindClass("java/lang/String");
    jstring strencode = env1->NewStringUTF("utf-8");
    jmethodID mid = env1->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    return (jbyteArray) env1->CallObjectMethod(jstr, mid, strencode);
}

std::string jstring2str(JNIEnv *env1, jstring jstr) {
    jbyteArray barr = jstring2jbyteArray(env1, jstr);
    jsize alen = env1->GetArrayLength(barr);
    jbyte *ba = env1->GetByteArrayElements(barr, JNI_FALSE);
    std::string rtn((char *) ba, alen);
    env1->ReleaseByteArrayElements(barr, ba, JNI_FALSE);
    return rtn;
}

std::string jbyteArray2hex(JNIEnv *env, jbyteArray jarray) {
    jsize array_size = env->GetArrayLength(jarray);
    jbyte *sha1 = env->GetByteArrayElements(jarray, NULL);
    char hex_sha[array_size * 2 + 1];
    for (int i = 0; i < array_size; ++i) {
        hex_sha[2 * i] = HexCode[((unsigned char) sha1[i]) / 16];
        hex_sha[2 * i + 1] = HexCode[((unsigned char) sha1[i]) % 16];
    }
    hex_sha[array_size * 2] = '\0';
    env->ReleaseByteArrayElements(jarray, sha1, JNI_FALSE);
    return hex_sha;
}

jobject getApplication(JNIEnv *env) {
    jclass localClass = env->FindClass("android/app/ActivityThread");
    if (localClass != NULL) {
        jmethodID getapplication = env->GetStaticMethodID(localClass, "currentApplication",
                                                          "()Landroid/app/Application;");
        if (getapplication != NULL) {
            jobject application = env->CallStaticObjectMethod(localClass, getapplication);
            return application;
        }
        return NULL;
    }
    return NULL;
}

jbyteArray calc_md5(JNIEnv *env, jbyteArray input) {
    //MessageDigest.getInstance("SHA1")
    jclass message_digest_class = env->FindClass("java/security/MessageDigest");
    jmethodID methodId = env->GetStaticMethodID(message_digest_class, "getInstance", "(Ljava/lang/String;)Ljava/security/MessageDigest;");
    //如果取SHA1则输入SHA1
    //jstring sha1_jstring=env->NewStringUTF(env,"SHA1");
    jstring sha1_jstring = env->NewStringUTF("MD5");
    jobject sha1_digest = env->CallStaticObjectMethod(message_digest_class, methodId, sha1_jstring);
    //sha1.digest (certByte)
    methodId = env->GetMethodID(message_digest_class, "digest", "([B)[B");
    return (jbyteArray) env->CallObjectMethod(sha1_digest, methodId, input);
}

void check_signature(JNIEnv *env, jobject context) {
    //获取到Context
    jclass activity = env->GetObjectClass(context);
    // 得到 getPackageManager 方法的 ID
    jmethodID methodID_func = env->GetMethodID(activity, "getPackageManager",
                                               "()Landroid/content/pm/PackageManager;");
    // 获得PackageManager对象
    jobject packageManager = env->CallObjectMethod(context, methodID_func);
    jclass packageManagerclass = env->GetObjectClass(packageManager);
    //得到 getPackageName 方法的 ID
    jmethodID methodID_pack = env->GetMethodID(activity, "getPackageName", "()Ljava/lang/String;");
    //获取包名
    jstring name_str = (jstring) (env->CallObjectMethod(context, methodID_pack));
    // 得到 getPackageInfo 方法的 ID
    jmethodID methodID_pm = env->GetMethodID(packageManagerclass, "getPackageInfo",
                                             "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    // 获得应用包的信息
    jobject package_info = env->CallObjectMethod(packageManager, methodID_pm, name_str, 64);
    // 获得 PackageInfo 类
    jclass package_infoclass = env->GetObjectClass(package_info);
    // 获得签名数组属性的 ID
    jfieldID fieldID_signatures = env->GetFieldID(package_infoclass, "signatures",
                                                  "[Landroid/content/pm/Signature;");
    // 得到签名数组，待修改
    jobject signatur = env->GetObjectField(package_info, fieldID_signatures);
    jobjectArray signatures = (jobjectArray) (signatur);
    // 得到签名
    jobject signature = env->GetObjectArrayElement(signatures, 0);
    // 获得 Signature 类，待修改
    jclass signature_clazz = env->GetObjectClass(signature);
    //---获得签名byte数组
    jmethodID tobyte_methodId = env->GetMethodID(signature_clazz, "toByteArray", "()[B");
    jbyteArray signature_byte = (jbyteArray) env->CallObjectMethod(signature, tobyte_methodId);
    //把byte数组转成流
    jclass byte_array_input_class = env->FindClass("java/io/ByteArrayInputStream");
    jmethodID init_methodId = env->GetMethodID(byte_array_input_class, "<init>", "([B)V");
    jobject byte_array_input = env->NewObject(byte_array_input_class, init_methodId,
                                              signature_byte);
    //实例化X.509
    jclass certificate_factory_class = env->FindClass("java/security/cert/CertificateFactory");
    jmethodID certificate_methodId = env->GetStaticMethodID(certificate_factory_class,
                                                            "getInstance",
                                                            "(Ljava/lang/String;)Ljava/security/cert/CertificateFactory;");
    jstring x_509_jstring = env->NewStringUTF("X.509");
    jobject cert_factory = env->CallStaticObjectMethod(certificate_factory_class,
                                                       certificate_methodId, x_509_jstring);
    //certFactory.generateCertificate(byteIn);
    jmethodID certificate_factory_methodId = env->GetMethodID(certificate_factory_class,
                                                              "generateCertificate",
                                                              ("(Ljava/io/InputStream;)Ljava/security/cert/Certificate;"));
    jobject x509_cert = env->CallObjectMethod(cert_factory, certificate_factory_methodId,
                                              byte_array_input);

    jclass x509_cert_class = env->GetObjectClass(x509_cert);
    jmethodID x509_cert_methodId = env->GetMethodID(x509_cert_class, "getEncoded", "()[B");
    jbyteArray cert_byte = (jbyteArray) env->CallObjectMethod(x509_cert, x509_cert_methodId);
    jbyteArray md5_byte = calc_md5(env, cert_byte);

    //toHexString
    std::string hex_md5 = jbyteArray2hex(env, md5_byte);
    if (hex_md5 != md5final) {
        exit(0);
    }
}

jstring get_cuid(JNIEnv *env, jobject context) {
    jclass deviceUtil = env->FindClass("com/linken/common/DeviceUtil");
    jmethodID method = env->GetStaticMethodID(deviceUtil, "getDeviceId",
                                              "(Landroid/content/Context;)Ljava/lang/String;");
    return (jstring) env->CallStaticObjectMethod(deviceUtil, method, context);


}

int base64_encode(const char *in_str, int in_len, char *out_str) {
    BIO *b64, *bio;
    BUF_MEM *bptr = NULL;
    size_t size = 0;

    if (in_str == NULL || out_str == NULL)
        return -1;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_write(bio, in_str, in_len);
    BIO_flush(bio);

    BIO_get_mem_ptr(bio, &bptr);
    memcpy(out_str, bptr->data, bptr->length);
    out_str[bptr->length] = '\0';
    size = bptr->length;

    BIO_free_all(bio);
    return size;
}