project(readesm)
cmake_minimum_required(VERSION 2.8)

if(WIN32)
	#set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
	#set(CMAKE_EXE_LINKER_FLAGS=-static)
	set(GUI_TYPE WIN32)
endif()
if(MAC)
	set(GUI_TYPE MACOSX_BUNDLE)
endif()

set(CMAKE_CXX_FLAGS "-Wall" )
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_DEBUG "-Werror")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE release)
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(Qt4 COMPONENTS QtGui QtWebKit REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#if this is a release, look into the release file to get release version
include(currentRelease.cmake OPTIONAL)

#*** Make Version known to program ***
#is this in version control? take version control revision
find_package(Subversion)
find_file(Subversion_IS_WC .svn ${CMAKE_SOURCE_DIR} NO_CMAKE_PATH)
if(Subversion_FOUND AND Subversion_IS_WC)
	Subversion_WC_INFO(${CMAKE_SOURCE_DIR} ER)
	set(VERSION_STRING "svn${ER_WC_REVISION}")
	set(VERSION_DATE ${ER_WC_LAST_CHANGED_DATE})
endif()

#*** check for crypto libraries and enable them if found ***
find_package(gmp)
find_package(gcrypt)
if(GMP_FOUND AND GCRYPT_FOUND)
	message("Found gcrypt and gmp. Will use cryptographic checks.")
	set(LIBS ${LIBS} ${GCRYPT_LIBRARIES} ${GMP_LIBRARIES})
	set(HAVE_CRYPTO 1)
else()
	message("Did not find both gmp and gcrypt. Disabling cryptographic checks.")
endif()

#*** configuration file ***
configure_file(${CMAKE_SOURCE_DIR}/config.h.in ${CMAKE_BINARY_DIR}/config.h)

#*** translation ***
file (GLOB TRANSLATIONS_FILES "ressources/translations/*.ts")
qt4_add_translation(QM_FILES ${readesm_SRCS} ${TRANSLATIONS_FILES})
add_custom_target(translations_target DEPENDS ${QM_FILES})

#*** Actual part needed to build everything ***
include_directories(${QT_INCLUDES} ${CMAKE_BINARY_DIR})

file(GLOB gui_CPP "gui/*.cpp")
file(GLOB fileformat_CPP "fileformat/*.cpp" "fileformat/*/*.cpp")
file(GLOB gui_H "gui/*.h")
file(GLOB fileformat_H "fileformat/*.h" "fileformat/*/*.h")
file(GLOB main "*.cpp" "*.h")
set(readesm_SRCS ${gui_CPP} ${fileformat_CPP} ${gui_H} ${fileformat_H} ${main})
set(readesm_RCCS readesm.qrc)

qt4_add_resources(readesm_RCC_SRCS ${readesm_RCCS})

qt4_automoc(${readesm_SRCS})

add_executable(readesm ${GUI_TYPE} ${readesm_SRCS} ${readesm_RCC_SRCS} ${QM_FILES} readesm.rc)
target_link_libraries(readesm ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTWEBKIT_LIBRARY} ${LIBS})


#*** .desktop file, icon and mime type ***
if(UNIX)
	install(FILES ressources/linux-integration/readesm.desktop DESTINATION share/applications )
	install(FILES ressources/icons/readesm.png ressources/icons/readesm.svg DESTINATION share/pixmaps )
	install(FILES ressources/linux-integration/x-esm.xml DESTINATION share/mime/packages )
endif()

#*** translation files ***
if(WIN32)
	install(FILES ${QM_FILES} DESTINATION "translations")
else()
	install(FILES ${QM_FILES} DESTINATION "share/readesm/translations")
endif()

#documentation
#*** Install binary ***
install(TARGETS readesm RUNTIME DESTINATION "bin")

#*** CPACK  packaging ***
set(CPACK_PACKAGE_VERSION ${VERSION_STRING})

set(CPACK_SOURCE_IGNORE_FILES
	"~$"
	"^${PROJECT_SOURCE_DIR}.*/.svn/"
	"^${PROJECT_SOURCE_DIR}/build/"
	"^${PROJECT_SOURCE_DIR}/dlls/"
	"^${PROJECT_SOURCE_DIR}/data/"
)

set(CPACK_SOURCE_GENERATOR TGZ;TBZ2;ZIP)
set(CPACK_GENERATOR TBZ2)
set(CPACK_PACKAGE_VENDOR "Andreas Goelzer <K_2005@goelzer.de>")
set(CPACK_NSIS_CONTACT ${CPACK_PACKAGE_VENDOR})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
set(CPACK_NSIS_MUI_FINISHPAGE_RUN "readesm")
set(CPACK_CREATE_DESKTOP_LINKS "readesm")
set(WEBSITE "http://readesm.sourceforge.net/")
#packaging
if(UNIX)
endif()
if(APPLE)
	set(PROGNAME Readesm )
	set(MACOSX_BUNDLE_ICON_FILE "${CMAKE_SOURCE_DIR}/ressources/icons/readesm.icns")
	set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${VERSION_STRING})
	set(MACOSX_BUNDLE_VERSION ${VERSION_STRING})
	set(MACOSX_BUNDLE_LONG_VERSION_STRING "Version ${VERSION_STRING}")
endif()
if(WIN32)
	set(CPACK_PACKAGE_EXECUTABLES "readesm" "readesm")
	set(CPACK_NSIS_URL_INFO_ABOUT "${WEBSITE}")
	set(CPACK_NSIS_HELP_LINK "${WEBSITE}/help.html")

	set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/ressources/icons/readesm.ico")
	set(CPACK_NSIS_MUI_UNIICON ${CPACK_NSIS_MUI_ICON})
	set(CPACK_NSIS_INSTALLED_ICON_NAME bin\\\\readesm.exe)
	# File types association:
	set(FILETYPE_DESCRIPTION "Digitaltacho Data")
	set(EXTENSIONS ddd esm add tgd)
	set(CPACK_NSIS_DEFINES "!include ${PROJECT_SOURCE_DIR}\\\\ressources\\\\windows-integration\\\\FileAssociation.nsh")
	foreach(EXTENSION ${EXTENSIONS})
		set(NSIS_INSTALL_EXT "${NSIS_INSTALL_EXT}\n\\\${registerExtension} \\\"\\\$INSTDIR\\\\bin\\\\readesm.exe\\\" \\\".${EXTENSION}\\\" \\\"${FILETYPE_DESCRIPTION}\\\"")
		set(NSIS_UNINSTALL_EXT "${NSIS_UNINSTALL_EXT}\n\\\${unregisterExtension} \\\".${EXTENSION}\\\" \\\"${FILETYPE_DESCRIPTION}\\\"")
	endforeach()
	set(NSIS_INSTALL_EXT "System::Call 'shell32.dll::SHChangeNotify(i, i, i, i) v (0x08000000, 0, 0, 0)'")
	set(NSIS_UNINSTALL_EXT "System::Call 'shell32.dll::SHChangeNotify(i, i, i, i) v (0x08000000, 0, 0, 0)'")
	set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS ${NSIS_INSTALL_EXT})
	set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS ${NSIS_UNINSTALL_EXT})
	set(CPACK_GENERATOR NSIS;ZIP)
	file(GLOB readesm_needed_DLL "${CMAKE_SOURCE_DIR}/dlls/*.dll")
	install(FILES homepage/help.html COPYING README DESTINATION doc)
	install(FILES ${readesm_needed_DLL} DESTINATION bin)
endif()


include(CPack)
