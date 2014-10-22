#This script lists classes and functions renamed in Framsticks 3.x.
#This is only provided for advanced users who need to update their own scripts.

cp "$1" /tmp/renamebackup
sed <"$1" >/tmp/tmprename -e "s/\.creaturecount/.size/g" -e "s/GenotypeLibrary.groupcount/GenePools.size/g" -e "s/LiveLibrary.groupcount/Populations.size/g" -e "s/GenotypeLibrary.copyGenotype(/GenePools.copySelected(/g" -e "s/GenotypeLibrary.delGenotype(/GenePools.deleteSelected(/g" -e "s/LiveLibrary.delete(/Populations.deleteSelected(/g" -e "s/LiveLibrary.kill(/Populations.killSelected(/g" -e "s/Library.getGroup/Library.get/g" -e "s/LiveLibrary/Populations/g" -e "s/GenotypeLibrary/GenePools/g" -e "s/CreaturesGroup/Population/g" -e "s/GenotypeGroup/GenePool/g" -e "s/.getGenotype(/.get(/g" -e "s/.getCreature(/.get(/g" -e "s/GenePools.get\(.*\).count/GenePools.get\1.size/g" -e "s/GenePools.mutate(/GenePools.mutateSelected(/g" -e "s/GenePools.crossover(/GenePools.crossoverSelected(/g" -e "s/GenePool.count/GenePool.size/g"
if diff "$1" /tmp/tmprename >/dev/null; then echo "$1: no diff"; else echo "$1: diff!"; cp /tmp/tmprename "$1"; fi
