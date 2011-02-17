project(readesm)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 COMPONENTS QtGui QtWebKit REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(gmp)
find_package(gcrypt)

if(gmp_FOUND AND gcrypt_FOUND)
	message("Found gcrypt and gmp. Will use cryptographic checks.")
	set(LIBS ${LIBS} ${gmp_LIBRARY} ${gcrypt_LIBRARY})
else()
	message("Did not find both gmp and gcrypt. Disabling cryptographic checks.")
	add_definitions(-DHAVE_NO_CRYPTO)
endif()

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(readesm_SRCS readesm.cpp gui/mainWindow.cpp legacyWrapper.cpp)
qt4_automoc(${readesm_SRCS})

add_executable(readesm ${readesm_SRCS})
#add_executable(readesm-nogui readesm-nogui.cpp)

target_link_libraries(readesm ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTWEBKIT_LIBRARY} ${LIBS})
