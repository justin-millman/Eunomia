#include "EunomiaStream.h"                                      // for EunomiaStream
#include <iomanip>                                              // for _Smanip
#include <iostream>                                             // for wcout, ostream, streamsize, endl
#include <locale>                                               // for locale
#include <string>                                               // for string
#ifdef _WIN32                                                   // Windows-specific file headers for
    #include <fcntl.h>                                          //   Unicode output management
    #include <io.h>
#endif

using std::wcout; using std::streamsize; using std::ostream;
using std::endl; using std::_Smanip;
using std::locale;
using std::string;

EunomiaStream& eout = EunomiaStream::getInstance();

// constructor
EunomiaStream::EunomiaStream()
    : out{ wcout } {

    #ifdef _WIN32
        _setmode(_fileno(stdout), _O_U16TEXT);
    #else
        locale::global(locale{ "en_US.utf8" });
        wcout.imbue(locale{});
    #endif
}

// singleton accessor
EunomiaStream& EunomiaStream::getInstance() {
    static EunomiaStream inst{};
    return inst;
}

// width accessor
streamsize EunomiaStream::width() const {
    return out.width();
}

// regular string printer
EunomiaStream& EunomiaStream::operator<<(const string& content) {
    auto end = content.cend();
    for (auto iter = content.cbegin(); iter != end; ++iter) {
        out << *iter;
    }
    return *this;
}

// C-string printer
EunomiaStream& EunomiaStream::operator<<(const char* content) {
    while (*content) {
        out << *content++;
    }
    return *this;
}

// regular character printer
EunomiaStream& EunomiaStream::operator<<(char content) {
    out << content;
    return *this;
}

// manipulator printer
EunomiaStream& EunomiaStream::operator<<(const _Smanip<streamsize> manip) {
    out << manip;
    return *this;
}

// endl printer
EunomiaStream& EunomiaStream::operator<<(ostream& (*)(ostream&)) {
    out << endl;
    return *this;
}