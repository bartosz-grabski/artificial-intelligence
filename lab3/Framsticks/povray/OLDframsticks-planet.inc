//
// framsticks.inc, planet setup, requires updating
//

#include "colors.inc"
#include "textures.inc"
#include "woods.inc"

#debug concat("Animation Frame #",str(AnimFrame,5,0),"\n")

//////////////////////////////////

#declare StandardCamera=camera { up 1.15*<0,0,1> right 1.15*<4/3,0,0> sky <0,0,1>
location <0,0,0> look_at <1,1,1> /*angle 100 orthographic*/ }

//////////////////////////////////

#declare GlobalAmbient=5; //oryg. 3

global_settings
 {
 max_trace_level 100
 adc_bailout 0.1
 ambient_light GlobalAmbient
 assumed_gamma 2.2
 }

///////////
// colours
///////////////////////////////////////////
// stick:
#declare OldStickRGB = <0.65,0.65,0.55>;
#declare OldStick2RGB = <0.55,0.55,0.45>;
#declare StickRGB = <1,0.1,0.1>;
#declare StaminaRGB = <1,0.9,0.7>;
#declare IngestRGB = <1,1,.7>;
#declare AsimilRGB = <1,1,0.14>;

/////////////////////
// muscle
#declare Muscle1RGB=<1,0.6,0.6>;
#declare Muscle2RGB=<0.9,0.3,0.3>;
#declare Muscle3RGB=<1,0.5,0.5>;

//////////////////////////
// fog:
#declare WaterFogColor = rgbf<0.2, 0.3, 0.6, 0.3>;
#declare WaterFogDistance = 60;

//{  distance 60   colour rgb<0.6, 0.8, 0.75> } //dist 16 ok

///////////////////////////////

#declare SimpleStickTexture = texture
	{ pigment { color Red }
	  finish { phong 1 }
   }

///////////////////////////////
// world:

#declare WORLD_FLAT=0;
#declare WORLD_BLOCKS=1;

#declare BORDER_NONE=0;
#declare BORDER_FENCE=1;
#declare BORDER_TELEPORT=2;

#declare OFFSET_Z=-0.2;

#declare WallTexture = texture { pigment {granite} }

#declare Ground0Texture = texture 
{
   pigment { 
   color rgb <0.29, 0.29, 0.31> }
finish {
    ambient 1.0/GlobalAmbient
    brilliance 3
    diffuse 0.9 
//    metallic
//    specular 0.70
//    roughness 1/60
    reflection 0
}
// normal {bozo 1 scale 3}
 normal {granite scale 10  }
 }

/*
texture
	{
	pigment             
// put this in your scene or world.inc file:
// #declare SimpleGround=yes
//
#ifdef (SimpleGround)
   {color rgb <1,0.9,0.6>} //yellow sand
//   {color rgb <1,1,1>}      //white
//  finish {ambient 1 diffuse 0 } //no shadows
#else
	 {
	 granite
		color_map
			{
			[ 0.0 rgb <0.95, 0.90, 0.60> ]
			[ 0.7 rgb <0.76, 0.70, 0.40> ]
			[ 1.0 rgb <0.76, 0.70, 0.40> ]
			}
	 scale 3
	 }
    normal {bumps 0.01 scale 0.02}
#end
	}*/

/////////////////////////////////

#declare UnderwaterTexture=texture
{
	pigment
    {
	 granite
	color_map
      { [ 0 color Gray60 ]
		  [ 1 color White ] }
	scale 2
    }
  finish {ambient 0.7/GlobalAmbient diffuse 0.3 }
}

/////////////////////////////////

#declare HighTexture=texture
{
   pigment { color rgb <0.33, 0.33, 0.36> } 
finish {
    ambient 1.0/GlobalAmbient
    brilliance 3
    diffuse 0.9 //.4
    metallic
//    specular 0.70
 //   roughness 1/60
    reflection 0.1
}
 //normal {bozo 1 scale 3}
 normal {hexagon
       normal { gradient x scale .7 },
       normal { gradient y scale .7 }
//       normal { bumps 4 scale 1 }
 }
}


///////////////////////////////////


