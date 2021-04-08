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

  if (objectsList.size() != 0)
  {
    Rect3D rect;
    rect.Center = center;
    rect.depth = halfWidth * 2;
    rect.height = halfWidth * 2;
    rect.width = halfWidth * 2;

    Engine::get().m_Debug.drawWorldRects(rect, { 1,1,1 }, true);
  }
}