
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/version.hpp)            
            
add_executable(hm-db 
    main.cpp 
    database.hpp 
    history.hpp
    configurator.hpp
    const_conf.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/version.hpp)

target_link_libraries(hm-db sqlite3 apathy clipp)

install(TARGETS hm-db DESTINATION ${CMAKE_INSTALL_BINDIR})