#declare WaterTexture = texture
{
 pigment { red 0.6 green 0.6 blue 1.0 filter 0.9 }
 finish { reflection 0.1 }
 normal { bumps 0.5 scale 2 }
}
#declare WaterInterior = interior
{ ior 1.1 caustics 0.3 }

#declare WaterWaveSpeed = <0,0,0.2>;

///////////////////////////////////////////////
// LIGHT SOURCE

#macro StandardLight()
light_source
 { <0,0,0> color rgb <1.5,1.5,1.5>
 looks_like
    {
    union
    {
    sphere { <0,0,0>,600
        texture { pigment { color rgb <1,1,1> }
                  finish { ambient 1/GlobalAmbient diffuse 0 }
                }
        }
    disc { <0,0,0>,CameraLocation-<2000,-1000,3000>,1
        texture { pigment { onion color_map { [0.2 rgbt <1,1,1,0>][1 rgbt <0.7,0.7,0.7,1>] } }
                  finish { ambient 1/GlobalAmbient diffuse 0 }
                }
         scale 2000
         }
    }
    }  
 translate <2000, -1000, 3000>
 }
#end

// An area light (creates soft shadows)
// WARNING: This special light can significantly slow down rendering times!
/*
light_source
{
  0*x // light's position (translated below)
  color rgb 0.7  // light's color
  // <widthVector> <heightVector> nLightsWide mLightsHigh
  area_light
  <8, 0, 0> <0, 0, 8> // lights spread out across this distance (x * z)
  4, 4                // total number of lights in grid (4x*4z = 16 lights)
  adaptive 0          // 0,1,2,3...
  jitter              // adds random softening of light
  translate <10, 10, -40>   // <x y z> position of light
}
*/

//////////////////////////////////////////////////////////
// SKY

#declare planet=sphere
{
 <0,0,0>,1
   texture {
          normal { bumps 0.5 scale 0.1}
          pigment { rgb <.7,.7,.7> }
          finish { ambient 0 diffuse 1 }
          } 
}

#declare planetring=disc
{ <0,0,0>,z,1,0.6
   texture {
        pigment { onion color_map
         {
         [ 0 color rgbt <0,0,0,1> ]
         [ 0.60 color rgbt <1,1,1,1> ]
         [ 0.63 color rgbt <1,1,1,0.2> ]
         [ 0.72 color rgbt <1,1,1,0.5> ]
         [ 0.79 color rgbt <1,1,1,0.2> ]
         [ 0.80 color rgbt <1,1,1,0.8> ]
         [ 0.85 color rgbt <1,1,1,0> ]
         [ 0.88 color rgbt <1,1,1,0.3> ]
         [ 0.90 color rgbt <1,1,1,1> ]
         [ 0.94 color rgbt <1,1,1,1> ]
         [ 0.98 color rgbt <1,1,1,0.3> ]
         [ 1 color rgbt <1,1,1,1> ]
         }
          }
        finish { ambient 0 diffuse 1 }
          }
}

#declare saturn=union
{
object {planet scale 0.4}
object {planetring}
}

#macro StandardSky()
/*
sky_sphere {
    pigment {
        gradient y
        pigment_map {
            [0.00 color rgb <0.5,0,.6> ]
            [0.17 color rgb <0,0,.4> ]
            [0.35 color rgb <0,0,0>  ]
        }
    }
 rotate <90,0,0>
*/
sky_sphere { pigment { color rgb <0,0,0> } }

sphere {
    <0,0,0>,1     hollow
    pigment {
        gradient y
        pigment_map {
            [0.00 color rgbt <0.5,0,1,0.6> ]
            [0.12 color rgbt <0,0,1,0.6> ]
            [0.5 color rgbt <0,0,1,1>  ]
            }         
    }
 finish { ambient 1/GlobalAmbient diffuse 0 }        
 rotate <90,0,0>
 scale 2000
}

#local Seed=seed(123);
#local N=10000;
#while (N>0)
  #local X=rand(Seed)*180-90;
  #local Y=rand(Seed)*180-90;
  sphere { <0,0,4*12000> 5+(rand(Seed)+rand(Seed))*40
        texture { pigment {
                color rgb<rand(Seed)*0.2+0.8,
                        rand(Seed)*0.2+0.8,
                        rand(Seed)*0.2+0.8>}
                  finish { ambient 1/GlobalAmbient diffuse 0 } 
                }
        rotate <X,0,Y>
        }
  #local N=N-1;
