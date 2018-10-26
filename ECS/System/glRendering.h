#ifndef __RENDERING__ 
#define __RENDERING__ 

#include <vector>
#include <utility>
#include <algorithm>


#include <ECS/Component/component.h>
#include <ECS/Component/componentmanager.h>
#include <ECS/Component/texturecomponent.h>
#include <ECS/Entity/entitymanager.h>
#include <ECS/Component/entityinformationcomponent.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

using namespace GTech;
/* Rendering System for OpenGl 


1. DATUM ORGANIZATION AND RELATION. 

+ A Mesh (MSH) is a geometry 3d model. For example a Cube, a pyramid, an airplane, a Cylinder, a tank, the avatar of a game or a Non Playing Character a.k.a NPC. The following will explain how it's the rendering data organized and related. 

+ For each Mesh (MSH) we have a list of rendering tuples (RTPLs). A RTPL is the smallest drawable data unit. To describe better what a RTPL is let's set some examples: 

    Example 1: Let's say we have a Cube Mesh (6 faces), if each face has a different color, each face will be represented by a RTPL, so that cube will have a list of 6 RTPLs. 

    Example 2: Let's say we have a Cube, three faces are blue and three faces are red. So there will be a list of two RTPLs. The first RTPL is to be painted by a blue diffuse color and the second one by a red diffuse color. 

    Example 3: Let's say we have a Cube, 5 faces are red and the face left is wrapped with a texture. So the list will be two RTPLs. The first RTPL is the one with the 5 faces and the second one is the one which would wrap the texture on the face. 

    So in first instance an RTPL is a group of faces that are to be draw in ONE and just ONE particular way. 

+ A RTPL (Rendering Tuple) is the smallest drawable unit, and it is composed by a collection of objects or datum. This collection is formed by: A Transformation Matrix (TMTX) a Vertex Array Object (VAO) and a Shader-Material (SHDR, MAT) pair.

+ A Transformation Matrix (TMTX) it's a numerical function represented by a matrix which provides the RTPL within the boundaries of the parent MSH (remember a RTPL belongs to a list which belongs to a MSH) local coordinates, with a location a scaled size and a rotation / orientation. The TMTX it's the _same_ for all the members of the list, and you could have one per MSH's RTPL list, but for flexibility we would keep the TMTX referenced in each RTPL. 

+ A Vertex Array Object (VAO) it's a reference to a data structure describing the geometry within the RTPL collection. If the RTPL represents the face of a MSH Cube, the VAO it's a reference to a data structure which describes the two triangles to make the MSH cube face. Every RTPL it's an instance of a VAO, and every RTPL has only and only one VAO reference.

+ A Material (MAT) it's a reference to a data structure which describes the RTPL's material and how it is set in the VAO. It might be set by means of a diffuse color or a texture.

+ A Shader (SHDR) it's a reference to a program that tells the rendering driver _how to_ render the RTPL. Several RTPLs across several MSHs may have the same SHDR references, this makes sense as SHDR references a program which in general, describes the way lights will interact with RTPL's material (MAT)

+ A Shader & Material pair (SHDR, MAT) it's a reference that indicates how the SHDR will draw the RTPL in relation with it's MAT. Every RTPL it's an instance of a (SHDR,MAT) pair, and every RTPL has only and only one (SHDR,MAT) pair reference.

2. RENDERING CYCLE.

*/

  




namespace ECS {
    
    
    class RenderingSystem {

        static unsigned long    SubscribeEntity(unsigned int entityId);
        static unsigned int     DrawSprites2D();
        static void             InitRenderingSystem();
        static void             ShutdownRenderingSystem();
        static void             UpdateRenderingSystem();

    };

}
#endif /*__RENDERING__ */
 
