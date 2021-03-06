#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "curl/curl.h"

using namespace std;

#ifdef _WIN32
string UTF8ToGBK(const string& strUTF8) {
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
    unsigned short * wszGBK = new unsigned short[len + 1];
    memset(wszGBK, 0, len * 2 + 2);

    MultiByteToWideChar(CP_UTF8, 0, (char*)strUTF8.c_str(), -1, (wchar_t*)wszGBK, len);

    len = WideCharToMultiByte(CP_ACP, 0, (wchar_t*)wszGBK, -1, NULL, 0, NULL, NULL);

    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, (wchar_t*)wszGBK, -1, szGBK, len, NULL, NULL);

    string strTemp(szGBK);
    delete[] szGBK;
    delete[] wszGBK;

    return strTemp;
}

#endif

/**
 * 一句话识别RESTful API HTTP请求的响应回调函数
 * 识别结果为JSON格式的字符串
 */
size_t responseCallback(void* ptr, size_t size, size_t nmemb, void* userData) {
    string* srResult = (string*)userData;

    size_t len = size * nmemb;
    char *pBuf = (char*)ptr;
    string response = string(pBuf, pBuf + len);
#ifdef _WIN32
    response = UTF8ToGBK(response);
#endif

    *srResult += response;
    cout << "current result: " << response << endl;
    cout << "total result: " << *srResult << endl;

    return len;
}

int sendAsrRequest(const char* request, const char* token, const char* fileName, string* srResult) {
    CURL* curl = NULL;
    CURLcode res;

    /**
    * 读取音频文件
    */
    ifstream fs;
    fs.open(fileName, ios::out | ios::binary);
    if (!fs.is_open()) {
        cerr << "The audio file is not exist!" << endl; 
        return -1;
    }
    stringstream buffer;
    buffer << fs.rdbuf();
    string audioData(buffer.str());

    curl = curl_easy_init();
    if (curl == NULL) {
        return -1;
    }

    /**
    * 设置HTTP请求行
    */
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_URL, request);

    /**
    * 设置HTTP请求头部
    */
    struct curl_slist* headers = NULL;
    // token
    string X_NLS_Token = "X-NLS-Token:";
    X_NLS_Token += token;
    headers = curl_slist_append(headers, X_NLS_Token.c_str());
    // Content-Type
    headers = curl_slist_append(headers, "Content-Type:application/octet-stream");
    // Content-Length
    string content_Length = "Content-Length:";
    ostringstream oss;
    oss << content_Length << audioData.length();
    content_Length = oss.str();
    headers = curl_slist_append(headers, content_Length.c_str());

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    /**
    * 设置HTTP请求数据
    */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, audioData.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, audioData.length());

    /**
    * 设置HTTP请求的响应回调函数
    */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, responseCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, srResult);

    /**
    * 发送HTTP请求
    */
    res = curl_easy_perform(curl);

    // 释放资源
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        cerr << "curl_easy_perform failed: " << curl_easy_strerror(res) << endl;
        return -1;
    }

    return 0;
}

int process(const char* request, const char* token, const char* fileName) {
    // 全局只初始化一次
    curl_global_init(CURL_GLOBAL_ALL);

    string srResult = "";
    int ret = sendAsrRequest(request, token, fileName, &srResult);

    curl_global_cleanup();

    return ret;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "params is not valid. Usage: ./demo your_token your_appkey" << endl;
        return -1;
    }

    string token = argv[1];
    string appKey = argv[2];

    string url = "http://nls-gateway.cn-shanghai.aliyuncs.com/stream/v1/asr";
    string format = "pcm";
    int sampleRate = 16000;
    bool enablePunctuationPrediction = true;
    bool enableInverseTextNormalization = true;
    bool enableVoiceDetection = false;
    string fileName = "sample.pcm";

    /**
    * 设置RESTful请求参数
    */
    ostringstream oss;
    oss << url;
    oss << "?appkey=" << appKey;
    oss << "&format=" << format;
    oss << "&sample_rate=" << sampleRate;
    if (enablePunctuationPrediction) {
        oss << "&enable_punctuation_prediction=" << "true";
    }
    if (enableInverseTextNormalization) {
        oss << "&enable_inverse_text_normalization=" << "true";
    }
    if (enableVoiceDetection) {
        oss << "&enable_voice_detection=" << "true";
    }

    string request = oss.str();
    cout << "request: " << request << endl;

    process(request.c_str(), token.c_str(), fileName.c_str());

    return 0;
}