#end 

object { saturn scale 1100 rotate <-20,-25,-10> translate <-1000,-3000,360> }
/*
object { planet scale 400 translate <1700,-900,900> }
object { planet scale 200 translate <-1000,-2000,800> }
object { planet scale 300 translate <0,2800,700> }
object { planet scale 100 translate <600,1800,1100> }
object { planet scale 100 translate <-500,2800,300> }
object { saturn scale 3500 rotate <-50,-170,0> translate <6000,3000,400> }
*/

#end

//background { color SkyBlue }

///////////////////////////////////////////////
// MANIPULATOR

#declare ManipulatorArm = union
 {
 cylinder { <0,0,0>,<0,0,10>,0.2 }
 sphere { <0,0,0>,0.3 }
 }
#declare ManipulatorFinger = union
 {
 cylinder { <0,0,0>,<-1,0,-1>,0.17 }
 sphere { <-1,0,-1>,0.17 }
 cone { <-1,0,-1>,0.17,<0,0,-2>,0.1 }
 sphere { <0,0,-2>,0.1 }
 }
#declare ManipulatorTexture=Aluminum

#macro Manipulator(Location,State)
union
 {
 object {ManipulatorArm}
 object {ManipulatorFinger rotate <0,5+25*abs(State),0>}
 object {ManipulatorFinger rotate <0,5+25*abs(State),180>}
 translate <0,0,2>
 texture {ManipulatorTexture}
 scale 0.5
 rotate <0,0,180>*abs(State)
 translate Location+<0,0,10>*abs(State)
 }
#end

///////////////////////////////////////////////
// FENCE

#declare FenceSegmentLen = 5;
#declare FenceHeight = 4;
#declare FenceBeamSpacing = 1;
#declare FenceBeamHeight = 1;
#declare FenceBeamsNum = 3;

#declare FenceTexture = texture { T_Wood17 scale 0.6 }

// FENCE

#declare FenceBeam = object
{
cylinder { <-0.4-FenceSegmentLen/2,0,0>,<FenceSegmentLen/2+0.4,0,0>,0.15
 texture {FenceTexture rotate <0,90,0> }}
}
#declare FencePost = union
{
box { <-0.2,-0.2,0>,<0.2,0.2,FenceHeight> }
  union
  {
  triangle { <-0.2,-0.2,0>,<-0.2,0.2,0>,<0,0,0.2> }
  triangle { <-0.2, 0.2,0>,< 0.2,0.2,0>,<0,0,0.2> }
  triangle { < 0.2, 0.2,0>,< 0.2,-0.2,0>,<0,0,0.2> }
  triangle { < 0.2,-0.2,0>,<-0.2,-0.2,0>,<0,0,0.2> }
   translate FenceHeight*z
  }
texture { FenceTexture }
}

#declare FenceSegment = union
{
object { FencePost }
#local N=FenceBeamsNum;
#local Y=FenceHeight;
#local R=seed(3333);
#while (N>0)
 #local Y=Y-FenceBeamSpacing;
 #local N=N-1;
 object { FenceBeam rotate <360*rand(R),(rand(R)-0.5)*20*atan2(FenceBeamSpacing,FenceSegmentLen),0> translate <FenceSegmentLen/2,0,Y> }
#end
}

#macro Fence()
#local K=WorldSize;
#while (K>7) #local K=K/2; #end
#local K=K+0.0001;
#local X=0;
#while (X<WorldSize-0.001)
object {FenceSegment scale <K/FenceSegmentLen,1,1>
	 translate <X,0,OFFSET_Z> }
object {FenceSegment scale <K/FenceSegmentLen,1,1>
    rotate <0,0,90>
	 translate <WorldSize,X,OFFSET_Z> }
object {FenceSegment scale <K/FenceSegmentLen,1,1>
    rotate <0,0,180>
	 translate <WorldSize-X,WorldSize,OFFSET_Z> }
object {FenceSegment scale <K/FenceSegmentLen,1,1>
    rotate <0,0,270>
	 translate <0,WorldSize-X,OFFSET_Z> }
