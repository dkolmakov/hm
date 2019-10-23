
set(VERSION "#define HM_VERSION=\"${VERSION}\";\n")
file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/version.hpp "${VERSION}")
