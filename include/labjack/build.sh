clear
mkdir -p targets;
rm -f targets/cbot;
DIR_ENET="third_party/EnetCsharp"

# g++ -std=c++11 -I./third_party/imgui -I./third_party/imgui/backends -g -Wall -Wformat `pkg-config --cflags glfw3` -c -o targets/imgui.o ./third_party/imgui/imgui.cpp
# g++ -std=c++11 -I./third_party/imgui -I./third_party/imgui/backends -g -Wall -Wformat `pkg-config --cflags glfw3` -c -o targets/imgui_demo.o ./third_party/imgui/imgui_demo.cpp
# g++ -std=c++11 -I./third_party/imgui -I./third_party/imgui/backends -g -Wall -Wformat `pkg-config --cflags glfw3` -c -o targets/imgui_draw.o ./third_party/imgui/imgui_draw.cpp
# g++ -std=c++11 -I./third_party/imgui -I./third_party/imgui/backends -g -Wall -Wformat `pkg-config --cflags glfw3` -c -o targets/imgui_tables.o ./third_party/imgui/imgui_tables.cpp
# g++ -std=c++11 -I./third_party/imgui -I./third_party/imgui/backends -g -Wall -Wformat `pkg-config --cflags glfw3` -c -o targets/imgui_widgets.o ./third_party/imgui/imgui_widgets.cpp
# g++ -std=c++11 -I./third_party/imgui -I./third_party/imgui/backends -g -Wall -Wformat `pkg-config --cflags glfw3` -c -o targets/imgui_impl_glfw.o ./third_party/imgui/backends/imgui_impl_glfw.cpp
# g++ -std=c++11 -I./third_party/imgui -I./third_party/imgui/backends -g -Wall -Wformat `pkg-config --cflags glfw3` -c -o targets/imgui_impl_opengl3.o ./third_party/imgui/backends/imgui_impl_opengl3.cpp

g++ -Ofast -ffast-math -std=c++17 -w -fcompare-debug-second \
    -o targets/*.o \
    -o targets/cbot src/cbot.cpp include/glad/gl.cpp src/FrameBuffer.cpp src/robot_gripper.cpp src/robot_control.cpp \
    -I include \
    -lrtde \
    -I$DIR_ENET \
    -I./third_party/IconFontCppHeaders \
    -I./third_party/flatbuffers/include \
    -L$HOME/build/mujoco-2.3.7/lib \
    -I$HOME/build/mujoco-2.3.7/include/mujoco \
    -I$HOME/build/mujoco-2.3.7/include/ \
    -lmujoco \
    -lenet \
    -lLabJackM \
    -I/usr/local/include/ -I./third_party/imgui -I./third_party/imgui/backends -lGLEW -lGLU -lGL `pkg-config --static --libs glfw3`

./targets/cbot