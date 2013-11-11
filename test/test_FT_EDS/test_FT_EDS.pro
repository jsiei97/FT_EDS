CONFIG += qtestlib debug
TEMPLATE = app
TARGET =
DEFINES += private=public

# Test code
DEPENDPATH += .
INCLUDEPATH += .
SOURCES += Test_FT_EDS.cpp

# Code to test
DEPENDPATH += ../../EDS/
INCLUDEPATH += ../../EDS/
SOURCES += FT_EDS.cpp

# Stubs
DEPENDPATH += stub/
INCLUDEPATH += stub/
SOURCES += EEPROM.cpp

