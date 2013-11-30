CONFIG += qtestlib debug
TEMPLATE = app
TARGET =
DEFINES += private=public

# Test code
DEPENDPATH += .
INCLUDEPATH += .
SOURCES += Test_IntegerExtra.cpp

# Code to test
DEPENDPATH += ../../EDS/
INCLUDEPATH += ../../EDS/
SOURCES += IntegerExtra.cpp


