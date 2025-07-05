#ifndef EUNOMIA_OUTPUT_MANAGER
#define EUNOMIA_OUTPUT_MANAGER

#include <iomanip>
#include <iosfwd>                                               // for wostream, ostream
#include <string>                                               // for to_wstring
#include "Utility.h"                                            // for OutChr_t, OutStr_t, Stream_t

class EunomiaStream;

extern EunomiaStream& eout;                                     // not really a global

/*
    The EunomiaStream class is an extremely simplified output stream that prints content to
    the standard output stream (i.e. the console) for the Eunomia application. The
    EunomiaStream is guaranteed to be able to print the widest characters needed by the program,
    and it performs OS-conditional compilation to ensure that the correct set-up is performed.
*/

class EunomiaStream {
    public:
        // [Singleton Accessor]
        // EFFECTS:  returns the Singleton instance of the EunomiaStream
        static EunomiaStream& getInstance();

        // [Width Accessor]
        std::streamsize width() const;

        // [Generic Stream Insertion Operator]
        template <typename OutType>
        EunomiaStream& operator<<(const OutType& content) {
            out << std::to_wstring(content);
            return *this;
        }

        // [Specialized Stream Insertion Operators]
        EunomiaStream& operator<<(const std::string& content);
        EunomiaStream& operator<<(const char* content);
        EunomiaStream& operator<<(char content);
        EunomiaStream& operator<<(const std::_Smanip<std::streamsize> manip);
        EunomiaStream& operator<<(std::ostream& (*)(std::ostream&));

    private:
        using Stream_t = std::wostream;

        Stream_t& out;

        // [Constructor]
        EunomiaStream();
};

#endif