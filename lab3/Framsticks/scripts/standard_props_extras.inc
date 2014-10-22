# standard evolution parameters - Extras group

prop:
id:stagnation
name:Stagnation (auto stop)
type:d 0 1000000 0
help:No improvement period required to stop simulation (stagnation). 0 disables automatic stopping.
group:Extras

prop:
id:minfitness
name:Minimal fitness
type:f
help:Minimal fitness that allows to automatically stop evolution when stagnation detected
group:Extras

prop:
id:boostphase
name:Boost phase after stagnation
type:d 0 1 1
help:~
After stagnation has been detected, switches negative selection to "delete worst", doubles "multiple evaluation" and starts another stagnation detection phase.
"Delete worst" results in extremely quick convergence and high selection pressure, similarly to "local search" optimization techniques.~
group:Extras

prop:
id:makesound
name:Sound on improvement
type:d 0 1 0
help:Emit sounds on improvements? (frequency depends on the magnitude of improvement)
group:Extras

prop:
id:savebest
name:Save improvements
type:d 0 3 ~Off~Best genotype only~All improving genotypes~Complete experiment state with best genotype
help:Saves (on each improvement) best found genotype or a complete experiment state to a file in scripts_output subdirectory.
group:Extras

prop:
id:log
name:Log fitness
type:d 0 5 ~Off~On improvement~Every evaluation~Every 10 evaluations~Every 100 evaluations~Every 1000 evaluations
help:~
Sends [LOG] messages with genotypes count and minimal, average and best gene pool fitness, which can be used to produce graphs by external tools like gnuplot or Excel.
It also sends the [LOGTITLE] message on experiment initialization, which summarizes most important parameters of your experiment. It can be used as a graph title.~
group:Extras
