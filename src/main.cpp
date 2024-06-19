#include <core/application/application.h>

int main(void)
{
    Application application = Application();

    while (application.shouldRun)
    {
        application.Update();
    }

    return 0;
}
