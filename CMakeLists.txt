project(readesm)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 COMPONENTS QtGui QtWebKit REQUIRED)

#find_package(libgmp)
#find_package(libgcrypto)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(readesm_SRCS readesm.cpp gui/mainWindow.cpp legacyWrapper.cpp)
qt4_automoc(${readesm_SRCS})
add_executable(readesm ${readesm_SRCS})
#add_executable(readesm-nogui readesm-nogui.cpp)

target_link_libraries(readesm ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTWEBKIT_LIBRARY} gmp gcrypt)
