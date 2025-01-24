#ifndef RENDERER3D_SHAPE_H
#define RENDERER3D_SHAPE_H

#include "Renderable.h"
#include "Mesh.h"
#include "stb_image.h"

class Shape : public Renderable {
public:
    Mesh mesh;

    void Draw(Shader &shader) override{
        mesh.Draw(shader);
    }

private:
    Shape(Mesh mesh) : mesh(std::move(mesh)) {}

    static vector<Texture> GetTextures(const string diffusePath = "", const string specularPath = "",
                                const string normalPath = "", const string heightPath = "")
    {
        vector<Texture> textures;

        if(diffusePath != "")
        {
            textures.emplace_back(
                    Texture {
                            .id = TextureFromFile(diffusePath.c_str()),
                            .type = "material.diffuse",
                            .path = diffusePath
                    }
            );
        }
        if(specularPath != "")
        {
            textures.emplace_back(
                    Texture {
                            .id = TextureFromFile(specularPath.c_str()),
                            .type = "material.specular",
                            .path = specularPath
                    }
            );
        }
        if(normalPath != "")
        {
            textures.emplace_back(
                    Texture {
                            .id = TextureFromFile(normalPath.c_str()),
                            .type = "material.normal",
                            .path = normalPath
                    }
            );
        }
        if(heightPath != "") {
            textures.emplace_back(
                    Texture{
                            .id = TextureFromFile(heightPath.c_str()),
                            .type = "material.height",
                            .path = heightPath
                    }
            );
        }

        return textures;
    }

    static unsigned int TextureFromFile(const char *path)
    {
        string filename = string(path);

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }

public:

    static Shape GetCube(const string& diffusePath = "",
                         const string& specularPath = "",
                         const string& normalPath = "",
                         const string& heightPath = "")
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        glm::vec3 positions[] = {
                // Front face
                {-0.5f, -0.5f,  0.5f},
                { 0.5f, -0.5f,  0.5f},
                { 0.5f,  0.5f,  0.5f},
                {-0.5f,  0.5f,  0.5f},
                // Back face
                {-0.5f, -0.5f, -0.5f},
                { 0.5f, -0.5f, -0.5f},
                { 0.5f,  0.5f, -0.5f},
                {-0.5f,  0.5f, -0.5f},
                // Top face
                {-0.5f,  0.5f, -0.5f},
                { 0.5f,  0.5f, -0.5f},
                { 0.5f,  0.5f,  0.5f},
                {-0.5f,  0.5f,  0.5f},
                // Bottom face
                {-0.5f, -0.5f, -0.5f},
                { 0.5f, -0.5f, -0.5f},
                { 0.5f, -0.5f,  0.5f},
                {-0.5f, -0.5f,  0.5f},
                // Right face
                { 0.5f, -0.5f, -0.5f},
                { 0.5f,  0.5f, -0.5f},
                { 0.5f,  0.5f,  0.5f},
                { 0.5f, -0.5f,  0.5f},
                // Left face
                {-0.5f, -0.5f, -0.5f},
                {-0.5f,  0.5f, -0.5f},
                {-0.5f,  0.5f,  0.5f},
                {-0.5f, -0.5f,  0.5f},
        };

        glm::vec3 normals[] = {
                // Normals per face
                { 0.0f,  0.0f,  1.0f}, // Front
                { 0.0f,  0.0f, -1.0f}, // Back
                { 0.0f,  1.0f,  0.0f}, // Top
                { 0.0f, -1.0f,  0.0f}, // Bottom
                { 1.0f,  0.0f,  0.0f}, // Right
                {-1.0f,  0.0f,  0.0f}, // Left
        };

        glm::vec2 texCoords[] = {
                {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
        };

        glm::vec3 tangents[] = {
                {1.0f,  0.0f,  0.0f},  // Front
                {-1.0f,  0.0f,  0.0f}, // Back
                {1.0f,  0.0f,  0.0f},  // Top
                {-1.0f,  0.0f,  0.0f}, // Bottom
                {0.0f,  0.0f, -1.0f},  // Right
                {0.0f,  0.0f,  1.0f},  // Left
        };

        glm::vec3 bitangents[] = {
                {0.0f,  1.0f,  0.0f},  // Front
                {0.0f,  1.0f,  0.0f},  // Back
                {0.0f,  0.0f,  1.0f},  // Top
                {0.0f,  0.0f, -1.0f},  // Bottom
                {1.0f,  0.0f,  0.0f},  // Right
                {1.0f,  0.0f,  0.0f},  // Left
        };

        for (int face = 0; face < 6; face++) {
            for (int v = 0; v < 4; v++) {
                Vertex vertex;
                vertex.Position = positions[face * 4 + v];
                vertex.Normal = normals[face];
                vertex.TexCoords = texCoords[v];
                vertex.Tangent = tangents[face];
                vertex.Bitangent = bitangents[face];
                vertices.push_back(vertex);
            }
        }

        // Indices
        unsigned int faceIndices[] = {
                0, 1, 2, 2, 3, 0,   // Front
                4, 5, 6, 6, 7, 4,   // Back
                8, 9, 10, 10, 11, 8, // Top
                12, 13, 14, 14, 15, 12, // Bottom
                16, 17, 18, 18, 19, 16, // Right
                20, 21, 22, 22, 23, 20  // Left
        };

        for (int i = 0; i < 36; i++) {
            indices.push_back(faceIndices[i]);
        }

        textures = GetTextures(diffusePath, specularPath, normalPath, heightPath);

        return Shape(Mesh(vertices, indices, textures));
    }

    static Shape GetPlane(const string& diffusePath = "",
                         const string& specularPath = "",
                         const string& normalPath = "",
                         const string& heightPath = "")
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        // Vertex positions
        glm::vec3 positions[] = {
                {-0.5f, 0.0f, -0.5f}, // Bottom-left
                { 0.5f, 0.0f, -0.5f}, // Bottom-right
                { 0.5f, 0.0f,  0.5f}, // Top-right
                {-0.5f, 0.0f,  0.5f}, // Top-left
        };

        // Texture coordinates
        glm::vec2 texCoords[] = {
                {0.0f, 0.0f}, {100.0f, 0.0f}, {100.0f, 100.0f}, {0.0f, 100.0f},
        };

        // Normal
        glm::vec3 normal = {0.0f, 1.0f, 0.0f};

        // Tangent and bitangent
        glm::vec3 tangent = {1.0f, 0.0f, 0.0f};   // Along the X-axis
        glm::vec3 bitangent = {0.0f, 0.0f, 1.0f}; // Along the Z-axis

        // Populate vertices
        for (int i = 0; i < 4; i++) {
            Vertex vertex;
            vertex.Position = positions[i];
            vertex.Normal = normal;
            vertex.TexCoords = texCoords[i];
            vertex.Tangent = tangent;
            vertex.Bitangent = bitangent;
            vertices.push_back(vertex);
        }

        // Indices for two triangles
        unsigned int planeIndices[] = {
                0, 1, 2, // Bottom-right triangle
                2, 3, 0  // Top-left triangle
        };

        for (int i = 0; i < 6; i++) {
            indices.push_back(planeIndices[i]);
        }

        textures = GetTextures(diffusePath, specularPath, normalPath, heightPath);

        return Shape(Mesh(vertices, indices, textures));
    }
};

#endif //RENDERER3D_SHAPE_H
