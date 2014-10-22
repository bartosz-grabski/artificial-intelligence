function dumpCreatures(direct)
{
  Populations.group = 0;
  var i, j, f;
  if (direct!=0)
  	  f = File.createDirect("dump.txt", "Coordinates, velocities and stress (linear and rotational).");
  else
  	  f = File.create("dump.txt", "Coordinates, velocities and stress (linear and rotational).");
  f.writeComment("parts are points, joints are sticks.");
  f.writeComment("format for parts: x y z vx vy vz");
  f.writeComment("format for joints: part1index(0-based)  part2index(0-based)  stresslinear  stressrotational");
  for (i = 0;i < Population.size;i++)
  {
    Populations.creature = i;
    f.writeString("\n# creature_number  number_of_parts  number_of_joints\n");
    f.writeString(""+i+" "+Creature.numparts+" "+Creature.numjoints+"\n");
    for (j = 0;j < Creature.numparts;j++)
    {
      var part = Creature.getMechPart(j);
      f.writeString(""+part.x+" "+part.y+" "+part.z+" "+part.vx+" "+part.vy+" "+part.vz+"\n");
    }
    var model=Creature.model;
    for (j = 0;j < Creature.numjoints;j++)
    {
      var joint = Creature.getMechJoint(j);
      var modeljoint=model.getJoint(j);
      f.writeString(""+modeljoint.p1+" "+modeljoint.p2+" "+joint.stress+" "+joint.rotstress+"\n");
    }
  }
  f.close();
}
