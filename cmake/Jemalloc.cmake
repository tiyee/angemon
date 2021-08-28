# Download and build Jemalloc

set(JEMALLOC_VERSION 5.2.1)
set(JEMALLOC_NAME jemalloc-${JEMALLOC_VERSION})
set(JEMALLOC_TAR_PATH ${DEP_ROOT_DIR}/${JEMALLOC_NAME}.tar.bz2)

if(NOT EXISTS ${JEMALLOC_TAR_PATH})
    message(STATUS "Downloading ${JEMALLOC_NAME}...")
    file(DOWNLOAD https://github.com/jemalloc/jemalloc/releases/download/${JEMALLOC_VERSION}/${JEMALLOC_NAME}.tar.bz2
            ${JEMALLOC_TAR_PATH} )
endif()

if(NOT EXISTS ${DEP_ROOT_DIR}/${JEMALLOC_NAME})
    message(STATUS "Extracting jemalloc...")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${JEMALLOC_TAR_PATH} WORKING_DIRECTORY ${DEP_ROOT_DIR})
endif()

if(NOT EXISTS ${DEP_ROOT_DIR}/${JEMALLOC_NAME}/Makefile)
    message("Configuring jemalloc locally...")
    # Builds with "--with-jemalloc-prefix=je_" on OSX
    execute_process(COMMAND autogen.sh)
    execute_process(COMMAND  ./configure WORKING_DIRECTORY ${DEP_ROOT_DIR}/${JEMALLOC_NAME} RESULT_VARIABLE JEMALLOC_CONFIGURE)
    if(NOT JEMALLOC_CONFIGURE EQUAL 0)
        message(FATAL_ERROR "${JEMALLOC_NAME} configure failed!")
        message("${RESULT_VARIABLE}")
    endif()
endif()

if(NOT EXISTS ${DEP_ROOT_DIR}/${JEMALLOC_NAME}/lib/libjemalloc.a)
    message("Building jemalloc locally...")
    execute_process(COMMAND make "build_lib_static" WORKING_DIRECTORY ${DEP_ROOT_DIR}/${JEMALLOC_NAME})
    if(NOT EXISTS ${DEP_ROOT_DIR}/${JEMALLOC_NAME}/lib/libjemalloc.a)
        message(FATAL_ERROR "${JEMALLOC_NAME} build failed!")
    endif()
endif()


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
