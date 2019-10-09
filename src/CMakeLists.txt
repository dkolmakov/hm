
add_executable(hm-db 
    main.cpp 
    database.hpp 
    history.hpp)

target_link_libraries(hm-db sqlite3 apathy)

install(TARGETS hm-db DESTINATION ${CMAKE_INSTALL_BINDIR})


