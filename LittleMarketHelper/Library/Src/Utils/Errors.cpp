
#include "Utils/Errors.h"
#include "Utils/Logger.h"


namespace lmh {

    // TODO2: this is not nice, figure out what to do
    const char* Error::what() const noexcept
    {
        // Log error before returning
        LMH_ERROR(message_);
        return message_.c_str();
    }

}