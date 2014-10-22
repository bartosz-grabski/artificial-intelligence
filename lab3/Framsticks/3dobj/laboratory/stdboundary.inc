// Teleport is built by script instead of loading an object from file


//builds UV 'wall' from point (x1,y1, -0.2) to (x2, y2, -0.2) with height 2
function buildUVLight(x1, x2, y1, y2)
{
  var z = -0.2;
  var w = z + 2;

  Material.new();
  Material.setFlat();
  Material.enable(GL.TEXTURE_2D);
  Material.texture = "uv.png";
  Material.translucent = 1;

  //inside
  GeomBuilder.addBranch(GeomBuilder.rootNode);

  VertexBuilder.newQuads();
  VertexBuilder.addVertex([x1, y1, z, x2, y2, z,
                           x2, y2, w, x1, y1, w]);


  VertexBuilder.addColor(0.9, 0.9, 0.9);
  VertexBuilder.material = Material.object;
  VertexBuilder.genTexCoordsYZ(0.1, 0.5);
  GeomBuilder.add(VertexBuilder.node);

  //outside
  GeomBuilder.addBranch(GeomBuilder.rootNode);

  VertexBuilder.newQuads();
  VertexBuilder.addVertex([ x1, y1, w, x2, y2, w,
                            x2, y2, z, x1, y1, z ]);


  VertexBuilder.addColor(0.9, 0.9, 0.9);
  VertexBuilder.material = Material.object;
  VertexBuilder.genTexCoordsYZ(0.1, 0.5);
  GeomBuilder.add(VertexBuilder.node);


}

// builds boundaries and adds extra elements:
// 	- flasks with no boundary and with fence
//	- jar with teleport
// teleport is made from UV 'walls'
function buildWorldBoundary()
{
  var spacing;
  switch (World.wrldbnd)
  {
  case 0:

    loadExtraElement("flask2.ac");
    return ;
  case 1:

    loadExtraElement("flask.ac");


    Loader.load("fence-element.ac");
    spacing = 1.35;
    break;
  case 2:

    loadExtraElement("jar.ac");

    var s = World.wrldsiz;
    buildUVLight(0, 0, 0, s);
    buildUVLight(s, s, 0, s);
    buildUVLight(s, 0, 0, 0);
    buildUVLight(s, 0, s, s);

    return ;
  }

  if (Loader.loaded == 0)
    return ;
  GeomBuilder.addBranch(GeomBuilder.rootNode);
  var node = GeomBuilder.currentNode;
  var pos = 0.0;
  var z = -0.2;
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
