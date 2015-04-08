#-------------------------------------------------
#
# This files defines common options for all
# subprojects
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -Werror -Wpedantic -Wall -Wextra -Wformat
CONFIG += c++14 console

equals(QMAKE_CXX, clang++) {
    QMAKE_CXXFLAGS += -Wdocumentation
}

equals(QMAKE_CXX, g++) {
    QMAKE_CXXFLAGS += -Weffc++
}
