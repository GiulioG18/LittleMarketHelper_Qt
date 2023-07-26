
#include "Errors.h"
#include "Logger.h"


namespace lmh {

    const char* Error::what() const noexcept
    {
        ERROR(message_);
        return message_.c_str();
    }

}