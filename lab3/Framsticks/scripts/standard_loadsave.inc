//Typical load/save handling (intended for files with *.expt extension)

global expLoadGenePoolCounter, expLoadPopulationCounter; //only used when reading expt files

function onExpLoad()
{
  var i;
  for (i = 0;i < GenePools.size;i++)
    GenePools.clearGroup(i);
  for (i = 0;i < Populations.size;i++)
    Populations.clearGroup(i);

  expLoadGenePoolCounter = -1; //will be incremented on read
  expLoadPopulationCounter = -1; //will be incremented on read

  Loader.addClass(sim_params.*);
  Loader.addClass(Population.*);
  Loader.addClass(GenePool.*);
  Loader.setBreakLabel(Loader.OnComment, "onExpLoad_Comment");
  Loader.setBreakLabel(Loader.BeforeObject, "onExpLoad_Object");
  Loader.setBreakLabel(Loader.BeforeUnknown, "onExpLoad_Unknown");
  Loader.run();

  if (expLoadGenePoolCounter < GenePools.size - 1)
    Simulator.message("onExpLoad(): not enough gene pool settings in input file: found " + (expLoadGenePoolCounter + 1) + ", expected " + GenePools.size, 1);
  if (expLoadPopulationCounter < Populations.size - 1)
    Simulator.message("onExpLoad(): not enough population settings in input file: found " + (expLoadPopulationCounter + 1) + ", expected " + Populations.size, 1);

  var loaded = 0;
  for (i = 0;i < GenePools.size;i++)
    loaded += GenePools[i].size;
  if (loaded > 0)
    Simulator.print("Experiment loaded (" + loaded + " genotypes)");
}

function loadOrSkipObject()
{
  // return 1=>obj should be loaded, 0=>skip this obj, -1=>do nothing (already handled)

  switch (Loader.objectName)
  {
  case "Population":
    expLoadPopulationCounter++;
    if (expLoadPopulationCounter >= Populations.size)
    {
      Simulator.message("onExpLoad(): too many populations in input file: expected " + Populations.size, 2);
      return 0;
    }
    else
    {
      Populations.group = expLoadPopulationCounter;
      Loader.currentObject = Population.*;
      return 1;
    }

  case "GenePool":
    expLoadGenePoolCounter++;
    if (expLoadGenePoolCounter >= GenePools.size)
    {
      Simulator.message("onExpLoad(): too many gene pools in input file: expected " + GenePools.size, 2);
      return 0;
    }
    else
    {
      GenePools.group = expLoadGenePoolCounter;
      Loader.currentObject = GenePool.*;
      return 1;
    }
  }

  return -1;
}

function onExpLoad_Unknown()
{
  switch (Loader.objectName)
  {
  case "org":
    GenePools.genotype = -1;
    Loader.currentObject = Genotype.*;
    Interface.makeFrom(Genotype.*).setAllDefault();
    Loader.loadObject();
    if (expLoadGenePoolCounter >= GenePools.size)
      Simulator.message("onExpLoad(): skipping genotype " + Genotype.name + " belonging to superfluous gene pool", 2);
    else
      GenePools.copySelected(GenePools.group);
    break;

  case "CreaturesGroup":
    Loader.objectName = "Population";
    Simulator.message("'CreaturesGroup' found in experiment settings file, please change to 'Population'", 1);
    if (loadOrSkipObject() == 1) //for unknown the default action is to skip, so we force load when == 1
      Loader.loadObject();
    break;

  case "GenotypeGroup":
    Loader.objectName = "GenePool";
    Simulator.message("'GenotypeGroup' found in experiment settings file, please change to 'GenePool'", 1);
    if (loadOrSkipObject() == 1) //for unknown the default action is to skip, so we force load when == 1
      Loader.loadObject();
    break;
  }
}

function onExpLoad_Object()
{
  if (loadOrSkipObject() == 0) //for known object the default action is to load, so we force skip when == 0
    Loader.skipObject();
}

function onExpSave()
{
  var i, j;
  var saved_genotypes = 0;

  File.writeComment("saved by 'standard_loadsave.inc'");
  File.writeObject(sim_params.*);

  for (i = 0;i < GenePools.size;i++)
  {
    GenePools.group = i;
    File.writeObject(GenePool.*);
    for (j = 0;j < GenePool.size;j++)
    {
      GenePools.genotype = j;
      File.writeNameObject("org", Genotype.*);
      saved_genotypes++;
    }
  }

  for (i = 0;i < Populations.size;i++)
  {
    Populations.group = i;
    File.writeObject(Population.*);
  }

  Simulator.print("Experiment saved (" + saved_genotypes + " genotypes)");
}

