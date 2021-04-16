#include "Engine.h"

#include "OctTree.h"

void OctTreeNode::Draw()
{
  for (OctTreeNode* node : childrenList)
  {
    if (node)
    {
      node->Draw();
    }
  }

  if (showBoxes)
  {
    Rect3D rect;
    rect.Center = center;
    rect.depth = halfWidth * 2;
    rect.height = halfWidth * 2;
    rect.width = halfWidth * 2;

    glm::vec3 color(level * 1.0f/ maxDepth);


    Engine::get().m_Debug.drawWorldRects(rect, color, true);
  }
}