#define GPU_DEBUG_CALLBACK
#include "../../gpulib_imgui.h"
#include "../../gpulib.h"

int main() {
  SDL_Window * sdl_window = NULL;
  GpuWindow("Dear ImGui", 1280, 720, 4, 0, 0, &sdl_window, NULL);
  GpuSetDebugCallback(GpuDebugCallback, NULL);

  imgui_init();

  struct ImGuiIO * io = igGetIO();
  struct ImGuiStyle * style = igGetStyle();

  char font[10000] = {0};
  SDL_snprintf(font, 10000, "%s%s", SDL_GetBasePath(), "NotoSans.ttf");
  ImFontAtlas_AddFontFromFileTTF(io->Fonts, font, 24, NULL, NULL);

  style->ScrollbarRounding = 0;
  style->WindowRounding = 0;
  style->FrameRounding = 0;

  for (;;) {
    GpuClear();
    imgui_new_frame(sdl_window, true);

    {
      // Flat UI by yorick.penninks: https://color.adobe.com/Flat-UI-color-theme-2469224/
      static struct ImVec3 color_for_text = {236 / 255.0, 240 / 255.0, 241 / 255.0};
      static struct ImVec3 color_for_head = { 41 / 255.0, 128 / 255.0, 185 / 255.0};
      static struct ImVec3 color_for_area = { 57 / 255.0,  79 / 255.0, 105 / 255.0};
      static struct ImVec3 color_for_body = { 44 / 255.0,  62 / 255.0,  80 / 255.0};
      static struct ImVec3 color_for_pops = { 33 / 255.0,  46 / 255.0,  60 / 255.0};

      // Mint Y Dark:
      //static struct ImVec3 color_for_text = (struct ImVec3){211 / 255.0, 211 / 255.0, 211 / 255.0};
      //static struct ImVec3 color_for_head = (struct ImVec3){ 95 / 255.0, 142 / 255.0,  85 / 255.0};
      //static struct ImVec3 color_for_area = (struct ImVec3){ 47 / 255.0,  47 / 255.0,  47 / 255.0};
      //static struct ImVec3 color_for_body = (struct ImVec3){ 64 / 255.0,  64 / 255.0,  64 / 255.0};
      //static struct ImVec3 color_for_pops = (struct ImVec3){ 30 / 255.0,  30 / 255.0,  30 / 255.0};

      // Arc Theme:
      //static struct ImVec3 color_for_text = (struct ImVec3){211 / 255.0, 218 / 255.0, 227 / 255.0};
      //static struct ImVec3 color_for_head = (struct ImVec3){ 64 / 255.0, 132 / 255.0, 214 / 255.0};
      //static struct ImVec3 color_for_area = (struct ImVec3){ 47 / 255.0,  52 / 255.0,  63 / 255.0};
      //static struct ImVec3 color_for_body = (struct ImVec3){ 56 / 255.0,  60 / 255.0,  74 / 255.0};
      //static struct ImVec3 color_for_pops = (struct ImVec3){ 28 / 255.0,  30 / 255.0,  37 / 255.0};

      igColorEdit3("Text", &color_for_text.x);
      igColorEdit3("Head", &color_for_head.x);
      igColorEdit3("Area", &color_for_area.x);
      igColorEdit3("Body", &color_for_body.x);
      igColorEdit3("Pops", &color_for_pops.x);

      imgui_easy_theming(color_for_text, color_for_head, color_for_area, color_for_body, color_for_pops);
    }

    static bool show_test_window = true;
    igShowTestWindow(&show_test_window);

    igRender();
    GpuSwap(sdl_window);
    
    for (SDL_Event e; SDL_PollEvent(&e);) {
      imgui_process_event(&e);
      if (e.type == SDL_QUIT)
        goto exit;
    }
  }

exit:
  imgui_deinit();
  return 0;
}