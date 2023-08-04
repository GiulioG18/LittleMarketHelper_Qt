
#include "Utils/Errors.h"
#include "Utils/Logger.h"


namespace lmh {

    const char* Error::what() const noexcept
    {
        // Log error before returning
        ERROR(message_);
        return message_.c_str();
    }

}