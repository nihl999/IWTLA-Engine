

Texture
{
    // return;
    if (tex != nullptr)
    {
        printf("returned from cubemesh tex != nullprt\n");
        return;
    }
    std::string filepath = FileUtils::getResourceFolderPath().append("container.jpg");
    printf("%s\n", filepath.c_str());
    stbi_set_flip_vertically_on_load(1);
    byte *data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        const char *failureReason = stbi_failure_reason();
        printf("error loading cube tex %s\n", failureReason);
        exit(1);
    }
    printf("%d", &tex);
    tex = data;
    printf("%d", &tex);
};
