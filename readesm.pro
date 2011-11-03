# This file is just used for lupdate and lrelease
# to build the exectutable, use cmake


TEMPLATE = app
TRANSLATIONS = ressources/translations/readesm_de.ts ressources/translations/readesm_hu.ts ressources/translations/readesm_es.ts
SOURCES += *.cpp gui/*.cpp fileformat/*.cpp fileformat/VuBlocks/*.cpp fileformat/Reporter/*.cpp fileformat/DataTypes/*.cpp fileformat/CardBlocks/*.cpp fileformat/PictureGenerators/*.cpp 
HEADERS += *.h gui/*.h fileformat/*.h fileformat/VuBlocks/*.h fileformat/Reporter/*.h fileformat/DataTypes/*.h fileformat/CardBlocks/*.h fileformat/PictureGenerators/*.h
CODECFORTR = UTF-8
CODECFORSRC = UTF-8
