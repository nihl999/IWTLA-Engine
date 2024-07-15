g++ -c imgui_draw.cpp imgui_impl_glfw.cpp imgui_impl_opengl3.cpp imgui_tables.cpp imgui_widgets.cpp imgui.cpp -I../../../include/imgui -I ../../../include/imgui/backends -I../../../include/ -luser32 -lgdi32 -lopengl32 -L../../ -lglfw3

ar rvs imguiglgl imgui_impl_glfw.o imgui_impl_opengl3.o

ar rvs imgui.a imgui.o imgui_draw.o imgui_tables.o imgui_widgets.o 