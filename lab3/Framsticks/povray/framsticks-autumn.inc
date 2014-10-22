//
// framsticks.inc, autumn setup, just a draft idea
// by Michal Marcinkowski
// updated by Szymon Ulatowski, 2006

// todo:
// - bending muscle
// - teleport
// - nicer chestnut (Part)
// - blocks ground
// - part/joint properties visualized
// - energy (?)
// - overall improvements


#include "colors.inc"
#include "textures.inc"
#include "woods.inc"

#debug concat("Animation Frame #",str(AnimFrame,5,0))

//#declare NoLight=true;
#declare NoLight=false;
#declare DarkFog=true;
//#declare DarkFog=false;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// you can override these settings in your "framsticks-user.inc" file:

#declare UseAreaLight=0;
#declare SmoothHeightfield=1;
#declare SimpleGround=0;
// Camera(<position>,<look_at_target>)

/////////////////////
// muscle colors
#declare Muscle1RGB=<1,0.1,0.1>;
#declare Muscle2RGB=<1,0,0>;
#declare Muscle3RGB=<0.3,0,0>;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#declare StandardCamera=camera { up <0,0,1> right <4/3,0,0> sky <0,0,1>
location <0,0,0> look_at <1,1,1> }

#if (NoLight)
 #declare GlobalAmbient=12;
 #if (DarkFog)
  fog { color Gray70   distance 10 fog_type 0 }
 #end
#else
 #declare GlobalAmbient=3;
#end

global_settings
 {
 max_trace_level 100
 adc_bailout 0.1
 ambient_light GlobalAmbient
 assumed_gamma 2.2
 }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// world:

#declare WORLD_FLAT=0;
#declare WORLD_BLOCKS=1;
#declare WORLD_SMOOTH=2;

#declare BORDER_NONE=0;
#declare BORDER_FENCE=1;
#declare BORDER_TELEPORT=2;

#declare OFFSET_Z=-0.2;

#declare WallTexture = texture { pigment {granite} }                                   

//237 187 131

#macro avgcolor(c1,c2)
(c1+c2)/2
#end

#declare AvgMixRGB = <235/255,185/255,129/255>;
#declare Ground0AvgRGB = <0.859,0.549,0.239>;

#declare Ground0AvgColor = color rgb avgcolor(Ground0AvgRGB,AvgMixRGB);

#declare Ground0Finish = finish {ambient 0.7/GlobalAmbient diffuse 0.3 }
#declare Ground0AvgTex = texture {pigment {Ground0AvgColor} finish {Ground0Finish}}
#declare Ground0BmpTex = texture {pigment {image_map {png "autumn-seamless2.png" interpolate 2} rotate <-90,0,0> scale <1.5,1,1>*0.02 } finish {Ground0Finish}}
#declare Ground0ProcTex = texture 
{
 pigment { bozo color_map {
  [0 rgb avgcolor(<236/255,213/255,74/255>,AvgMixRGB)]
  [0.25 rgb avgcolor(<231/255,186/255,46/255>,AvgMixRGB)]
  [0.5 rgb avgcolor(<220/255,138/255,59/255>,AvgMixRGB)]
  [0.75 rgb avgcolor(<217/255,115/255,80/255>,AvgMixRGB)]
  [1 rgb avgcolor(<150/255,72/255,35/255>,AvgMixRGB)] } }
 finish {Ground0Finish}
 scale 0.002
}

#declare Ground0Texture = texture {
#if (SimpleGround)
  pigment {color rgb <1,1,1>}      //white
#else
  cylindrical
  //bozo
  texture_map
	  {
          [0.0 Ground0AvgTex]
          [0.9 Ground0ProcTex]
	  [0.98 Ground0BmpTex]
	  }
  rotate <90,0,0>
  scale 1000
  translate <20,20,0>
