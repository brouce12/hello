/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_twirling_audio_Wakeup */

#ifndef _Included_com_twirling_audio_Wakeup
#define _Included_com_twirling_audio_Wakeup
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_twirling_audio_Wakeup
 * Method:    WakeupInit
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_twirling_audio_Wakeup_WakeupInit
  (JNIEnv *, jobject);

/*
 * Class:     com_twirling_audio_Wakeup
 * Method:    GetWakeupStatus
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_twirling_audio_Wakeup_GetWakeupStatus
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_twirling_audio_Wakeup
 * Method:    WakeupRun
 * Signature: (J[SI)V
 */
JNIEXPORT void JNICALL Java_com_twirling_audio_Wakeup_WakeupRun
  (JNIEnv *, jobject, jlong, jshortArray, jint);

/*
 * Class:     com_twirling_audio_Wakeup
 * Method:    WakeupRelease
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_twirling_audio_Wakeup_WakeupRelease
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif
