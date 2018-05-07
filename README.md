# Sigma Engine
Sigma Engine is a graphics engine written in C++17 using OpenGL 4.6. The primary purpose of this project is to be a platform for learning new real-time graphical techniques.
## Features
### Deferred Rendering
![Alt text](screenshots/gbuffer.png?raw=true "Geometry Buffer")
Top left = diffuse color

Bottom left = roughness (white is a rough surface, black is a smooth surface)

Top Right = world space normals computed from screen position and depth buffer.

Bottom Right = depth visualization
### Analytical Directional Lights
![Alt text](screenshots/directional_lights.png?raw=true "Analytical Directional Lights")
### Analytical Spot Lights
![Alt text](screenshots/spot_lights.png?raw=true "Analytical Spot Lights")
### Analytical Point Lights
![Alt text](screenshots/point_lights.png?raw=true "Analytical Point Lights")
### Cascaded Shadow Maps
![Alt text](screenshots/cascaded_shadow_maps.png?raw=true "Cascaded Shadow Maps")
### IBL (Image Based Lighting)
![Alt text](screenshots/image_based_lighting_0.png?raw=true "Image Based Lighting")
On the left Image based Lighting is enabled and on the right IBL is disabled. Notice how IBL simulates more accurate indirect lighting.   
![Alt text](screenshots/image_based_lighting_1.png?raw=true "Image Based Lighting")
On the left Image based Lighting is enabled and on the right IBL is disabled. Notice the sky's reflection on the surface of the sphere.
