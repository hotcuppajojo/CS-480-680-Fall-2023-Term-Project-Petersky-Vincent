# CS-480-680-Fall-2023-Term-Project-Petersky-Vincent

## Solar System Exploration Game

University of Nevada, Reno - Fall 2023
Instructor: Dr. Alireza Tavakkoli

## Authors and Contributions
- JoJo Petersky:
  - Lighting implementation, user interaction, camera modes, engine updates, starship model implementation, rendering fixes.

- Zephyr Vincent:
  - Skybox implementation, initial shader setup, texturing, initial lighting, planetary system setup, asteroid belt setup (partial).

Base Code: UNR CS480 PA-5 Solution provided by Dr. Alireza Tavakkoli
Included classes: Camera, Engine, Window, Graphics (+ graphics_headers), Shader, Texture, Mesh, Object, Sphere

---

## Gameplay Interactions

### Game Modes
- **Exploration Mode**: Navigate through space with a third-person view of your spaceship.
- **Planetary Observation Mode**: Observe planets up close in their respective orbits from a first-person perspective.

### Controls
- **Movement**: Use `W` to move forward, `S` to move backward, `A` to turn left, and `D` to turn right.
- **Mode Switch**: Press the `Spacebar` to toggle between Exploration Mode and Planetary Observation Mode.

## Implementation Details

### Interactivity Implementation
- **WASD Controls**: Implemented to provide a familiar control scheme for movement, commonly used in many games.
- **Spacebar Mode Switch**: Offers a quick and intuitive method to switch perspectives, enhancing the gameplay experience.

### Dependencies and Libraries

#### GLEW (OpenGL Extension Wrangler Library)
- GLEW is utilized for managing OpenGL extensions, allowing us to access modern graphics functionality across various platforms without directly handling the OS-specific loading mechanisms.

#### GLFW3 (Graphics Library Framework)
- GLFW is used for creating the window and OpenGL context, handling inputs, and managing the event loop. It provides a simple API that works across multiple operating systems, simplifying platform-specific intricacies.

#### ASSIMP (Open Asset Import Library)
- ASSIMP is crucial for loading complex models from various file formats. It abstracts the file format complexities and provides a consistent data structure, making it easier to process 3D models.

#### GLM (OpenGL Mathematics)
- GLM is a mathematics library designed for graphics. It implements vectors, matrices, and other mathematical constructs and operations, mimicking GLSL to work seamlessly with OpenGL.

#### SOIL2 (Simple OpenGL Image Loader)
- SOIL2 handles the loading of textures into OpenGL, abstracting the details of file format handling and texture binding. It's chosen for its simplicity and ease of integration into our graphics pipeline.

## Conclusion

This project brings together various aspects of computer graphics programming, from user input handling to complex 3D transformations and rendering. Each library and control scheme has been selected and implemented to create an engaging and smooth user experience that showcases the potential of modern OpenGL applications.
