//standard placement function

function place_randomly(centered,rotation)
{
  var x, y, z;
  if (rotation>0)
  {
    var angle;
    if (rotation==4) angle=Math.rndUni(0,Math.twopi); else
                     angle=[Math.pi,Math.pi2,Math.pi4][rotation-1]*Math.random(8);
    Creature.rotate(0,0,angle);
  }
  if (centered == 0) //random x,y
  {
    x = (World.wrldsiz - Creature.size_x) * Math.rnd01;
    y = (World.wrldsiz - Creature.size_y) * Math.rnd01;
  } else          //centered position
  {
    x = (World.wrldsiz - Creature.size_x) / 2;
    y = (World.wrldsiz - Creature.size_y) / 2;
  }
  z = WorldMap.getHeight(x, y);
  if (World.wrldwat > z) // water here!
  {
    if (ExpParams.creath > 0)
      z = World.wrldwat + ExpParams.creath;
    else
      z = (1.0 + ExpParams.creath) * World.wrldwat - z * ExpParams.creath;
  }
  else                  // land area
  {
    if (ExpParams.creath > 0)
      z += ExpParams.creath;
  }

  if (World.simtype==1) z+=0.2;
  Creature.moveAbs(x, y, z - 0.999); //assumes that the size of the lowest Part is 1
  return ;
}
