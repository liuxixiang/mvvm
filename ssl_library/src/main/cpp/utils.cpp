#include <jni.h>
#include <sstream>
#include <iomanip>
#include <openssl/md5.h>
#include <openssl/aes.h>
#include <vector>
#include "device_id.h"

static std::string device_id;
// 秘钥
static unsigned char key[] = "N76$Yyl5AwP8yabW";

std::string step2(JNIEnv *env, jobject context, std::string &md5, std::string &cuid, int b2) {
    switch (b2) {
        case 0:
            return md5.substr(0, 16) + cuid + md5.substr(16, 32);

        case 1:
            return md5.substr(0, 8) + cuid + md5.substr(8, 32);

        case 2: //
            return md5 + cuid;

        default:
            return std::string();
    }
}

std::string
step1(JNIEnv *env, jobject context, std::string &&md5, std::string &cuid, int b1, int b2) {
    int count = 0;
    switch (b1) {
        case 0:
            for (int i = 0; i < md5.size(); ++i) {
                if (isdigit(md5[i])) {
                    if (2 == (24 + count * 9) % 3) {
                        md5[i] += 1;
                    }
                    ++count;
                }
            }
            break;

        case 1:
            for (int i = 0; i < md5.size(); ++i) {
                if (isalpha(md5[i])) {
                    if ((count >> 2) % 3) {
                        md5[i] += 1;
                    }
                    ++count;
                }
            }
            break;

        case 2: //
            for (int i = 0; i < md5.size(); ++i) {
                if (isalpha(md5[i])) {
                    if ((count + 1394) % 2) {
//                        md5[i] += 'A' - 'a';
                    }
                    ++count;
                }
            }
            break;
    }
    return step2(env, context, md5, cuid, b2);
}

std::string step0(JNIEnv *env, jobject context, int b0, int b1, int b2) {
    std::string cuid = jstring2str(env, get_cuid(env, context));
    unsigned char buf[16];
    unsigned char *md5;
    std::string plain;
    switch (b0) {
        case 0:
            plain = "g6#kI(gdDiq!3uPY" + cuid;
            md5 = MD5((const unsigned char *) plain.c_str(), plain.size(), buf);
            break;

        case 1: //
            plain = cuid + "f5^Ij&gdSds%7mLR";
            md5 = MD5((const unsigned char *) plain.c_str(), plain.size(), buf);
            break;

        case 2:
            plain = "s0@Ld^se" + cuid + "Rsb|4xTS";
            md5 = MD5((const unsigned char *) plain.c_str(), plain.size(), buf);
            break;
    }

    std::ostringstream oss;
    for (int i = 0; i < 16; i++) {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int) md5[i];
    }

    return step1(env, context, oss.str(), cuid, b1, b2);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_a(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 0, 0, 0);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_b(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 0, 0, 1);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_c(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 0, 0, 2);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_d(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 0, 1, 0);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_e(JNIEnv *env, jclass type, jstring pass) {

    AES_KEY aes_key;
    AES_set_encrypt_key(key, 128, &aes_key);
    const char *str = env->GetStringUTFChars(pass, 0);
    int len = strlen(str);
    int paddingLen = 16 - len % 16;
    int cryptLen = len + paddingLen;

    std::vector<unsigned char> out(cryptLen);
    std::vector<char> in(cryptLen);

    memcpy(in.data(), str, len);
    memset(in.data() + len, paddingLen, paddingLen);

    for (int i = 0; i < cryptLen / 16; ++i) {
        AES_encrypt(reinterpret_cast<const unsigned char *>(in.data() + i * 16), out.data() + i * 16, &aes_key);
    }

    char base64[64];
    base64_encode((const char *) out.data(), cryptLen, base64);

    return env->NewStringUTF(base64);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_f(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 0, 1, 2);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_g(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 0, 2, 0);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_h(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 0, 2, 1);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_i(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 0, 2, 2);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_j(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 1, 0, 0);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_k(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 1, 0, 1);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_l(JNIEnv *env, jclass type) {
    int64_t ts = time(nullptr);

    AES_KEY aes_key;
    AES_set_encrypt_key(key, 128, &aes_key);

    unsigned char in[16];
    memset(in, 8, 16);
    *((int64_t *) in) = ts;

    unsigned char out[16];
    AES_encrypt(in, out, &aes_key);

    char base64[64];
    base64_encode((const char *) out, 16, base64);

    return env->NewStringUTF(base64);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_m(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 1, 1, 0);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_n(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 1, 1, 1);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_o(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 1, 1, 2);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_p(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 1, 2, 0);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_q(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 1, 2, 1);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_r(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 1, 2, 2);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_s(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 2, 0, 0);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_t(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 2, 0, 1);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_u(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 2, 0, 2);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_v(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 2, 1, 0);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_w(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 2, 1, 1);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_x(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 2, 1, 2);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_y(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 2, 2, 0);
    }
    return env->NewStringUTF(device_id.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_linken_common_a_z(JNIEnv *env, jclass type) {
    if (device_id.empty()) {
        jobject context = getApplication(env);
        check_signature(env, context);
        device_id = step0(env, context, 2, 2, 1);
    }
    return env->NewStringUTF(device_id.c_str());
}
