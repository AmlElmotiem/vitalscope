include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

# Crow needs standalone Asio for its networking, but does not fetch it
# automatically. Fetch it ourselves and point Crow's own find_package(asio)
# at it via the ASIO_INCLUDE_DIR cache variable, set before Crow is added.
FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-30-2
)
FetchContent_Populate(asio)
set(ASIO_INCLUDE_DIR ${asio_SOURCE_DIR}/asio/include CACHE PATH "" FORCE)

set(CROW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CROW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG v1.2.0
)
FetchContent_MakeAvailable(crow)

add_executable(vitalscope_server main.cpp)
target_link_libraries(vitalscope_server PRIVATE
    vitalscope_lib
    nlohmann_json::nlohmann_json
    Crow::Crow
)

# Asio's Windows backend calls into the Winsock API (WSAStartup, etc.),
# which lives in these system import libraries, not in Crow or Asio itself.
if(WIN32)
    target_link_libraries(vitalscope_server PRIVATE ws2_32 mswsock)
endif()
