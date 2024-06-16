#include "Application.h"

int main(void)
{
    Application application = Application();

    while (application.shouldRun)
    {
        application.Update();
    }

    application.CleanUp();
    return 0;
}
