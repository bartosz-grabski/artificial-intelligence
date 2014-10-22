# standard evolution parameters

prop:
id:initialgen
name:Initial genotype
help:~
The gene pool will be replaced with the supplied genotype when the experiment begins.
Use the empty initial genotype if you want to preserve the current gene pool.~
type:s 1

prop:
id:capacity
name:Gene pool capacity
type:d 0 10000

prop:
id:MaxCreated
name:Simulated creatures
type:d 0 50

prop:
id:placement
name:Initial placement
type:d 0 1 ~Random~Central
help:For 'Central' placement, newborn creatures are placed at the world center, if possible.

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
id:p_nop
name:Unchanged
type:f 0 100
group:Selection

prop:
id:evalcount
name:Multiple evaluation
type:d 0 1000 0
group:Selection
help:~
If more than zero:
- each genotype will be evaluated many times
- fitness will be averaged
- fitness standard deviation will be stored in the 'user1' field of a genotype
- there will be no "Unchanged" genotypes ("Unchanged" value is considered zero).~

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
help:Positive selection: how to choose genotypes for cloning/mutation/crossover
group:Selection

prop:
id:delrule
name:Delete genotypes
type:d 0 2 ~Randomly~Inverse-proportionally to fitness~Only the worst
help:Negative selection: which genotypes should be removed when we need room for new genotypes in the gene pool
group:Selection

prop:
id:cr_c
name:Constant
type:f -10000 10000
help:Constant value added to total fitness
group:Fitness

prop:
id:cr_life
name:Life span
type:f -10000 10000
help:Weight of life span in total fitness
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
id:cr_joints
name:Body joints
type:f -10000 10000
help:Weight of structure size (number of joints) in total fitness
group:Fitness

prop:
id:cr_nnsiz
name:Brain neurons
type:f -10000 10000
help:Weight of brain size (number of neurons) in total fitness
group:Fitness

prop:
id:cr_nncon
name:Brain connections
type:f -10000 10000
help:Weight of brain connections in total fitness
group:Fitness

prop:
id:cr_di
name:Distance
type:f -10000 10000
help:Weight of distance in total fitness
group:Fitness

prop:
id:cr_vpos
name:Vertical position
type:f -10000 10000
help:Weight of vertical position in total fitness
group:Fitness

prop:
id:cr_vvel
name:Vertical velocity
type:f -10000 10000
help:Weight of vertical velocity in total fitness
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

prop:
id:Energy0
name:Starting energy
type:f 0 10000
group:Energy
help:Base starting energy level (for each stick)

prop:
id:e_meta
name:Idle metabolism
type:f 0 1
group:Energy
help:Each stick consumes this amount of energy in one time step

prop:
id:feed
name:Automatic feeding
type:d 0 100
group:Energy
help:Number of energy pieces in the world

prop:
id:feede0
name:Food's energy
group:Energy
type:f 0 1000

prop:
id:foodgen
name:Food's genotype
group:Energy
type:s 1
help:~
The default food model is a simple, single part object:
  //0
  m:Vstyle=food
  p:
(this genotype is used when you leave this field blank).
You can specify another genotype to create "intelligent" or mobile food.~

prop:
id:feedtrans
name:Ingestion multiplier
group:Energy
type:f 0 100

prop:
id:aging
name:Aging time
group:Energy
type:d 0 100000
help:Idle metabolism doubles after this period (0 disables aging)