#local X=X+K;
#end
#end

#macro TeleportBoundary()
difference 
 {box {<-5,-5,0.001>,<WorldSize+5,WorldSize+5,1>}
  box {<0,0,0>,<WorldSize,WorldSize,1.001>}

 pigment {color rgbf <1,1,1,1>}
 interior {media
	{ emission 0.1
	  density {gradient z translate <0,0,2> }
	}
   }
 hollow
 scale <1,1,7>
 translate <0,0,OFFSET_Z>
 }
#end


/////////////////////////////////////
// DUST

#declare RenderDust=no; //yes

#declare DustColor = color rgbf <0.7, 0.7, 0.6, 0>;
#declare DustTexture = texture
{
  pigment { bozo scale 0.3 }
  finish {ambient 1.0/GlobalAmbient diffuse 0}
}

#declare DustSize=0.4;
#declare DustDensity=0.4; //1.0
#declare DustIntervals=1; //2. im mniej tym szybciej i gorzej

#macro DustCloud(Location,Density,Size)
#if (RenderDust)
sphere { Location, Size*DustSize
  pigment { rgbt<1, 1, 1, 1> }
  interior {
    media {
      scattering {1, rgb <0.7,0.5,0.5>*DustDensity*Density/(Size*Size*Size) extinction 2}
      intervals DustIntervals
    }
  }
  hollow
}
#end
#end

/////////////////////////////////////
// ENERGY

#declare EnergyTexture = texture
{
pigment { marble
  color_map {
   [ 0 color rgbf <1,1,1,0.9+.2> ]
   [ 0.1 color rgbf <1,0.95,0.85,0.9+.2> ]
   [ 0.2 color rgbf <1,0.95,0.8,0.95+.2> ]
   [ 0.6 color rgbf <1,0.9,0.8,0.95+.2>]
            }
		turbulence 0.5
        }
	finish { ambient 2.0/GlobalAmbient
	         phong 0.4 }
}

#macro BeginEnergy()
#end

#macro EndEnergy()
#if (NumEnergyComponents>0)
blob {
 threshold 1
 #while (NumEnergyComponents>0)
  #declare NumEnergyComponents=NumEnergyComponents-1;
  component EnergyComponentsPower[NumEnergyComponents] EnergyComponentsPower[NumEnergyComponents] EnergyComponents[NumEnergyComponents] 
 #end
 texture {EnergyTexture translate <AnimFrame*0.03,0,0>}
// interior {WaterInterior} 
 hollow
 }
#end
#end

#declare REnergySeed=seed(300+mod(AnimFrame,20));

#declare PureEnergy=
union
{
 sphere
  { 0, 1 pigment { bozo color_map {[0.0 color rgbt <1,1,1,0.95>][1.0 color rgbt <1,1,1,1>]}
                 scale 0.2+0.2*rand(REnergySeed) turbulence 2 }
          finish { ambient rgb 4*<2,1,0.5>/GlobalAmbient } hollow
  } 
#local N=10;
#while (N>0)
 sphere { 0, 0.7 pigment {color rgbt <1,1,1,0.85>}
         finish { ambient rgb 3*<1,1,0.5>/GlobalAmbient } hollow scale <1,0.1,0.1>
 rotate <360.0*rand(REnergySeed),360.0*rand(REnergySeed),360.0*rand(REnergySeed)>
  }
#local N=N-1;
#end
}

/*
#declare PureEnergy=
sphere { 0, 1
  pigment { rgbt<1, 1, 1, 1> }
  interior {
    media {
      emission <5,5,2>
      density {
        spherical
        turbulence 0.5
//        color_map {
//          [0.0 color rgb <0, 0, 0>]
//          [0.4 color rgb <1, 0, 0>]
//          [0.7 color rgb <1, 1, 0.5>]
//          [1.0 color rgb <1, 1, 1>]
//        } 
      }
    }
  }
  hollow
}
*/

#declare MAXNRGCOMP=50;
#declare EnergyComponents=array[MAXNRGCOMP]
#declare EnergyComponentsPower=array[MAXNRGCOMP]
#declare NumEnergyComponents=0;

