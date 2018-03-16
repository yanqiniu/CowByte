# CowByte Engine Scenegraph Overview

### Basic Data Structures:
 * SceneNode class is what a Scenegraph in CBE is made of.
 * A SceneNode contains a m_LocalTransform matrix and a m_WorldTransform matrix.
 * SceneNode publicly inherits from Component, and that is how you can attach things like a MeshInstance to a SceneNode and have it show up with different transform.

### The Hierarchy:
 * Root?
    - I'm not currently thinking about adding copmplicated features like splitscreen or...idk, maybe async scene loading needs multiple root nodes too? Therefore one Root SceneNode should be enough - SceneNode::RootNode is defined as a static.
 * Attach to SceneNode:
    - In Component class, there's a AttachTo_NonSceneNode_Parent() and an AttachTo_SceneNode_Parent(), call the respective version. This seems a little redundant and it's because CBE currently does not supports any form of Component type specifier. This will prob be fixed in the future.

### The Transforms:
* Like mentioned above each node has two transform matrices:
    - m_LocalTransform: transform relative to a SceneNode parent.
    - m_WorldTransform: world transform matrix that transforms a model space position to world space. 
* m_WorldTransform update:
    - Notice that this does not update automatically with m_LocalTransform because it requires walking the tree from current node all the way to RootNode to update it, and that is costly if multiple local updates are called within one frame. So call SceneNode::UpdateWorldTransform() manually when you feel it should.
