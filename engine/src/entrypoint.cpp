#include <core/application/application.h>

#ifdef OURO_PLATFORM_WIN
extern Ouroboros::Application *Ouroboros::CreateApplication();

int main(int argc, char *argv[]) {
  auto application = Ouroboros::CreateApplication();
  application->Run();
  delete application;
  return 0;
}
#endif