#macro AddEnergyComponent(Location,Power)
#if (NumEnergyComponents<MAXNRGCOMP)
#declare EnergyComponents[NumEnergyComponents]=Location;
#declare EnergyComponentsPower[NumEnergyComponents]=Power;
#declare NumEnergyComponents=NumEnergyComponents+1;
#end
#end

#macro Energy(Location,Amount)
#if (Amount>1)
#local R=1;
#else
#local R=Amount;
#end
#if (R>0.0001)
AddEnergyComponent(Location,2)
light_source { 0 color 4*<1,0.5,0.2> shadowless 
	fade_distance 1 fade_power 2 looks_like
{
//union {
// sphere {0,R
//	texture {EnergyTexture translate <AnimFrame*0.03,0,0>}
//    hollow
//        }
 object { PureEnergy scale 0.5 }
// }
}
translate Location + <0,0,0.1>
}
#end
#end

/////////////////////////////////////

#declare St_Asimil=0.25;
#declare St_Frict=0.25;
#declare St_Stamina=0.25;
#declare St_Ingest=0.25;
#declare St_Ingesting=0;
#declare St_Old=0;
#declare St_Speed=0.25;
#declare St_Damage=0;
#declare St_BendMuscle=0;
#declare St_BendMusclePos=0;
#declare St_RotMuscle=0;
#declare St_RotMusclePos=0;
#declare St_Recept_G=0;
#declare St_Recept_T=0;
#declare St_Recept_S=0;

#macro BeginCreature()
#end

#macro EndCreature()
#end

#macro CreatureStickEnd(Location,Rotation)
union {
sphere { <0,0,0>,0.15
interior {   ior 1.33 caustics 0.3 } 
texture
{
 pigment { color rgbf<0.9, 0.9, 1, 0.9> }
#if (St_Old)
  normal {  //Wrinkles
    waves St_Old*0.4
    frequency 10
    turbulence 0.2
    scallop_wave
    scale <0.1,0.8,0.1>
    rotate <10,40,30>
  }
#else
   normal { 
    waves .5
    frequency 2
//  turbulence 0.2
    sine_wave
    scale <0.2,1,1>
//  rotate <10,40,30>
    phase AnimFrame/50+(Location.x+Location.y+Location.z)*3  //random shift, different (but almost constant, depending on location) for each stickend
//#debug str((Location.x+Location.y+Location.z)/3,10,10)
//#debug "\n"
   }
#end
 finish {
roughness 0.01
    ambient 1.5/GlobalAmbient
    diffuse 0.2
reflection 0.2
  }   
}
 
/* texture
   {
//#if (St_Old<1)
   pigment { color rgb (StickRGB*(1-St_Old)+OldStickRGB*St_Old) }
//#else
//   pigment { Chlorofil scale 2 }
//#end
	normal { bumps 1.5*St_Frict*(1-St_Old) scale 0.01 }
   finish { phong (1-St_Old)*(1-St_Frict) }
   }*/
       }
  
#if (St_Ingest > 0.05)
#local tmp_dam=min(1.0,max(0.0,St_Damage));
 #local N=St_Ingest*50*(1-tmp_dam);
 #local R=seed(123);
 #while (N>0)
  #local N=N-1;
  sphere { <0,0,0.14> 0.03
	texture { 
#if (St_Ingesting)
		pigment { color White filter .6}
		finish { ambient .8/GlobalAmbient }
#else
		pigment { color rgb IngestRGB*(1-St_Old)+OldStickRGB*St_Old filter 0.6}
		finish { phong (1-St_Old) ambient .8/GlobalAmbient}
#end
		}
	rotate <0,360*rand(R),360*rand(R)>
    }
  #end
#end
rotate Rotation*57.3
translate Location
}
#if (St_Ingesting)
AddEnergyComponent(Location,0.99)
 #local Vect=2*(St_Ingesting_Location-Location);
 #local N=St_Ingest;
 #while (N>0)
  #local P=<rand(R)*0.2-0.1,rand(R)*0.2-0.1,rand(R)*0.2-0.1>;
  #local A=mod(2+rand(R)-mod(AnimFrame,20)/19,1);
  #if (A<0.5)
   object {PureEnergy scale 0.25 translate Vect*A+P+Location}
  #end
  #local N=N-0.05;
 #end 
#end
#end

