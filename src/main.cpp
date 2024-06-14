#include "Application.h"

int main(void)
{
    Application application = Application();
    printf("\n%d %d %d \n", CubeMesh::width, CubeMesh::height, CubeMesh::nrChannels);

    while (application.shouldRun)
    {
        application.Update();
    }

    application.CleanUp();
    return 0;
}
