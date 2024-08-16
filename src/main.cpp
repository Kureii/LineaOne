#include <app.h>

#include <memory>

int main(int, char**) {
  if (auto app = std::make_unique<linea_one::App>(); app->Init()) {
    app->Run();
  }
  return 0;
}