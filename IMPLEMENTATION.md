# Implementation Details

This document provides an overview of the renderer's internal architecture and implemented algorithms.

---

# Rendering Pipeline

Each rendered pixel follows the same workflow:

1. Generate primary rays from the camera.
2. Traverse the scene to find the closest intersection.
3. Evaluate the material at the hit point.
4. Scatter the ray according to the material properties.
5. Recursively trace scattered rays.
6. Average all samples.
7. Apply gamma correction.
8. Write the final colour to the output image.

---

# Camera

Implemented features include:

* Perspective projection
* Adjustable field of view
* Camera positioning
* Look-at transformation
* Depth of field
* Defocus blur
* Configurable focus distance

---


## Multi-threaded Rendering

The renderer uses OpenMP to parallelize the rendering process across multiple CPU cores. The image is divided into independent workloads, allowing multiple threads to trace rays simultaneously.

As each pixel is computed independently, the workload scales well with the number of available cores while requiring very little synchronization. This substantially decreases render times without affecting image quality or rendering accuracy.





# Mathematical Foundation

The renderer is built on a custom `vec3` implementation supporting:

* Vector arithmetic
* Dot product
* Cross product
* Unit vectors
* Reflection
* Refraction

The `ray` class represents rays using an origin and direction vector.

---

# Scene Objects

All renderable objects implement a common hittable interface responsible for:

* Ray intersection
* Surface normal computation
* Bounding box generation

Implemented primitives include:

* Sphere
* Moving sphere

---

# Materials

The renderer currently supports three physically based material models.

## Lambertian

Diffuse surfaces using stochastic hemisphere sampling.

## Metal

Reflective surfaces with configurable roughness.

## Dielectric

Transparent materials supporting:

* Reflection
* Refraction
* Total internal reflection
* Schlick's approximation

---

# Textures

Implemented texture types include:

* Solid colour
* Checker texture
* Image texture

Image loading is performed using **stb_image**.

---

# Bounding Boxes

Each renderable object provides an Axis-Aligned Bounding Box (AABB) used during acceleration structure construction.

---

# Bounding Volume Hierarchy

To reduce ray-object intersection tests, the renderer constructs a Bounding Volume Hierarchy (BVH).

Traversal proceeds by:

1. Testing a node's bounding box.
2. Rejecting entire subtrees on a miss.
3. Recursively traversing child nodes on a hit.
4. Returning the closest valid intersection.

The current implementation partitions objects using the largest axis of the bounding box.

---

# Anti-Aliasing

Anti-aliasing is achieved by generating multiple randomly jittered rays for every pixel and averaging the resulting colours.

---

# Monte Carlo Path Tracing

Indirect illumination is approximated through recursive stochastic sampling. Increasing the number of samples improves convergence and reduces image noise.

---

# Gamma Correction

A gamma correction pass is applied before writing the final image to produce visually accurate output on standard displays.

---

# Output

Images are written in the PPM image format.

---

# External Libraries

* C++17 Standard Library
* stb_image

No rendering engines or graphics APIs are used.

---

# Future Improvements

Planned extensions include:
* A UI interface for scene creation
* Surface Area Heuristic (SAH) BVH construction
* Triangle mesh support
* OBJ file loading
* HDR environment maps
* Multiple Importance Sampling (MIS)
* Volumetric rendering
* GPU acceleration

---

# References

* Peter Shirley — *Ray Tracing in One Weekend*
* Peter Shirley — *Ray Tracing: The Next Week*

