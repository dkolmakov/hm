
add_subdirectory(sqlitecpp)
target_compile_options(sqlite3 PRIVATE "-DSQLITE_MAX_WORKER_THREADS=0" "-DSQLITE_ENABLE_FTS5")

add_library(SQLiteLiteIncludes INTERFACE)
target_include_directories(SQLiteLiteIncludes INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/sqlitecpp/include")

add_subdirectory(apathy)
add_subdirectory(clipp)
