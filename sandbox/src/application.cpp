#include <ouroboros.h>

class Sandbox : public Ouroboros::Application {
public:
  Sandbox() = default;
  ~Sandbox() = default;
};

Ouroboros::Application *Ouroboros::CreateApplication() { return new Sandbox(); }