## Controls
* ```ESC``` - quit the application
* ```1, 2, 3``` - choose camera (stationary/tracking ship/TPP following ship)
* ```0``` - enable freecam (controlled with ```WASD``` and mouse movement), enabled by default
* ```UP/DOWN/LEFT/RIGHT``` - change (relative) direction of the spaceship's spotlight
* ```Z/X``` - change between day/night
* ```C/V``` - decrease/increase level of fog
* ```B/N``` - change between Blinn/Phong specular lighting calculations
* ```F/G``` - enable/disable normal mapping for certain textures

## Additional functionality documentation
### Phong/Blinn specular lighting component
The functionality is implemented by using a simple switch in the light calculation functions
in both ```BaseFragmentShader.fs``` and ```NormalFragmentShader.fs``` like such:
```glsl
    if (useBlinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    };
```
Where ```useBlinn``` is an externally set boolean uniform. It changes calculations
of the light's specular component between Blinn and Phong shading models (where it's
based either on the degree between normal and halfway vectors or between view and
reflection vectors).

### Normal mapping
The normal mapping is handled by appropriate measures in ```NormalVertexShader.fs```
and ```NormalFragmentShader.fs```. Firstly, a
```TBN``` (tangent/bitangent/normal) matrix is calculated in the vertex shader, like such:
```glsl
    mat4 normalMatrix = transpose(inverse(model));

    vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(normalMatrix * vec4(aNormal,    0.0)));
    vs_out.TBN = mat3(T, B, N);
```
Note, that the normal vector is multiplied by the transpose of an inverse of the
model matrix to get its appropriate direction. ```aNormal```, ```aTangent```
and ```aBitangent``` are passed in vector arrays of appropriate models
and setting them up properly (as with the ```Shape``` class included in this project)
is necessary for the normal mapping to work correctly.

Afterwards, the ```TBN``` matrix is passed to the fragment shader, in which it's
used to transform the normal vector read from the normal map from tangent
space to world space, like such:

```glsl
    vec3 norm = texture(material.normal1, fs_in.TexCoords).rgb;
    norm = norm * 2.0 - 1.0;
    norm = normalize(fs_in.TBN * norm);
```

Afterwards, all of the calculations in the fragment shader proceed normally.