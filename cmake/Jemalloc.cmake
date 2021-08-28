# Download and build Jemalloc

set(JEMALLOC_VERSION 5.2.1)
set(JEMALLOC_NAME jemalloc-${JEMALLOC_VERSION})
set(JEMALLOC_TAR_PATH ${DEP_ROOT_DIR}/${JEMALLOC_NAME}.tar.bz2)
set(JEMALLOC_TAR_EXPECT_MD5 "3d41fbf006e6ebffd489bdb304d009ae")
if (NOT EXISTS ${JEMALLOC_TAR_PATH})
    message(STATUS "Downloading ${JEMALLOC_NAME}...")
    file(DOWNLOAD https://github.com/jemalloc/jemalloc/releases/download/${JEMALLOC_VERSION}/${JEMALLOC_NAME}.tar.bz2
            ${JEMALLOC_TAR_PATH} EXPECTED_MD5 ${JEMALLOC_TAR_EXPECT_MD5} )
endif ()
file(MD5 ${JEMALLOC_TAR_PATH} JEMALLOC_TAR_MD5)
message(STATUS "${JEMALLOC_NAME} md5 is: " ${JEMALLOC_TAR_MD5})
if (NOT JEMALLOC_TAR_EXPECT_MD5 STREQUAL ${JEMALLOC_TAR_MD5})
    execute_process(COMMAND ${CMAKE_COMMAND} -E rm ${JEMALLOC_TAR_PATH} WORKING_DIRECTORY ${DEP_ROOT_DIR})
    message(FATAL_ERROR "File ${JEMALLOC_NAME} md5 is not Identify, Download again")
endif ()
if (NOT EXISTS ${DEP_ROOT_DIR}/${JEMALLOC_NAME})
    message(STATUS "Extracting jemalloc...")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${JEMALLOC_TAR_PATH} WORKING_DIRECTORY ${DEP_ROOT_DIR})
endif ()

if (NOT EXISTS ${DEP_ROOT_DIR}/${JEMALLOC_NAME}/Makefile)
    message("Configuring jemalloc locally...")
    # Because the default setting "--with-jemalloc-prefix=je_" on OSX ,so we set it as unitized；
    execute_process(COMMAND autogen.sh)
    execute_process(COMMAND ./configure "--with-jemalloc-prefix=je_" WORKING_DIRECTORY ${DEP_ROOT_DIR}/${JEMALLOC_NAME} RESULT_VARIABLE JEMALLOC_CONFIGURE)
    if (NOT JEMALLOC_CONFIGURE EQUAL 0)
        message(FATAL_ERROR "${JEMALLOC_NAME} configure failed!")
        message("${RESULT_VARIABLE}")
    endif ()
endif ()

if (NOT EXISTS ${DEP_ROOT_DIR}/${JEMALLOC_NAME}/lib/libjemalloc.a)
    message("Building jemalloc locally...")
    execute_process(COMMAND make "build_lib_static" WORKING_DIRECTORY ${DEP_ROOT_DIR}/${JEMALLOC_NAME})
    if (NOT EXISTS ${DEP_ROOT_DIR}/${JEMALLOC_NAME}/lib/libjemalloc.a)
        message(FATAL_ERROR "${JEMALLOC_NAME} build failed!")
    endif ()
endif ()


#
#include(ExternalProject)
#
#set(JEMALLOC_VERSION 5.2.1)
#set(JEMALLOC_NAME jemalloc-${JEMALLOC_VERSION})
#
#
#set(JEMALLOC_ROOT          ${DEP_ROOT_DIR}/xxx/${JEMALLOC_NAME})
#
#
#set(JEMALLOC_URL           https://github.com/jemalloc/jemalloc/archive/${JEMALLOC_VERSION}.zip)
#set(JEMALLOC_CONFIGURE     mkdir -p ${JEMALLOC_ROOT}/src/snappy && cd ${JEMALLOC_ROOT}/src/snappy && cmake -D CMAKE_INSTALL_PREFIX=${JEMALLOC_ROOT} .)
#
#
#ExternalProject_Add(snappy
#        URL                   ${JEMALLOC_URL}
#        DOWNLOAD_DIR         ${JEMALLOC_ROOT}
#        DOWNLOAD_NAME         ${JEMALLOC_NAME}.zip
#        SOURCE_DIR           ${JEMALLOC_ROOT}/src
#        URL_MD5               4448f59c2716efe94018054961ae12ed
#        PREFIX                ${JEMALLOC_ROOT}
#        CONFIGURE_COMMAND     autogen.sh
#        BUILD_COMMAND         make
#        INSTALL_COMMAND       make install
#        )
