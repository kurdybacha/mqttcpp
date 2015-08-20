#ifndef _GL_GLOBAL_H
#define _GL_GLOBAL_H

#include <memory>

#define GL_NAMESPACE mqttcpp
#define GL_BEGIN_NAMESPACE namespace GL_NAMESPACE {
#define GL_END_NAMESPACE }

#define GL_DECLARE_PRIVATE(Class) \
    std::unique_ptr<Class##Private> d; \
    friend class Class##Private;

#define GL_DISABLE_COPY(Class) \
    Class(const Class &) = delete;\
    Class &operator=(const Class &) = delete; \


#endif // _GL_GLOBAL_H

