cmake_minimum_required(VERSION 3.13)
project("DSA Tescases Repo Frontend")

add_executable(DSATestcasesRepo)
set_target_properties(DSATestcasesRepo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "dist/linux/"
)
target_sources(DSATestcasesRepo PRIVATE "gui_main.cpp" "main.cpp")
target_sources(DSATestcasesRepo PRIVATE "imgui/imgui.cpp" "imgui/imgui_draw.cpp" "imgui/imgui_widgets.cpp")
target_sources(DSATestcasesRepo PRIVATE "imgui/misc/cpp/imgui_stdlib.cpp")
target_sources(DSATestcasesRepo PRIVATE "imgui/examples/imgui_impl_glfw.cpp" "imgui/examples/imgui_impl_opengl2.cpp")
target_include_directories(DSATestcasesRepo PRIVATE "imgui/" "imgui/examples" "imgui/misc/cpp")
target_link_libraries(DSATestcasesRepo "glfw" "GL")