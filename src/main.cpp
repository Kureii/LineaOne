#include "gui/app.h"

int main(int, char**) {
  if (auto app = std::make_unique<linea_ona::App>(); app->Init()) {
    app->Run();
  }
  return 0;
}