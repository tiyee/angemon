include(ExternalProject)

set(JEMALLOC_ROOT          ${CMAKE_BINARY_DIR}/thirdparty/jemalloc)
set(JEMALLOC_LIB_DIR       ${JEMALLOC_ROOT}/lib)
set(JEMALLOC_INCLUDE_DIR   ${JEMALLOC_ROOT}/include)

set(JEMALLOC_URL           https://github.com/jemalloc/jemalloc/archive/5.2.1.zip)
set(JEMALLOC_CONFIGURE     cd ${JEMALLOC_ROOT}/src/JeMalloc && source ./autogen.sh  && ${JEMALLOC_ROOT}/src/JeMalloc/configure --prefix=${JEMALLOC_ROOT} )
set(JEMALLOC_MAKE          cd ${JEMALLOC_ROOT}/src/JeMalloc && make )
set(JEMALLOC_INSTALL       cd ${JEMALLOC_ROOT}/src/JeMalloc && make install )

ExternalProject_Add(JeMalloc
        URL                   ${JEMALLOC_URL}
        DOWNLOAD_NAME         jemalloc-5.2.1.zip
        URL_MD5               4448f59c2716efe94018054961ae12ed
        PREFIX                ${JEMALLOC_ROOT}
        CONFIGURE_COMMAND     ${JEMALLOC_CONFIGURE}
        BUILD_COMMAND         ${JEMALLOC_MAKE}
        INSTALL_COMMAND       ${JEMALLOC_INSTALL}
        )
