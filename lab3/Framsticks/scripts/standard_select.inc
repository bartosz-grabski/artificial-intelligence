//standard selection methods

function selectedForDeletion()
{
  if (ExpParams.delrule == 0)
    return GenePools.random();
  if (ExpParams.delrule == 1)
    return GenePools.revroulette();
  return GenePools.worst();
}

function selectedForCreation()
{
  if (ExpParams.selrule == 0)
    return GenePools.random();
  if (ExpParams.selrule == 1)
    return GenePools.roulette();
  return GenePools.tournament(ExpParams.selrule);
}

function selectedSimilar()
{
  if (ExpParams.selrule == 0)
    return GenePools.randomLikeThis(ExpParams.xov_mins);
  else
    return GenePools.likeThisRoulette(ExpParams.xov_mins);
}
