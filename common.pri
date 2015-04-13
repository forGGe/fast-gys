#-------------------------------------------------
#
# This files defines common options for all
# subprojects
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -Werror -Wpedantic -Wall -Wextra -Wformat
CONFIG += c++11 console

equals(QMAKE_CXX, clang++) {
    QMAKE_CXXFLAGS += -Wdocumentation
}


equals(QMAKE_CXX, g++) {
    # HACK: on Windows warning emits even in system headers
    # this will stay until any better workaround will be found
   !win32 {
        QMAKE_CXXFLAGS += -Weffc++
   }
}

