//
// Created by TrifingZW on 24-12-30.
//

#pragma once

#include <android/log.h>
#include <sstream>

/*!
 * 使用此方法将字符串输出到 logcat。请注意，您应使用 std::endl 来提交该行。
 *
 * 例如：
 *  aout << "Hello World" << std::endl;
 */
extern std::ostream aout;

/*!
 * 使用此类创建一个写入 logcat 的输出流。默认情况下，定义了一个全局的输出流 @a aout。
 */
class AndroidOut : public std::stringbuf
{
public:
    /*!
     * 创建一个新的 logcat 输出流
     * @param kLogTag 输出的日志标签
     */
    explicit AndroidOut(const char* kLogTag) : logTag_(kLogTag) {}

protected:
    int sync() override
    {
        __android_log_print(ANDROID_LOG_DEBUG, logTag_, "%s", str().c_str());
        str("");
        return 0;
    }

private:
    const char* logTag_;
};
