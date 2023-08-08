
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
            const std::string& file,
            long line,
            const std::string& functionName,
            const std::string& reason)
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