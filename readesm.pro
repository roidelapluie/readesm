# This file is just used for lupdate and lrelease
# to build the exectutable, use cmake


TEMPLATE = app
TRANSLATIONS = readesm_de.ts readesm_es.ts readesm_hu.ts
SOURCES += *.cpp gui/*.cpp fileformat/*.cpp fileformat/VuBlocks/*.cpp fileformat/reporter/*.cpp fileformat/DataTypes/*.cpp fileformat/CardBlocks/*.cpp
HEADERS += *.h gui/*.h fileformat/*.h fileformat/vuBlocks/*.h fileformat/reporter/*.h fileformat/dataTypes/*.h fileformat/cardBlocks/*.h
CODECFORTR = UTF-8
CODECFORSRC = UTF-8
