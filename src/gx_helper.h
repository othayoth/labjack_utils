#ifndef GX_HELPER
#define GX_HELPER

#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include "glad/gl.h"
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "IconsForkAwesome.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "types.h"
#include "implot.h"


typedef struct gx_context
{
    u32 swap_interval;
    u32 width;
    u32 height;
    GLFWwindow *render_target;
    char *render_target_title;
    char *glsl_version;
} gx_context;


static void gx_glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


void gx_init(gx_context *context, GLFWwindow *render_target)
{
    context->render_target = render_target;
    glfwMakeContextCurrent(render_target);
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD\n");
		exit(EXIT_FAILURE);
	}
    glfwSwapInterval(1); // Enable vsync
}

GLFWwindow *gx_glfw_init_render_target(u32 marjor_version, u32 minor_version, u32 width, u32 height, const char *title, char *glsl_version)
{
    // Setup window
    glfwSetErrorCallback(gx_glfw_error_callback);
    if (!glfwInit())
    {
        printf("Could not initialize glfw!");
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // local_glsl_version = "#version 330";
    strcpy(glsl_version, "#version 130");

    // Create window with graphics context
    GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window)
    {
        printf("Could not initialize window!");
        glfwTerminate();
        exit(EXIT_FAILURE);
    };

    return window;
}

void gx_imgui_init(gx_context *context)
{
    // ************* Dear Imgui ********************//
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlotContext *implotCtx = ImPlot::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(context->render_target, true);
    ImGui_ImplOpenGL3_Init(context->glsl_version);

    // Load a nice font
    io.Fonts->AddFontFromFileTTF("fonts/Roboto-Regular.ttf", 30.0f);
    // merge in icons from Font Awesome
    static const ImWchar icons_ranges[] = {ICON_MIN_FK, ICON_MAX_16_FK, 0};
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF("fonts/forkawesome-webfont.ttf", 15.0f, &icons_config, icons_ranges);
    // use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid
}

void gx_delete_buffer(GLuint *texture) 
{
	glDeleteBuffers(1, texture);
}

static void create_texture(GLuint *texture, int image_width, int image_height)
{
    // Create a OpenGL texture identifier
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_TEXTURE_2D);
}

static void bind_texture(GLuint *texture)
{
    glBindTexture(GL_TEXTURE_2D, *texture);
}

static void unbind_texture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

static void create_pbo(GLuint *pbo, int image_width, int img_height)
{
    glGenBuffers(1, pbo);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, *pbo);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, image_width * img_height * 4 * sizeof(unsigned char), 0, GL_DYNAMIC_COPY);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}


static void upload_image_pbo_to_texture(int image_width, int image_height)
{
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image_width, image_height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
}


static void bind_pbo(GLuint *pbo)
{
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, *pbo);
}

static void unbind_pbo()
{
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}


void render_initialize_target(gx_context *window)
{
    GLFWwindow *render_target = gx_glfw_init_render_target(3, 3, window->width, window->height, "labjack_utils", window->glsl_version);
    gx_init(window, render_target);
    gx_imgui_init(window);
}

void create_new_frame()
{
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void render_a_frame(gx_context *window)
{
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window->render_target, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(window->render_target);
}

void gx_cleanup(gx_context *window)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // ImPlot::DestroyContext();
    glfwDestroyWindow(window->render_target);
    glfwTerminate();
}


#endif