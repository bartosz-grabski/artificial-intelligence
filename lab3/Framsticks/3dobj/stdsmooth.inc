// creates the heightfield environment geometry

function buildSmoothWorld()
{
//  Simulator.print("building smoothworld geometry...");
  var dx, dy, mx, my, x, y;

  dx = World.wrldsiz / (WorldMap.xsize-1);
  dy = World.wrldsiz / (WorldMap.ysize-1);

      Material.new();
      Material.setSmooth();
      Material.disable(GL.BLEND);
      Material.enable(GL.TEXTURE_2D);
      Material.enable(GL.LIGHTING);
      Material.enable(GL.COLOR_MATERIAL);
      Material.colormat=GL.AMBIENT_AND_DIFFUSE;
      Material.specular=0;
      Material.texture = "niceground.png";

  y = 0.0;
  for (my = 1;my < WorldMap.ysize;my++)
  {
    x = 0.0;
    for (mx = 1;mx < WorldMap.xsize;mx++)
    {
      VertexBuilder.newTriangleStrip();
      addSmoothPoint(mx - 1, my, x, y + dy);
      addSmoothPoint(mx - 1, my - 1, x, y);
      addSmoothPoint(mx, my, x + dx, y + dy);
      addSmoothPoint(mx, my - 1, x + dx, y);
      VertexBuilder.genTexCoordsXY(0.1, 0.1);
      GeomBuilder.addBranch(GeomBuilder.rootNode);
      VertexBuilder.material = Material.object;
      GeomBuilder.add(VertexBuilder.node);
      x += dx;
    }
    y += dy;
  }

  buildWorldBoundary();
  buildWorldWater();
}

function addSmoothPoint(mapx, mapy, x, y)
{
  var nz = World.wrldsiz/(WorldMap.xsize+WorldMap.ysize);
  var z = WorldMap.getMap(mapx, mapy).z;
  if (World.simtype==0) z-=0.2;
  VertexBuilder.addVertex(x, y, z);
  addHeightColor(z);
  // calculate normal vector (approximate)
  var xm1 = WorldMap.getMap(mapx - 1, mapy).z;
  var xp1 = WorldMap.getMap(mapx + 1, mapy).z;
  var ym1 = WorldMap.getMap(mapx, mapy - 1).z;
  var yp1 = WorldMap.getMap(mapx, mapy + 1).z;
  VertexBuilder.addNormal((xm1 - xp1) / World.wrldsiz*WorldMap.xsize,
			  (ym1 - yp1) / World.wrldsiz*WorldMap.ysize, nz);
}

function addHeightColor(z)
{VertexBuilder.addColor(1.0,(z/9.0)+0.5,0.0);}
