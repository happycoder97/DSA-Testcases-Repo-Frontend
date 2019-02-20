cmake_minimum_required(VERSION 3.13)
project("DSA Tescases Repo Frontend")
set(CMAKE_CXX_STANDARD 17)

add_executable(DSATestcasesRepo)
set_target_properties(DSATestcasesRepo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "dist/"
)
target_sources(DSATestcasesRepo PRIVATE "src/gui_main.cpp" "src/main.cpp")
target_sources(DSATestcasesRepo PRIVATE "src/login.cpp")
target_sources(DSATestcasesRepo PRIVATE "src/backend.cpp")
target_sources(DSATestcasesRepo PRIVATE "imgui/imgui.cpp" "imgui/imgui_draw.cpp" "imgui/imgui_widgets.cpp")
target_sources(DSATestcasesRepo PRIVATE "imgui/misc/cpp/imgui_stdlib.cpp")
target_sources(DSATestcasesRepo PRIVATE "imgui/examples/imgui_impl_glfw.cpp" "imgui/examples/imgui_impl_opengl2.cpp")
target_include_directories(DSATestcasesRepo PRIVATE "imgui/" "imgui/examples" "imgui/misc/cpp")
target_link_libraries(DSATestcasesRepo PRIVATE "glfw" "GL")

add_subdirectory("cpr")
include_directories(${CPR_INCLUDE_DIRS})
target_link_libraries(DSATestcasesRepo PRIVATE ${CPR_LIBRARIES})

find_package(Threads REQUIRED)
add_subdirectory("asyncplusplus")
add_dependencies(DSATestcasesRepo Async++)
target_link_libraries(DSATestcasesRepo PRIVATE Async++)

# ------------ JSON ------------------------------
# Typically you don't care so much for a third party library's tests to be
# run from your own project's code.
set(JSON_BuildTests OFF CACHE INTERNAL "")

# Don't use include(nlohmann_json/CMakeLists.txt) since that carries with it
# unintended consequences that will break the build.  It's generally
# discouraged (although not necessarily well documented as such) to use
# include(...) for pulling in other CMake projects anyways.
add_subdirectory(json)
target_link_libraries(DSATestcasesRepo PRIVATE nlohmann_json::nlohmann_json)