/////////////////////////////////////////////////////////////

#macro CreatureStick(Location1,Location2,Rotation)
#local tmp_dam=min(1.0,max(0.0,St_Damage));
#if (St_BendMuscle | St_RotMuscle)
#local MuscleColor1=color rgb Muscle1RGB*(1-St_Old)+OldStick2RGB*St_Old;
#local MuscleColor2=color rgb Muscle2RGB*(1-St_Old)+OldStick2RGB*St_Old;
#local MuscleColor3=color rgb Muscle3RGB*(1-St_Old)+OldStick2RGB*St_Old;
#local MuscleTexture = texture
{
pigment
 {
 gradient x
 frequency 1
 turbulence 0.1
 ramp_wave
 color_map { [ 0 color MuscleColor1 filter .4]
             [ 0.2 color MuscleColor2 filter .4]
             [ 1 color MuscleColor3 filter .4] }
 }
normal
  {
  radial
  scallop_wave
  frequency 6
  rotate <0,0,90>
  }
finish { phong (1-St_Old)*0.6 ambient 0.9/GlobalAmbient }
}

#declare Muscle = union
{
cylinder {<0,0,0>,<1,0,0>,0.1 interior {ior 1.33 caustics 0.3}}
sphere {<0,0,0>,0.1 interior {ior 1.33 caustics 0.3}}
texture {MuscleTexture}
}
#end
#local Wrinkles = normal
  {
  waves St_Old*0.4
  frequency 10
  turbulence 0.2
  scallop_wave
  scale <0.1,0.8,0.1>
  rotate <10,40,30>
  }
#local StickColor=color rgb (StickRGB*(1-St_Old)+OldStickRGB*St_Old);
#declare Chlorofil = pigment
  {
  gradient x
  frequency 1
  turbulence 2
  color_map
    {
     [0 color StickColor]
     [0.95-St_Asimil color StickColor]
     [1.05-St_Asimil color rgb (AsimilRGB*(1-St_Old)+OldStickRGB*St_Old)]
    }
  triangle_wave
  }
#local StickTexture=texture
{
//pigment {Chlorofil}          
 #local temp_asim=max(0,(St_Asimil-0.25)/0.75);
 pigment { color rgbf<0.9-temp_asim*0.2, 0.9+temp_asim*0.2, 1-temp_asim*0.3, 0.9> }
#if (St_Old)
   normal { Wrinkles }
#else
   normal { 
    waves .5
    frequency 2
//  turbulence 0.2
    sine_wave
    scale <0.2,1,1>
//  rotate <10,40,30>
    phase AnimFrame/50+(Location1.x+Location1.y+Location1.z)*3  //random shift, different (but almost constant, depending on location) for each stick
   }
#end
//finish { phong (1-St_Old)*(1-St_Frict) }
 finish {
//    specular 1
roughness 0.01
    ambient 1.5/GlobalAmbient
    diffuse 0.2
reflection 0.2
  }   
}

