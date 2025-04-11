#include <SDL3/SDL.h>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include "tinyfiledialogs/tinyfiledialogs.h"

// #include <commdlg.h>
#include <windows.h>

#include "ImGuiFileDialog.h"

namespace fs = std::filesystem;

HWND GetSDLWindowHandle(SDL_Window *window) {
  auto hwnd = static_cast<HWND>(
          SDL_GetPointerProperty(SDL_GetWindowProperties(window),
                                 SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr));
  return hwnd;
}

std::string OpenFileDialog(HWND parent) {
  // OPENFILENAMEA ofn;
  // char szFile[260] = {0};
  // ZeroMemory(&ofn, sizeof(ofn));
  // ofn.lStructSize = sizeof(ofn);
  // ofn.hwndOwner = parent;
  // ofn.lpstrFile = szFile;
  // ofn.nMaxFile = sizeof(szFile);
  // ofn.lpstrFilter = "Executable Files (*.exe)\0*.exe\0All Files (*.*)\0*.*\0";
  // ofn.nFilterIndex = 1;
  // ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  //
  // if (GetOpenFileNameA(&ofn)) {
  //   return ofn.lpstrFile;
  // }
  return "";
}

void drawGui() {
  IGFD::FileDialogConfig config;
  config.path = ".";
  ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp", config);
  if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
    if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
      // action
    }

    // close
    ImGuiFileDialog::Instance()->Close();
  }
}

int main(int, char **) {
  // [If using SDL_MAIN_USE_CALLBACKS: all code below until the main loop starts
  // would likely be your SDL_AppInit() function]
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
    printf("Error: SDL_Init(): %s\n", SDL_GetError());
    return -1;
  }

  Uint32 window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
  SDL_Window *window = SDL_CreateWindow("Leprac", 1280, 720, window_flags);
  if (window == nullptr) {
    printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
    return -1;
  }
  SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
  SDL_SetRenderVSync(renderer, 1);
  if (renderer == nullptr) {
    SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
    return -1;
  }
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  SDL_ShowWindow(window);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void) io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard |
                    ImGuiConfigFlags_NavEnableGamepad;

  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer3_Init(renderer);

  bool show_demo_window = true;
  bool show_another_window = true;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  fs::path exe_path{};

  bool show_dialog = false;

  bool done = false;
  while (!done) {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
    // tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to
    // your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
    // data to your main application, or clear/overwrite your copy of the
    // keyboard data. Generally you may always pass all inputs to dear imgui,
    // and hide them from your application based on those two flags. [If using
    // SDL_MAIN_USE_CALLBACKS: call ImGui_ImplSDL3_ProcessEvent() from your
    // SDL_AppEvent() function]
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL3_ProcessEvent(&event);
      if (event.type == SDL_EVENT_QUIT)
        done = true;
      if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
          event.window.windowID == SDL_GetWindowID(window))
        done = true;
    }

    // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your
    // SDL_AppIterate() function]
    if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
      SDL_Delay(10);
      continue;
    }

    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("test");
    ImGui::Text("This is some useful text.");
    if (ImGui::Button("set path")) {
    }
    if (ImGui::Button("Select .exe Path")) {
      show_dialog = true;
      HWND parentHwnd = GetSDLWindowHandle(window);
      std::string path = OpenFileDialog(parentHwnd);
    }
    if (ImGui::Button("Open")) {
      system("\"D:/Game/len'en/le01 1.20a/Le01.exe\"");
    }
    ImGui::End();

    if (show_dialog) {
      drawGui();
      // printf("Button clicked.\n");
      // const char *filters[] = {"*.exe"};
      //
      // const char *selectedFile =
      //         tinyfd_openFileDialog("Select an EXE file", // title
      //                               ".", // default dir
      //                               1, // # filters
      //                               filters, // ext filters
      //                               "Executable Files", // filter desc
      //                               0 // 是否允许多选
      //         );
      //
      // if (selectedFile) {
      //   printf("Selected: %s\n", selectedFile);
      //   exe_path = fs::path(selectedFile);
      // } else {
      //   printf("Dialog closed or error.\n");
      // }
      // show_dialog = false;
    }

    ImGui::ShowDemoWindow(&show_demo_window);

    {
      static float f = 0.0f;
      static int counter = 0;

      ImGui::Begin("Hello, world!");
      ImGui::Checkbox("Demo Window", &show_demo_window);
      ImGui::Checkbox("Another Window", &show_another_window);
      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
      ImGui::ColorEdit3(
              "clear color",
              (float *) &clear_color); // Edit 3 floats representing a color

      if (ImGui::Button("Button")) // Buttons return true when clicked (most
                                   // widgets return true when edited/activated)
        counter++;
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / io.Framerate, io.Framerate);
      ImGui::End();
    }

    if (show_another_window) {
      ImGui::Begin(
              "Another Window",
              &show_another_window); // Pass a pointer to our bool variable (the
                                     // window will have a closing button that
                                     // will clear the bool when clicked)
      ImGui::Text("Hello from another window!");
      if (ImGui::Button("Close Me"))
        show_another_window = false;
      ImGui::End();
    }

    ImGui::Render();
    // SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x,
    // io.DisplayFramebufferScale.y);
    SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y,
                                clear_color.z, clear_color.w);
    SDL_RenderClear(renderer);
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);



  }

  // Cleanup
  // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your
  // SDL_AppQuit() function]
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
