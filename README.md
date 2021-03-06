# Ray Tracing Engine
Mostly based in Ray Tracing books from Peter Shirley.

# Renders
![render1](renders/5000spp-50r-11-02-2022.jpg)
![render2](renders/10000spp-50r-08-01-2022.jpg)
![render3](renders/4000spp-50r-10-01-2022.jpg)
![render4](renders/4000spp-50r-16-01-2022.jpg)

# COMPILATION
Standing from the root of the project:
1. `cmake CMakeLists.txt -B build`
2. `cmake --build build`
3. Lastly, to run: `./build/main`

Note: On windows you need to install posix pthreads or you can disable threads with flag `DISABLE_THREADS` when compiling.

# CONFIGURATION
For the scenary and visual options of the engine you need to configure the file "conf.json". The syntax of the file is:

- filename_output: string
- format_output: string with options: "jpg" or "png" or "ppm",
- image_width: unsigned int
- image_height: unsigned int
- samples_per_pixel: unsigned int
- max_depth: unsigned int
- background: color
- camera: 
    - lookfrom : vec3
    - lookat: vec3
    - vup: vec3
    - dist_to_focus: double,
    - aperture: double
- world: array of object with material (see example)
- constant_mediums: array of constant mediums


Where:  
*vec3 := [double, double, double]*  
*color := [double, double, double]* where each component goes from 0 to 1 and represents RGB.
### Properties Objects
Possible types with their corresponding attributes:

- sphere
    - position: vec3
    - radius: 60

- polygon
    - points: vec3[]  // in order (depending on clockwise or counter-clockwise will be different normal vector direction)
    - radius: 60

- triangle_mesh
    - model: string
    - rotate: vec3
    - scale: double
    - position: vec3

- constant_medium
    - density: double
    - color: color
    - boundary: a hittable object

### Properties Materials
Possible types with their corresponding attributes:

- diffuse_light
    - color: color
    - intensity: double

- solid_color
    - color: color

- dielectric
    - refraction: double

- lambertian
    - color: color

- metal
    - color: color
    - fuzziness: double

- lambertian-texture
    - texture: texture

- metal-texture
    - texture: texture
    - fuzziness: double

### Properties Texture
Possible types with their corresponding attributes:
- image
    - path: string

- checker
    - color1: color
    - color2: color

### Properties Constant Medium
- density: double
- color: color
- boundary: an object to use as boundary


### Example

```json
{
    "filename_output": "img.jpg",
    "format_output": "jpg",
    "image_width": 1280,
    "image_height": 720,
    "samples_per_pixel": 100,
    "max_depth": 50,
    "background": [0,0,0],
    "camera": {
        "lookfrom" : [1000,1000,1000],
        "lookat": [-50,100,0],
        "vup": [0,1,0],
        "dist_to_focus": 10.0,
        "aperture": 0.01
    },
    "world": [
        {
            "object": {
                "type": "polygon",
                "properties":{
                    "points": [
                        [-1500,0,-1500],
                        [1500,0,-1500],
                        [1500,0,1500],
                        [-1500,0,1500]
                    ]
                }
            },
            "material": {
                "type": "lambertian",
                "properties": {
                    "color": [0, 0.8, 0.6]
                }
            }
        },
        {
            "object": {
                "type": "triangle_mesh",
                "properties": {
                    "model": "models/dragon.obj",
                    "rotate": [0, 0, 0],
                    "scale": 2,
                    "position": [0,50,0]
                }
            },
            "material": {
                "type": "dielectric",
                "properties": {
                    "refraction": 0.002
                }
            }
        },
        {
            "object": {
                "type": "polygon",
                "properties": {
                    "points": [
                        [-200,200,100],
                        [-200,200,-0],
                        [-200,0,-0],
                        [-200,0,100]
                    ]
                }
            },
            "material": {
                "type": "diffuse_light",
                "properties": {
                    "color": [1,1,1],
                    "intensity": 10
                }
            }
        }
    ],
    "constant_mediums": [
        {
            "density": 0.00001,
            "color": [1,1,1],
            "boundary": {
                "type": "sphere",
                "properties":{
                    "position": [0,0,0],
                    "radius": 500
                }
            }
        }
    ]
}
```
