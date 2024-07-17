#include <core/application/application.h>
#include <core/logger.h>

#ifdef OURO_PLATFORM_WIN

extern Ouroboros::Application *Ouroboros::CreateApplication();

int main(int argc, char *argv[]) {
  Ouroboros::StartupLogging();
  OUROINFO("Engine started");
  auto application = Ouroboros::CreateApplication();
  application->Run();
  Ouroboros::ShutdownLogging();
  delete application;
  return 0;
}
#endif
