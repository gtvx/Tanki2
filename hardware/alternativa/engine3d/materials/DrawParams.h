#ifndef DRAWPARAMS_H
#define DRAWPARAMS_H

class VulkanWindow;
class Camera3D;
class VertexBufferResource;
class IndexBufferResource;
class Object3D;
class VulkanUniform;

class DrawParams
{
public:
    VulkanWindow *vulkanWindow;
    Camera3D *camera;
    VertexBufferResource *vertexBuffer;
    IndexBufferResource *indexBuffer;
    int firstIndex;
    int numTriangles;
    Object3D *object;
	VulkanUniform *vulkanUniform;
	bool decal = false;
};

#endif // DRAWPARAMS_H
