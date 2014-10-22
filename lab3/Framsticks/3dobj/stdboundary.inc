function buildWorldBoundary()

{
  var spacing;
  switch (World.wrldbnd)
  {
  case 0:
    return ;
  case 1:
    Loader.load("fence-element.ac");
    spacing = 1.99;
    break;
  case 2:
    Loader.load("teleport.ac");
    spacing = 3.0;
    break;
  }
  if (Loader.loaded == 0)
    return ;
  GeomBuilder.addBranch(GeomBuilder.rootNode);
  var node = GeomBuilder.currentNode;
  var pos = 0.0;
  var z = -0.2;
  if (World.simtype) z=0.0;
  while (pos <= World.wrldsiz)
  {
    GeomBuilder.addTransform(node);
    GeomBuilder.matrixReset();
    GeomBuilder.matrixMove(pos, 0, z);
    GeomBuilder.setMatrix(GeomBuilder.currentNode);
    GeomBuilder.add(Loader.loaded);

    GeomBuilder.addTransform(node);
    GeomBuilder.matrixMove(0, World.wrldsiz, 0);
    GeomBuilder.setMatrix(GeomBuilder.currentNode);
    GeomBuilder.add(Loader.loaded);

    GeomBuilder.addTransform(node);
    GeomBuilder.matrixReset();
    GeomBuilder.matrixOrient(0, 0, 1.57);
    GeomBuilder.matrixMove(0, pos, z);
    GeomBuilder.setMatrix(GeomBuilder.currentNode);
    GeomBuilder.add(Loader.loaded);

    GeomBuilder.addTransform(node);
    GeomBuilder.matrixMove(World.wrldsiz, 0, 0);
    GeomBuilder.setMatrix(GeomBuilder.currentNode);
    GeomBuilder.add(Loader.loaded);

    pos += spacing;
  }
}
