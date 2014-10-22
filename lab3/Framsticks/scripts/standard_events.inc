// standard user events handling (user can place food manually)
//
// to be used by experiment definitions supporting "food" group.
// requires the addfood function (should be implemented in the experiment definition).

function queryEventNames()
{
  return ["Place food", "Drop food"];
}

function onUserEvent(type, point, vector)
{
  //Simulator.print("called event #"+type);
  var p = WorldMap.intersect(point, vector);
  if (p)
  {
    //	Simulator.print("intersection found:"+p[0]+","+p[1]+","+p[2]);
    addfood();
    var z = p[2];
    if (type == 1)
      z += 10;
    Creature.moveAbs(p[0] - Creature.size_x / 2, p[1] - Creature.size_y / 2, z);
  }
  //else
  //	Simulator.print("no intersection...");
}