union
{
 



#if (St_Recept_G)
#if (tmp_dam<0.9)
union {
 difference {
#end
#end



cylinder { <0,0,0>,<1,0,0>,0.1 interior {ior 1.33 caustics 0.3} texture {StickTexture} }



#if (St_Recept_G)
#if (tmp_dam<0.9)
//  cylinder { <0.38,0,0>,<0.62,0,0>,0.11 } //nie usuwamy srodka cylindra,
 sphere { <0.5,0,0>,(1-tmp_dam)*0.03 } //usuwamy tylko miejsce na kulke
  }
 cylinder { <0.38,0,0>,<0.62,0,0>,0.11*(1-tmp_dam) 
  texture
  {
   pigment { color rgbf <.8,.9,1, 0.3> }
   finish
   {
    ambient .7/GlobalAmbient
    diffuse .5
    reflection 0.5
    //refraction 1
    //ior 1.4
    phong 0.3
    phong_size 60
   }
  }
         }
 sphere { <0.5,0,0>,(1-tmp_dam)*0.03
  texture { pigment { color rgb St_Old*OldStick2RGB+(1-St_Old)*<1,0,0> }
            finish { phong 1 } }
        }
 }
#end
#end
#if (St_Recept_S)
difference {
 cylinder { <0.75,0,0>,<0.85,0,0>,0.12 
    pigment { color rgb St_Old*OldStick2RGB+(1-St_Old)*<1,0.6,0> filter .9}
    interior {   ior 1.33 caustics 0.3 }
    finish {ambient 4.5/GlobalAmbient}
 }
#local N=10;
#while (N>0)
 #local N=N-1;
 sphere { <0.78,0.12,0>,0.019 pigment { color Gray50 } rotate x*N*36 }
 sphere { <0.82,0.12,0>,0.019 pigment { color Gray50 } rotate x*(18+N*36) }
#end
 }
#end

#if (St_BendMuscle)
 #local R=0.2;
 #local NumSeg=12;
 #local Angle=0;
 #local Vect=<R,0,0>;
 #local O=(pi+St_BendMusclePos)*R;
 #local Len1=O/NumSeg;
 #local Angle2=0;
 #local Vect2=<R,0,0>;
 #local O2=(pi-St_BendMusclePos)*R;
 #local Len12=O2/NumSeg;
 #local A=1+tmp_dam*NumSeg;
 #while (A<NumSeg)
  #local A=A+1;
  #local Angle=Angle+(pi+St_BendMusclePos)/NumSeg;
  #local Angle2=Angle2+(pi-St_BendMusclePos)/NumSeg;

#if (St_BendMusclePos>-2.8)
  object
    { Muscle
    scale <Len1,St_Speed,St_Speed>
    rotate <0,0,270-degrees(Angle)>
    translate Vect
    }
#end
#if (St_BendMusclePos<2.8)
  object
    { Muscle
    scale <Len12,St_Speed,St_Speed>
    rotate <0,0,90+degrees(Angle2)>
    translate Vect2
    }
#end

  #local Vect=Vect+<-sin(Angle),-cos(Angle),0>*Len1;
  #local Vect2=Vect2+<-sin(Angle2),cos(Angle2),0>*Len12;
 #end
#end
#if (tmp_dam<0.09)
#if (St_RotMuscle)
 torus { 0.12, 0.1*St_Speed*(1-tmp_dam) interior {ior 1.33 caustics 0.3}
texture
{
  pigment
   {
   radial
   frequency 25
   turbulence 0.1
   ramp_wave
    color_map { [ 0 color MuscleColor1 filter .9]
             [ 0.2 color MuscleColor2  filter .9]
             [ 1 color MuscleColor3  filter .9] }
   }
   finish { phong (1-St_Old)*0.6 ambient 0.9/GlobalAmbient }
   } 
   rotate <0,0,90>
   translate <0.13,0,0>
  }
#end
#end
#if (St_Recept_T)
 #local N=3*(1-tmp_dam);
 #while (N>0)
  #local N=N-1;
  union {
    cylinder { <1,0.1,0>,<1.15,0.12,0>,0.01 }
    sphere { <1.15,0.12,0>,0.01 }
    cone { <1.15,0.12,0>,0.01,<1.3,0.15,0>,0.007 }
    sphere { <1.3,0.15,0>,0.02 }
    texture { pigment { color rgb <0.5,0.5,0.5> } 
      finish { phong 1 ambient 1/GlobalAmbient reflection .8} }
    rotate <45+120*N,0,0>
    }
 #end
#end

#if (St_Stamina>0.05)
 #local N=6*(1-tmp_dam);
 #while (N>0)
  #local N=N-1;
  cylinder 
   { <0,0,0.1>,<1,0,0.1>,St_Stamina*0.04 interior {ior 1.33 caustics 0.3}
   rotate <N*60,0,0>
   texture { pigment { color rgb StaminaRGB*(1-St_Old)+OldStick2RGB*St_Old filter .8} 
    //finish { ambient 1/GlobalAmbient} 
   }
	}
 #end
#end


scale <vlength(Location2-Location1),1,1>
rotate Rotation*57.3
translate Location1
}
#end


////////////////////////////////////////////////

#macro GetGroundTexture(Height)
#if (Height>WaterLevel)
#if (Height>(WaterLevel+2))
texture {HighTexture}
#else
texture {Ground0Texture}
#end
#else
texture {UnderwaterTexture}
#end
#end

#macro CommonWorldObjects()
StandardSky()
StandardLight()
#end

