project(sqlite LANGUAGES C)

add_library(sqlite3
 sqlite3.c
 sqlite3.h
 sqlite3ext.h
)

target_link_libraries(sqlite3 ${CMAKE_THREAD_LIBS_INIT})

target_compile_options(sqlite3 PRIVATE "-DSQLITE_OMIT_LOAD_EXTENSION" "-DSQLITE_MAX_WORKER_THREADS=0" "-DSQLITE_ENABLE_FTS5")

target_include_directories(sqlite3 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
