project(readesm)
cmake_minimum_required(VERSION 2.8)

if(WIN32)
	#set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
	#set(CMAKE_EXE_LINKER_FLAGS=-static)
	set(GUI_TYPE WIN32)
endif(WIN32)
if(MAC)
	set(GUI_TYPE MACOSX_BUNDLE)
endif()
set(CMAKE_BUILD_TYPE release)

find_package(Qt4 COMPONENTS QtGui QtWebKit REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#*** Make Version known to program ***
#is this in version control? take version control revision
find_package(Subversion)
find_file(Subversion_IS_WC .svn ${CMAKE_CURRENT_SOURCE_DIR} NO_CMAKE_PATH)
if(Subversion_FOUND AND Subversion_IS_WC)
	Subversion_WC_INFO(${CMAKE_CURRENT_SOURCE_DIR} ER)
	set(VERSION_STRING "svn${ER_WC_REVISION}")
	set(VERSION_DATE ${ER_WC_LAST_CHANGED_DATE})
endif()

#if this is a release, look into the release file to get release version
include(currentRelease.cmake OPTIONAL)

#*** check for crypto libraries and enable them if found ***
find_package(gmp)
find_package(gcrypt)
if(GMP_FOUND AND GCRYPT_FOUND)
	message("Found gcrypt and gmp. Will use cryptographic checks.")
	set(LIBS ${LIBS} ${GCRYPT_LIBRARIES} ${GMP_LIBRARIES})
else()
	message("Did not find both gmp and gcrypt. Disabling cryptographic checks.")
	set(HAVE_NO_CRYPTO 1)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

#*** translation ***
file (GLOB TRANSLATIONS_FILES "*.ts")
set_source_files_properties(${TRANSLATIONS_FILES} PROPERTIES OUTPUT_LOCATION ${CMAKE_CURRENT_SOURCE_DIR})
#qt4_create_translation(QM_FILES ${readesm_SRCS} ${TRANSLATIONS_FILES})
qt4_add_translation(QM_FILES ${readesm_SRCS} ${TRANSLATIONS_FILES})
add_custom_target(translations_target DEPENDS ${QM_FILES})

#*** Actual part needed to build everything ***
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB gui_CPP "gui/*.cpp")
file(GLOB fileformat_CPP "fileformat/*.cpp" "fileformat/*/*.cpp")
file(GLOB gui_H "gui/*.h")
file(GLOB fileformat_H "fileformat/*.h" "fileformat/*/*.h")

set(readesm_SRCS ${gui_CPP} ${fileformat_CPP} ${gui_H} ${fileformat_H} readesm.cpp )
set(readesm_RCCS readesm.qrc)

qt4_add_resources(readesm_RCC_SRCS ${readesm_RCCS})

qt4_automoc(${readesm_SRCS})
add_definitions(-Wall)

add_executable(readesm ${GUI_TYPE} ${readesm_SRCS} ${readesm_RCC_SRCS} ${QM_FILES})

target_link_libraries(readesm ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTWEBKIT_LIBRARY} ${LIBS})

#*** .desktop file and mime type ***
find_program(XDG-MIME_EXECUTABLE xdg-mime)
find_program(XDG-DESKTOP-MENU_EXECUTABLE xdg-desktop-menu)
message("xdg-mime ${XDG-MIME_EXECUTABLE} ")
message("xdg-desktop-menu ${XDG-DESKTOP-MENU_EXECUTABLE} ")
if(XDG-MIME_EXECUTABLE AND XDG-DESKTOP-MENU_EXECUTABLE)
	install(CODE
		"
		execute_process(COMMAND ${XDG-MIME_EXECUTABLE} install --novendor esm.xml)
		execute_process(COMMAND ${XDG-DESKTOP-MENU_EXECUTABLE} install --novendor readesm.desktop)
		"
	)
else()
	message("Did not find tools to install the mimetype.")
endif()

#*** Install binary ***
install(TARGETS readesm RUNTIME DESTINATION bin)

#*** CPACK  packaging ***

set(CPACK_SOURCE_GENERATOR TGZ;TBZ2;ZIP)
set(CPACK_GENERATOR TBZ2)
set(CPACK_PACKAGE_VENDOR "Andreas Goelzer <K_2005@goelzer.de>")
set(CPACK_NSIS_CONTACT ${CPACK_PACKAGE_VENDOR})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
set(CPACK_NSIS_MUI_FINISHPAGE_RUN "readesm")
set(CPACK_CREATE_DESKTOP_LINKS "readesm")
#packaging
if(UNIX)
endif()
if(WIN32)
	set(CPACK_PACKAGE_EXECUTABLES "readesm" "readesm")
	set(CPACK_GENERATOR NSIS;ZIP)
	file(GLOB readesm_needed_DLL "${CMAKE_CURRENT_SOURCE_DIR}/../dlls/*.dll")
	install(FILES ${readesm_needed_DLL} DESTINATION bin)
endif()

set(CPACK_PACKAGE_VERSION ${VERSION_STRING})

set(CPACK_SOURCE_IGNORE_FILES
	"~$"
	"^${PROJECT_SOURCE_DIR}.*/.svn/"
	"^${PROJECT_SOURCE_DIR}/build/"
)

include(CPack)
