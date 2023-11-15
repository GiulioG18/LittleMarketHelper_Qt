
// ========================================================================
//		Errors handler
// ========================================================================

#pragma once

#include <exception>
#include <sstream>
#include <string>
#include <boost/current_function.hpp>


namespace lmh {

    class Error : public std::exception
    {
    public:

        Error(
            std::string_view file,
            int64_t line,
            std::string_view functionName,
            std::string_view reason)
        {
            std::ostringstream msg;
            msg << "FILE:       " << file << "(" << line << ")" << std::endl;
            msg << "FUNCTION:   " << functionName << std::endl;
            msg << "ERROR:      " << reason;

            message_ = msg.str();
        }

        const char* what() const noexcept override;

    private:

        std::string message_;
    };

}