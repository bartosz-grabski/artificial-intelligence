// creates the "blocks" environment geometry

function buildBlockWorld()
{
//  Simulator.print("building blockworld geometry...");
  var xs = WorldMap.xsize - 2, ys = WorldMap.ysize - 2;
  var blockx = World.wrldsiz / xs, blocky = World.wrldsiz / ys;
  var mx, my, x, y;
  y = 0.0;
  Material.new();
  Material.setFlat();
  Material.enable(GL.TEXTURE_2D);
  Material.enable(GL.LIGHTING);
  Material.disable(GL.BLEND);
  Material.enable(GL.COLOR_MATERIAL);
  Material.colormat=GL.AMBIENT_AND_DIFFUSE;
  Material.specular=0;
  Material.texture = "niceground.png";
  for (my = 1;my <= ys;my++)
  {
    x = 0.0;
    for (mx = 1;mx <= xs;mx++)
    {
      addBlock(x, y, blockx, blocky, mx, my);
      x += blockx;
    }
    y += blocky;
  }
  buildWorldBoundary();
  buildWorldWater();
}

function addBlock(x1, y1, xsize, ysize, mx, my)
{
  var z, z0, zx, zy, zxy, zy0, zy1, zx0, zx1, n1, nn;
  var shift_z = -0.2;
  if (World.simtype) shift_z=0.0;
  z = WorldMap.getMap(mx, my).z + shift_z;

  GeomBuilder.addBranch(GeomBuilder.rootNode);
  VertexBuilder.newQuads();

  switch (WorldMap.getMap(mx, my).type)
  {
  case 0:
    z0 = z;
    zx = z;
    zy = z;
    zxy = z;
    VertexBuilder.addNormal(0, 0, 1);
    break;
  case 1:
    z0 = WorldMap.getMap(mx - 1, my).z + shift_z;
    zy = z0;
    zx = WorldMap.getMap(mx + 1, my).z + shift_z;
    zxy = zx;
    n1 = z0 - zx;
    nn = Math.sqrt(n1 * n1 + xsize * xsize);
    VertexBuilder.addNormal(n1 / nn, 0.0, xsize / nn);
    break;
  case 2:
    z0 = WorldMap.getMap(mx, my - 1).z + shift_z;
    zx = z0;
    zy = WorldMap.getMap(mx, my + 1).z + shift_z;
    zxy = zy;
    n1 = z0 - zy;
    nn = Math.sqrt(n1 * n1 + ysize * ysize);
    VertexBuilder.addNormal(0.0, n1 / nn, ysize / nn);
    break;
  }
  var x2 = x1 + xsize, y2 = y1 + ysize;
  VertexBuilder.addVertex([x1, y1, z0, x2, y1, zx, x2, y2, zxy, x1, y2, zy]);
  VertexBuilder.addColor(1.0, (z / 9.0) + 0.5, 0.0);
  VertexBuilder.material = Material.object;
  VertexBuilder.genTexCoordsXY(0.1, 0.1);
  GeomBuilder.add(VertexBuilder.node);

  switch (WorldMap.getMap(mx, my - 1).type)
  {
  case 0:
    zy0 = WorldMap.getMap(mx, my - 1).z + shift_z;
    zy1 = zy0;
    break;
  case 1:
    zy0 = WorldMap.getMap(mx - 1, my - 1).z + shift_z;
    zy1 = WorldMap.getMap(mx + 1, my - 1).z + shift_z;
    break;
  case 2:
    zy0 = z0;
    zy1 = z0;
    break;
  }
  if ((z0 != zy0) || (zx != zy1))
  {
    //Simulator.print("zy:"+z0+" "+zx+" "+zy1+" "+zy0);
    VertexBuilder.newQuads();
    VertexBuilder.addVertex([x2, y1, zx, x1, y1, z0, x1, y1, zy0, x2, y1, zy1]);
    if ((z0 + zx) > (zy0 + zy1))
      VertexBuilder.addNormal(0, -1, 0);
    else
      VertexBuilder.addNormal(0, 1, 0);
    VertexBuilder.addColor(0.8, 0.8, 0.8);
    VertexBuilder.material = Material.object;
    VertexBuilder.genTexCoordsXZ(0.03, 0.03);
    GeomBuilder.add(VertexBuilder.node);
  }

  switch (WorldMap.getMap(mx - 1, my).type)
  {
  case 0:
    zx0 = WorldMap.getMap(mx - 1, my).z + shift_z;
    zx1 = zx0;
    break;
  case 1:
    zx0 = z0;
    zx1 = z0;
    break;
  case 2:
    zx0 = WorldMap.getMap(mx - 1, my - 1).z + shift_z;
    zx1 = WorldMap.getMap(mx - 1, my + 1).z + shift_z;
    break;
  }
  if ((z0 != zx0) || (zy != zx1))
  {
    //Simulator.print("zx:"+z0+" "+zx+" "+zx1+" "+zx0);
    VertexBuilder.newQuads();
    VertexBuilder.addVertex([x1, y1, z0, x1, y2, zy, x1, y2, zx1, x1, y1, zx0]);
    if ((z0 + zy) > (zx0 + zx1))
      VertexBuilder.addNormal( -1, 0, 0);
    else
      VertexBuilder.addNormal(1, 0, 0);
    VertexBuilder.addColor(0.8, 0.8, 0.8);
    VertexBuilder.material = Material.object;
    VertexBuilder.genTexCoordsYZ(0.03, 0.03);
    GeomBuilder.add(VertexBuilder.node);
  }
}
