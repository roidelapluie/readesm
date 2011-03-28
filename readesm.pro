# This file is just used for lupdate and lrelease
# to build the exectutable, use cmake


TEMPLATE = app
TRANSLATIONS = readesm_de.ts readesm_es.ts
SOURCES += *.cpp gui/*.cpp fileformat/*.cpp fileformat/vuBlocks/*.cpp fileformat/reporter/*.cpp
HEADERS += *.h gui/*.h fileformat/*.h fileformat/vuBlocks/*.h fileformat/reporter/*.h
CODECFORTR = UTF-8
CODECFORSRC = UTF-8