#macro BeginWorld()
CommonWorldObjects()
#switch (WorldType)
#case (WORLD_FLAT)
plane { z,OFFSET_Z GetGroundTexture(0) }
#break
#end
#if ((WaterLevel>0) | (WorldType != WORLD_FLAT))
// polygon {5, <-999,-999,0>,<999,-999,0>,<999,999,0>,<-999,999,0>,<-999,-999,0>
 plane { z,0
  texture { WaterTexture
  translate WaterWaveSpeed*AnimFrame }
  interior { WaterInterior }
	translate <0,0,WaterLevel+OFFSET_Z> 
  hollow
  }

//media { scattering {1,rgb WaterFogColor*0.01} }

//
 /*
 fog { color WaterFogColor
  distance WaterFogDistance
	fog_type 2
	fog_offset WaterLevel+OFFSET_Z-0.1
	fog_alt 0.1
	rotate <90,0,0>
	}
   */


fog { color WaterFogColor
  distance WaterFogDistance
fog_type 2
fog_offset WaterLevel+OFFSET_Z-0.1
fog_alt 0.1
rotate <90,0,0>
}

//
#end
#end

#macro FlatBlock(X,Y,Z)
box { <X,Y,-10>,<X+WorldBlockX,Y+WorldBlockY,Z+OFFSET_Z>
		GetGroundTexture(Z) }
#end

#macro SlideBlockX(X,Y,Z,Z2)
#local X2=X+WorldBlockX;
#local Y2=Y+WorldBlockY;
#local ZO=Z+OFFSET_Z;
#local ZO2=Z2+OFFSET_Z;
union {
 polygon {5,<X,Y,ZO>,<X2,Y,ZO2>,<X2,Y2,ZO2>,<X,Y2,ZO>,<X,Y,ZO>}
 polygon {5,<X,Y,ZO>,<X,Y2,ZO>,<X,Y2,-10>,<X,Y,-10>,<X,Y,ZO>}
 polygon {5,<X2,Y,ZO2>,<X2,Y2,ZO2>,<X2,Y2,-10>,<X2,Y,-10>,<X2,Y,ZO2>}
 polygon {5,<X,Y2,ZO>,<X2,Y2,ZO2>,<X2,Y2,-10>,<X,Y2,-10>,<X,Y2,ZO>}
 polygon {5,<X,Y,ZO>,<X2,Y,ZO2>,<X2,Y,-10>,<X,Y,-10>,<X,Y,ZO>}
  GetGroundTexture(((Z+Z2)/2)) }
#end

#macro SlideBlockY(X,Y,Z,Z2)
#local X2=X+WorldBlockX;
#local Y2=Y+WorldBlockY;
#local ZO=Z+OFFSET_Z;
#local ZO2=Z2+OFFSET_Z;
union {
 polygon {5,<X,Y,ZO>,<X2,Y,ZO>,<X2,Y2,ZO2>,<X,Y2,ZO2>,<X,Y,ZO>}
 polygon {5,<X,Y,ZO>,<X,Y2,ZO2>,<X,Y2,-10>,<X,Y,-10>,<X,Y,ZO>}
 polygon {5,<X2,Y,ZO>,<X2,Y2,ZO2>,<X2,Y2,-10>,<X2,Y,-10>,<X2,Y,ZO>}
 polygon {5,<X,Y2,ZO2>,<X2,Y2,ZO2>,<X2,Y2,-10>,<X,Y2,-10>,<X,Y2,ZO2>}
 polygon {5,<X,Y,ZO>,<X2,Y,ZO>,<X2,Y,-10>,<X,Y,-10>,<X,Y,ZO>}
  GetGroundTexture(((Z+Z2)/2)) }
#end


#macro EndWorld()
#switch (WorldBorder)
#case (BORDER_FENCE)
Fence()
#break
#case (BORDER_TELEPORT)
TeleportBoundary()
#break
#end
#end

#declare NoCameraSet=1;
#declare CameraLocation=<0,0,0>;
#macro Camera(Loc,LookAt)
#if (NoCameraSet)
camera { StandardCamera location Loc look_at LookAt }
#declare CameraLocation=Loc;
#declare NoCameraSet=0;
#end
#end