#end
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#declare HighTexture=texture
{
	pigment
    {
	 crackle
	color_map
      { [ 0 color Gray40 ]
		  [ 0.1 color Gray80 ]
		  [ 1 color White ] }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
#if (UseAreaLight)
 light_source
 {
  0*x // light's position (translated below)
  color rgb 1.5  // light's color
           // <widthVector> <heightVector> nLightsWide mLightsHigh
  area_light  <8,0,0> <0,8,0>  4, 4
  adaptive 1    jitter    translate <-20, -20, 30>
 }
#else
 light_source
 { <2000, -1000, 3000> color
 rgb <1.5,1.5,1.5>
 }
#end
#end

//////////////////////////////////////////////////////////
// SKY

#macro StandardSky()
sky_sphere {
    pigment {
        gradient y
        pigment_map {
            [0.01 rgb <0.9, 0.92, 0.95> ] // horizon
            [0.25
    wrinkles
    turbulence 0.65
    octaves 6
    omega 0.7
    lambda 2
    color_map {
        [0.0 color rgb <0.93,0.93,0.93>]
        [0.1 color rgb <0.88,0.88,0.88>]
        [0.5 color rgb <0.4, 0.4, 0.8>]
        [1.001 color rgb <0.31, 0.36, 0.93>]
    }
    scale <1.5, 0.25, 1.5> rotate z*5]
  [0.60
    bozo
    color_map {
        [0.0 color rgb <0.95,0.95,0.95>]
        [0.1 color rgb <0.9,0.9,0.9>]
        [0.5 color rgb <0.4, 0.4, 0.8>]
        [1.001 color rgb <0.04, 0.2, 0.66>]
    }
    turbulence 0.65
    octaves 6
    omega 0.707
    lambda 2
    scale <1.5, 0.6, 1.5> rotate z*10]
        }
    }
 rotate <90,0,0>
}
#end

//background { color SkyBlue }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FENCE
                                                                                
                                                                                
#declare FenceSegmentLen = 5;
#declare FenceHeight = 4;
#declare FenceBeamSpacing = 1;
#declare FenceBeamHeight = 1;
#declare FenceBeamsNum = 3;
                                                       
#declare FenceTexture = pigment {
    granite
      color_map {
        [0.0 rgb<0.95, 0.90, 0.60>]
        [0.7 rgb<0.76, 0.70, 0.40>]
        [1.0 rgb<0.76, 0.70, 0.40>]      
        }
  }

// FENCE

#declare FenceBeam = object
{
cylinder { <-0.2-FenceSegmentLen/2,0,0>,<FenceSegmentLen/2+0.2,0,0>,0.15
 texture {FenceTexture rotate <0,90,0> }}
}
#declare FencePost = union
{
box { <-0.1,-0.1,0>,<0.1,0.1,FenceHeight> texture { FenceTexture } }

sphere { <0,0,0>, 1 scale <0.27,0.27,0.37>  pigment { color Green } normal { bumps 0.5 scale 0.02 } translate FenceHeight*z }

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
#local K=field_World_wrldsiz;
#while (K>7) #local K=K/2; #end
#local K=K+0.0001;
#local X=0;
#while (X<field_World_wrldsiz-0.001)
object {FenceSegment scale <K/FenceSegmentLen,1,1>
	 translate <X,0,OFFSET_Z> }
object {FenceSegment scale <K/FenceSegmentLen,1,1>
    rotate <0,0,90>
	 translate <field_World_wrldsiz,X,OFFSET_Z> }
object {FenceSegment scale <K/FenceSegmentLen,1,1>
    rotate <0,0,180>
	 translate <field_World_wrldsiz-X,field_World_wrldsiz,OFFSET_Z> }
object {FenceSegment scale <K/FenceSegmentLen,1,1>
    rotate <0,0,270>
	 translate <0,field_World_wrldsiz-X,OFFSET_Z> }
#local X=X+K;
#end
#end

#macro TeleportBoundary()
difference 
 {box {<-5,-5,0.001>,<field_World_wrldsiz+5,field_World_wrldsiz+5,1>}
  box {<0,0,0>,<field_World_wrldsiz,field_World_wrldsiz,1.001>}

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENERGY
                                                                        
#declare EnergyTexture = texture
{
pigment { marble
  color_map {
   [ 0 color rgbf <1,1,1,0.9> ]
   [ 0.1 color rgbf <1,0.95,0.85,0.9> ]
   [ 0.2 color rgbf <1,0.95,0.8,0.95> ]
   [ 0.6 color rgbf <1,0.9,0.8,0.95>]
            }
		turbulence 0.5
        }
	finish { ambient 1.0/GlobalAmbient
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
          finish { ambient rgb 5*<2,1,0.5>/GlobalAmbient } hollow
  } 
#local N=10;
#while (N>0)
 sphere { 0, 0.7 pigment {color rgbt <1,1,1,0.85>}
         finish { ambient rgb 5*<1,1,0.5>/GlobalAmbient } hollow scale <1,0.1,0.1>
 rotate <360.0*rand(REnergySeed),360.0*rand(REnergySeed),360.0*rand(REnergySeed)>
  }
#local N=N-1;
#end
}

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
light_source { 0 color <1,0.5,0.2> shadowless 
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

/////////////////////////////////////////////////////////////

#declare MuscleTexture = texture
{
pigment
 {
 gradient x   frequency 1   turbulence 0.1   ramp_wave
 color_map {[0 color Muscle1RGB]  [0.2 color Muscle2RGB]  [1 color Muscle3RGB]}
 }
normal { radial   scallop_wave   frequency 6   rotate <0,0,90> }
finish { phong 0.6 }
}

#declare Muscle = union
{
cylinder {<0,0,0>,<1,0,0>,0.1}
sphere {<0,0,0>,0.1}
texture {MuscleTexture}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // chestnut

#declare ChestnutColor = color red 0.45 green 0.1 blue 0.04;
#declare ChestnutTexture = texture { pigment { color ChestnutColor } finish { phong 0.7 } }

#macro CreatureStickEnd(Location,Rotation)

union {
blob { threshold 0.6
    sphere {<0.1,0,0>,0.40, 1 texture {ChestnutTexture} }
    sphere {<0,0,-0.05>,0.25, 1 pigment {color Flesh} finish { phong 0.1 } }
    sphere {<0,0,0.05>,0.32, 1 texture {ChestnutTexture} }
    sphere {<-0.1,0,0>0.33, 1 texture {ChestnutTexture} }
    
     }

// receptor Smell - 'obroza' z siarka koloru niebieskiego
#if (St_Recept_S)
 union {      
      cylinder { <-0.15,0,0>,<-0.25,0,0>,0.001 pigment { color White }}
#local N=10;
#while (N>0)
 #local N=N-1; 
 //sphere { <-0.16,0.24,-0.01>,0.025 pigment { color Blue } rotate x*N*36 } 
     blob { threshold .6
       sphere {<-0.18,0.24,-0.01>,0.04, 1 pigment {color Blue}}
       sphere {<-0.18,0.26,-0.01>,0.04, 1 pigment {color Blue}}
       rotate x*N*36
       }
 box {<-0.17,0.12,-0.02>,<-0.19,0.24,0> pigment { color Yellow } rotate x*N*36}
#end
 }
#end // Recept_S



// receptor Touch - siarka koloru zielonego
#if (St_Recept_T)
 #local N=3;
 #while (N>0)
  #local N=N-1;
union {
    cylinder { <0,0.1,0>,<0.15,0.12,0>,0.01 pigment { granite color_map { [0.0 rgb<0.95, 0.90, 0.60>] [0.7 rgb<0.76, 0.70, 0.40>] [1.0 rgb<0.76, 0.70, 0.40>] } } }
    sphere { <0.15,0.12,0>,0.01 pigment { color Yellow } }
    cone { <0.15,0.12,0>,0.01,<0.3,0.15,0>,0.007 pigment { granite color_map { [0.0 rgb<0.95, 0.90, 0.60>] [0.7 rgb<0.76, 0.70, 0.40>] [1.0 rgb<0.76, 0.70, 0.40>] } } }
    blob { threshold .6
       sphere {<0.3,0.15,0>,0.04, 1 pigment {color Green}}
       sphere {<0.33,0.16,0>,0.04, 1 pigment {color Green}}
       }
   texture { finish { phong 1 } }
    rotate <45+120*N,0,0>
    }
 #end
#end // St_Recept_T


#if (field_p_ing > 0.05)
 #local N=field_p_ing*50;
 #local R=seed(123);
 #while (N>0)
  #local N=N-1;
  sphere { <0,0,0.14> 0.13
  texture { 
	pigment { color DarkBrown }
	//finish { phong 1 }
	}
   rotate <0,360*rand(R),360*rand(R)>
    }
 #end
#end

rotate Rotation*57.3 
//this pigment/rotation seems not supported (by pov-export or this source?),
//but still gives somehow better look than Part-less Joints

translate Location
}
#end

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                                                   // match
#macro CreatureStick(Location1,Location2,Rotation)  
union
{   
  box { <0,-0.05,-0.05>,<1,0.05,0.05>
  pigment {
    granite
      color_map {
                  [0.0 rgb<0.95, 0.90, 0.60>]
                  [0.7 rgb<0.76, 0.70, 0.40>]
                  [1.0 rgb<0.76, 0.70, 0.40>]      
                }
          }
      }

// jako receptor G posluzy lustrzana opaska
#if (St_Recept_G)
union { difference {
#end
#if (St_Recept_G)
 cylinder { <0.38,0,0>,<0.62,0,0>,0.1
  texture
    {
    pigment  {color Black}
    finish   {ambient 0   diffuse 0   reflection 0.5   refraction 1   phong 0.3   phong_size 60}
    } 
          } 
}}
#end // Recept_G

// miesien obracajacy - Rotation
#if (St_RotMuscle)
 torus { 0.12, 0.1*St_Speed
texture
{
  pigment
   {
   radial
   frequency 25
   turbulence 0.1
   ramp_wave
    color_map { [ 0 color Black ]  [ 0.2 color DarkBrown ]  [ 1 color DarkGreen ] }
   }

   finish { phong 0.6 }
   } 
   rotate <0,0,90>
   translate <0.32,0,0>
   scale<1,1,1>
  }
#end  // Rotation


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
 #local A=1;

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
#end // BendMuscle

scale <vlength(Location2-Location1),1,1>
rotate Rotation*57.3
translate Location1
}
#end

///////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                                        // original ground
#macro GetGroundTexture(Height)
#if (Height>field_World_wrldwat)
#if (Height>(field_World_wrldwat+2))
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

#declare field_World_wrldtyp=0;
#declare field_World_wrldwat=0;
#declare field_World_wrldbnd=0;

#macro BeginWorld()
#if (field_World_simtype=1)
 #declare OFFSET_Z=0.0;
#end
CommonWorldObjects()
#switch (field_World_wrldtyp)
#case (WORLD_FLAT)
plane { z,OFFSET_Z
texture {Ground0Texture}
}

#break
#end
#if ((field_World_wrldwat>0) | (field_World_wrldtyp != WORLD_FLAT))
// polygon {5, <-999,-999,0>,<999,-999,0>,<999,999,0>,<-999,999,0>,<-999,-999,0>
 plane { z,0
  texture { WaterTexture
  translate WaterWaveSpeed*AnimFrame }
  interior { WaterInterior }
	translate <0,0,field_World_wrldwat+OFFSET_Z> 
  hollow
  }

#end
#end

#macro FlatBlock(X,Y,Z)
box { <X,Y,-10>,<X+WorldBlockX,Y+WorldBlockY,Z+OFFSET_Z>
	GetGroundTexture(Z)}
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

#macro SmoothMapNormal(i,j)
#if (i>0)
#local zlx=MapData[i-1][j];
#else
#local zlx=MapData[i][j];
#end
#if (j>0)
#local zly=MapData[i][j-1];
#else
#local zly=MapData[i][j];
#end
#if (i<ny)
#local zrx=MapData[i+1][j];
#else
#local zrx=MapData[i][j];
#end
#if (j<nx)
#local zry=MapData[i][j+1];
#else
#local zry=MapData[i][j];
#end
<(zlx-zrx)/dy,(zly-zry)/dx,2>
#end

#macro MakeSmoothWorld()
#local i=0;
#local j=0;
#local X=0;
#local Y=0;
#local nx=dimension_size(MapData,2)-1;
#local ny=dimension_size(MapData,1)-1;
#local dx=field_World_wrldsiz/nx;
#local dy=field_World_wrldsiz/ny;
mesh {
#while (i<nx)
  #declare j=0;
  #declare Y=0;
  #while (j<ny)

#if (SmoothHeightfield)
smooth_triangle { <X,Y,MapData[j][i]>,SmoothMapNormal(j,i),
		  <X+dx,Y,MapData[j][i+1]>,SmoothMapNormal(j,i+1),
		  <X+dx,Y+dy,MapData[j+1][i+1]>,SmoothMapNormal(j+1,i+1) }
smooth_triangle { <X,Y,MapData[j][i]>,SmoothMapNormal(j,i),
		  <X+dx,Y+dy,MapData[j+1][i+1]>,SmoothMapNormal(j+1,i+1),
		  <X,Y+dy,MapData[j+1][i]>,SmoothMapNormal(j+1,i) }
#else
triangle { <X,Y,MapData[j][i]>,<X+dx,Y,MapData[j][i+1]>,<X+dx,Y+dy,MapData[j+1][i+1]> }
triangle { <X,Y,MapData[j][i]>,<X+dx,Y+dy,MapData[j+1][i+1]>,<X,Y+dy,MapData[j+1][i]> }
#end

    #declare j=j+1; #declare Y=Y+dy;
  #end
  #declare i=i+1; #declare X=X+dx;
#end
texture {Ground0Texture}
}
union
{
#local S=field_World_wrldsiz;
polygon {5,<0,0,0>,<0,S,0>,<0,S,-3>,<0,0,-3>,<0,0,0>}
polygon {5,<0,0,0>,<S,0,0>,<S,0,-3>,<0,0,-3>,<0,0,0>}
polygon {5,<S,0,0>,<S,S,0>,<S,S,-3>,<S,0,-3>,<S,0,0>}
polygon {5,<0,S,0>,<S,S,0>,<S,S,-3>,<0,S,-3>,<0,S,0>}
texture {Ground0Texture}
}
#end

#macro EndWorld()
#switch (field_World_wrldbnd)
#case (BORDER_FENCE)
Fence()
#break
#case (BORDER_TELEPORT)
TeleportBoundary()
#break
#end
#if (field_World_wrldtyp=WORLD_SMOOTH)
MakeSmoothWorld()
#end
#end

#declare NoCameraSet=1;
#macro Camera(Loc,LookAt)
#if (NoCameraSet)
// camera { StandardCamera location Loc look_at LookAt }
camera { up <0,0,1> right <4/3,0,0> sky <0,0,1> angle 45
         location Loc look_at LookAt }
#declare NoCameraSet=0;
#end
#end

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#macro BeginScene()
BeginEnergy()
#end

#macro EndScene()
EndEnergy()
#end

#macro BeginObject()
#declare PartAssimTab=array[field_Creature_numparts]
#end

#macro EndObject()
#end

#macro BeginPart(Id)
#declare St_Recept_S=0;
#declare St_Recept_T=0;
#declare CreatePartAtEnd=1;
#declare PartAssimTab[Id]=field_p_as;
#end

#macro PartGeometry(Location,Rotation)
 #if (!strcmp(field_m_Vstyle,"food"))
  Energy(Location,1.0)
  #declare CreatePartAtEnd=0;
 #end
 #if (!strcmp(field_m_Vstyle,"manipulator"))
  Manipulator(Location,0.0)
  #declare CreatePartAtEnd=0;
 #end
  #declare StickEndLoc=Location;
  #declare StickEndRot=Rotation;
#end

#macro EndPart()
#if (CreatePartAtEnd)
CreatureStickEnd(StickEndLoc,StickEndRot)
#end
#end

#macro BeginJoint(Part1,Part2)
#declare St_Recept_G=0;
#declare St_BendMuscle=0;
#declare St_RotMuscle=0;
#declare St_Asimil=(PartAssimTab[Part1]+PartAssimTab[Part2])/2.0;
#end

#macro JointGeometry(Location1,Location2,Rotation)
#declare StickLoc1=Location1;
#declare StickLoc2=Location2;
#declare StickRot=Rotation;
#end

#macro EndJoint()
#declare St_Speed=0.5;
CreatureStick(StickLoc1,StickLoc2,StickRot)
#end

#macro BeginNeuro(NClass)
 #if (!strcmp(NClass,"|"))
  #declare St_BendMuscle=1;
  #declare St_BendMusclePos=field_n_s*3.14;
 #end
 #if (!strcmp(NClass,"@"))
  #declare St_RotMuscle=1;
 #end
 #if (!strcmp(NClass,"G"))
  #declare St_Recept_G=1;
 #end
 #if (!strcmp(NClass,"S"))
  #declare St_Recept_S=1;
 #end
 #if (!strcmp(NClass,"T"))
  #declare St_Recept_T=1;
 #end
#end

#macro EndNeuro()
#end
