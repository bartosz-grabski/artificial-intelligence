# evolution demo parameters

prop:
id:initialgen
name:Initial genotype
type:s 1
help:~
Initial genotype, mutated to create initial population of genotypes.
You need to (re)initialize the experiment to (re)create initial population of genotypes.~

prop:
id:delrule
name:Delete genotypes
type:d 0 2 ~Randomly~Inverse-proportionally to fitness~Only the worst

prop:
id:MaxCreated
name:Simulated creatures
type:d 0 50
help:You need to (re)initialize the experiment to change this setting and create initial population of given size.

prop:
id:rotation
name:Initial orientation
type:d 0 4 ~Always 0 degrees~Randomized 180 degrees~Randomized 90 degrees~Randomized 45 degrees~Random
help:Initial heading of newborn creatures

prop:
id:creath
name:Initial elevation
type:f -1 50
help:~
Vertical position (above the surface) where newborn creatures are placed.
Negative values are only used in the water area:
  0   = at the surface
-0.5 = half depth
-1   = just above the bottom~

prop:
id:predefinedSetup
name:Predefined setup
type:d 0 2 ~Vertical position optimization~Velocity optimization~Body size optimization

prop:
id:lifeLengthInSteps
name:Number of steps each creature lives
type:d
help:~
Number of simulation steps for the creature in the center of cratures circle.
Beware to performance sampling period when setting this value.~

prop:
id:numberOfIdleSteps
name:Number of idle visualization steps
type:d
help:~
Number of idle visualization steps:
- after parent (parents) approaches (approach) the center and before new creature is born in the world
- after new creature is born in the world and before parent (parents) returns (return) to original place (places)
- after central creature disappearance
- after replacement of circle circumference creature by central creature
If this parameter is set to zero, than simulation speeds up.
If this parameter is set to big value, then some simulation details may be better observed.
It is possible to change this value during simulation, to obtain different goals.
~

prop:
id:p_nop
name:Unchanged
type:f 0 100
group:Selection

prop:
id:p_mut
name:Mutated
type:f 0 100
group:Selection

prop:
id:p_xov
name:Crossed over
type:f 0 100
group:Selection

prop:
id:xov_mins
name:Minimal similarity
type:f 0 9999
help:~
Only genotypes with dissimilarity below this threshold will be crossed over.
Value of 0 means no crossover restrictions.~
group:Selection

prop:
id:selrule
name:Selection rule
type:~
d 0 5 \~Random\~Fitness-proportional (roulette)\~Tournament (2 genotypes)\~Tournament (3 genotypes)\~Tournament (4 genotypes)\~Tournament (5 genotypes)~
group:Selection

prop:
id:cr_c
name:Constant
type:f -10000 10000
help:Constant value added to total fitness
group:Fitness

prop:
id:cr_vpos
name:Vertical position
type:f -10000 10000
group:Fitness

prop:
id:cr_v
name:Velocity
type:f -10000 10000
help:Weight of horizontal velocity in total fitness
group:Fitness

prop:
id:cr_gl
name:Body parts
type:f -10000 10000
help:Weight of body size (number of parts) in total fitness
group:Fitness

prop:
id:cr_norm
name:Criteria normalization
type:d 0 1
help:Normalize each criterion to 0..1 interval before weighting
group:Fitness

prop:
id:cr_simi
name:Similarity speciation
type:d 0 1
help:If enabled, fitness of each genotype will be reduced by its phenotypic similarity to all other genotypes in the gene pool
group:Fitness

state:
id:totaltestedcr
name:Evaluated creatures
type:d
help:Total number of creatures evaluated in the experiment
flags:16

state:
id:stepType
name:Type of step
type:d
flags:32

state:
id:stepsCounter
name:Resetable steps counter
type:d
help:Number of executed steps of given type
flags:32

state:
id:globals
name:Global variables names and values pairs
type:s
help:String of | separated global variables (name=value) pairs
flags:32

state:
id:currentGenotype
name:Index of current genotype in GenePools
type:d
help:Index of current genotype in GenePools
flags:32

state:
id:currentCreature
name:Index of current creature in Populations
type:d
help:Index of current creature in Populations
flags:32
