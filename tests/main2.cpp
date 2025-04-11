#include "SDL.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl3.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <filesystem>
#include <string>
#include <cstring>
#include <cstdio>

namespace fs = std::filesystem;

enum class GameID {
    Le01,
    Le02,
    Le03,
    Le04,
    count,
};

static const char *GameName[]{
    "Le01.exe",
};


static std::string game_a_path;
static std::string game_b_path;
static std::string game_c_path;
static std::string search_directory;

static int selected_game = -1;

// 自动搜索指定目录下的游戏可执行文件，假设文件名为 a.exe、b.exe、c.exe
void AutoSearchGames(const std::string &directory) {
    // 清空之前的路径
    game_a_path.clear();
    game_b_path.clear();
    game_c_path.clear();

    try {
        for (const auto &entry: fs::directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                std::string path = entry.path().string();
                if (path.find("a.exe") != std::string::npos) {
                    game_a_path = path;
                } else if (path.find("b.exe") != std::string::npos) {
                    game_b_path = path;
                } else if (path.find("c.exe") != std::string::npos) {
                    game_c_path = path;
                }
            }
        }
    } catch (const std::exception &e) {
        printf("Error during directory iteration: %s\n", e.what());
    }
}

int main(int, char **) {
    // 初始化 SDL3（视频和定时器子系统）
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return -1;
    }

    // 设置 OpenGL 属性（这里以 OpenGL 3.2 Core 为例）
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // 创建 SDL3 窗口（SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE）
    SDL_Window *window = SDL_CreateWindow("Universal Game Mod Tool",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        printf("Error creating SDL window: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // 创建 OpenGL 上下文
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        printf("Error creating GL context: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // 启用垂直同步

    // 创建 ImGui 上下文并初始化平台和渲染器后端
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    ImGui::StyleColorsDark();

    // 注意：此处使用的是假设存在的 ImGui_Impl_SDL3_InitForOpenGL（基于SDL3的实现）
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");

    // 用于存储文本输入缓冲区，初始大小设为260字符
    char buf_a[260] = {0};
    char buf_b[260] = {0};
    char buf_c[260] = {0};
    char buf_dir[260] = {0};

    bool done = false;
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // 如果收到退出事件，则退出主循环
            if (event.type == SDL_EVENT_QUIT)
                done = true;

            // 将事件传递给 ImGui
            ImGui_ImplSDL3_ProcessEvent(&event);
        }

        // 启动新的一帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame(window);
        ImGui::NewFrame();

        // 构建 GUI 面板
        ImGui::Begin("Game Mod Control Panel");

        // 游戏选择区域
        ImGui::Text("Select the game to launch:");
        if (ImGui::Button("Game A"))
            selected_game = 0;
        ImGui::SameLine();
        if (ImGui::Button("Game B"))
            selected_game = 1;
        ImGui::SameLine();
        if (ImGui::Button("Game C"))
            selected_game = 2;
        ImGui::Text("Selected game: %s",
                    selected_game == 0
                        ? "Game A"
                        : (selected_game == 1 ? "Game B" : (selected_game == 2 ? "Game C" : "None")));

        ImGui::Separator();

        // 手动设置各个游戏的路径
        ImGui::Text("Manual Game Path Settings:");
        std::strncpy(buf_a, game_a_path.c_str(), sizeof(buf_a));
        if (ImGui::InputText("Game A Path", buf_a, sizeof(buf_a)))
            game_a_path = buf_a;
        std::strncpy(buf_b, game_b_path.c_str(), sizeof(buf_b));
        if (ImGui::InputText("Game B Path", buf_b, sizeof(buf_b)))
            game_b_path = buf_b;
        std::strncpy(buf_c, game_c_path.c_str(), sizeof(buf_c));
        if (ImGui::InputText("Game C Path", buf_c, sizeof(buf_c)))
            game_c_path = buf_c;

        ImGui::Separator();

        // 自动搜索区：输入一个目录，然后搜索该目录下是否存在 a.exe, b.exe, c.exe
        ImGui::Text("Auto Search for Game Executables (a.exe, b.exe, c.exe):");
        std::strncpy(buf_dir, search_directory.c_str(), sizeof(buf_dir));
        if (ImGui::InputText("Search Directory", buf_dir, sizeof(buf_dir)))
            search_directory = buf_dir;
        if (ImGui::Button("Search for Games"))
            AutoSearchGames(search_directory);

        // 显示搜索结果
        ImGui::Text("Found Game A: %s", game_a_path.empty() ? "Not Found" : game_a_path.c_str());
        ImGui::Text("Found Game B: %s", game_b_path.empty() ? "Not Found" : game_b_path.c_str());
        ImGui::Text("Found Game C: %s", game_c_path.empty() ? "Not Found" : game_c_path.c_str());

        ImGui::Separator();

        // 开始游戏按钮（后续可根据selected_game与路径启动游戏）
        if (ImGui::Button("Start Game")) {
        }

        ImGui::End();

        // 渲染部分
        ImGui::Render();
        // 获取当前窗口尺寸（这里可使用SDL_GL_GetDrawableSize获得更准确的尺寸）
        int display_w = 1280, display_h = 720;
        SDL_GL_GetDrawableSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    // 清理资源
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
