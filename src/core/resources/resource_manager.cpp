#include "resource_manager.h"
#include <common/stb_image.h>

namespace ResourceManager
{
    std::unordered_map<std::string, Graphics::Texture> textures = std::unordered_map<std::string, Graphics::Texture>();

    std::string getResourceFolderPath()
    {
        // todo make dynamic...
        return "../resources/";
    }

    Graphics::Texture loadTextureFromFile(std::string filename)
    {
        Graphics::Texture texture = {};

        std::string filepath = getResourceFolderPath().append(filename);
        printf("filepath > %s\n", filepath.c_str());
        byte *data = stbi_load(filepath.c_str(), &texture.width, &texture.height, (i32 *)&texture.nrChannels, 0);
        if (!data)
        {
            const char *failureReason = stbi_failure_reason();
            printf("error loading texture: %s\n", failureReason);
            exit(1);
        }
        // todo logging function
        texture.data = std::make_shared<byte *>(data);
        textures.emplace(filename, texture);
        printf("texture: %s loaded successfully, id: %d\n", filename.c_str(), texture.id);
        return texture;
    };

    void init()
    {
        stbi_set_flip_vertically_on_load(1);
    }

    void shutdown()
    {
        for (auto texture = textures.begin(); texture != textures.end(); texture++)
        {
            stbi_image_free(*texture->second.data);
            // todo this works?
            texture->second.data.reset();
        }
    }

}
