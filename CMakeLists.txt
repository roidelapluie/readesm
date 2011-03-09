project(readesm)
cmake_minimum_required(VERSION 2.8)
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
qt4_create_translation(QM_FILES ${readesm_SRCS} ${TRANSLATIONS_FILES})
add_custom_target(translations_target DEPENDS ${QM_FILES})

#*** Actual part needed to build everything ***
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/fileformat ${CMAKE_CURRENT_SOURCE_DIR}/fileformat/vuBlocks ${CMAKE_CURRENT_SOURCE_DIR}/fileformat/reporter)

file(GLOB gui_CPP "gui/*.cpp")
file(GLOB fileformat_CPP "fileformat/*.cpp" fileformat/*/*.cpp) 

set(readesm_SRCS ${gui_CPP} ${fileformat_CPP} readesm.cpp )
set(readesm_RCCS readesm.qrc)

qt4_add_resources(readesm_RCC_SRCS ${readesm_RCCS})

qt4_automoc(${readesm_SRCS})
add_definitions(-Wall -Os)

add_executable(readesm ${readesm_SRCS} ${readesm_RCC_SRCS} ${QM_FILES})

target_link_libraries(readesm ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTWEBKIT_LIBRARY} ${LIBS})

#*** Documentation Files ***


#*** Install files ***
install(TARGETS readesm RUNTIME DESTINATION bin)


#packaging
set(CPACK_PACKAGE_VERSION ${VERSION_STRING})
set(CPACK_GENERATOR TBZ2)
set(CPACK_SOURCE_GENERATOR TGZ;TBZ2)
set(CPACK_SOURCE_IGNORE_FILES
	"~$"
	"^${PROJECT_SOURCE_DIR}.*/.svn/"
	"^${PROJECT_SOURCE_DIR}/build/"
)

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libgcrypt11, libgmp3c2, libqtcore4, libqtgui4")
include(CPack)