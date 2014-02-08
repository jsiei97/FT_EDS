CONFIG += qtestlib debug
QT     -= gui
TEMPLATE = app
TARGET =
DEFINES += private=public
DEFINES += protected=public
DEFINES += EEPROM_MAX_SIZE=100

# Test code
DEPENDPATH += .
INCLUDEPATH += .
SOURCES += Test_FT_EDS.cpp

# Code to test
DEPENDPATH += ../../EDS/
INCLUDEPATH += ../../EDS/
SOURCES += FT_EDS.cpp HexDump.cpp

# Stubs
DEPENDPATH += stub/
INCLUDEPATH += stub/
SOURCES += EEPROM.cpp

