//
// framsticks.inc, robot #2 setup
//
#include "colors.inc"
#include "textures.inc"
#include "woods.inc"

#debug concat("Animation Frame #",str(AnimFrame,5,0))

////////////////////////////////
// you can override these settings in your "framsticks-user.inc" file:

#declare UseAreaLight=0;
#declare SmoothHeightfield=1;
#declare SimpleGround=1;
// Camera(<position>,<look_at_target>)

////////////////////////////////

#declare StandardCamera=camera { up <0,0,1> right <4/3,0,0> sky <0,0,1>
location <0,0,0> look_at <1,1,1> }

#declare GlobalAmbient=5; //oryg. 3

global_settings
 {
 max_trace_level 100
 adc_bailout 0.1
 ambient_light GlobalAmbient
 assumed_gamma 2.2
 }

//////////////////////////////////////////
// colours
///////////////////////////////////////////
// stick:
#declare OldStickRGB = <0.65,0.65,0.55>;
#declare OldStick2RGB = <0.55,0.55,0.45>;
#declare StickRGB = <0.46,0.57,0.42>;
#declare StaminaRGB = <0.53,0.65,0.50>;
#declare IngestRGB = <0.93,0.93,0>;
#declare AsimilRGB = <0.14,1,0.14>;

/////////////////////
// muscle
#declare Muscle1RGB=<1,0.1,0.1>;
#declare Muscle2RGB=<1,0,0>;
#declare Muscle3RGB=<0.3,0,0>;

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
#declare WORLD_SMOOTH=2;

#declare BORDER_NONE=0;
#declare BORDER_FENCE=1;
#declare BORDER_TELEPORT=2;

#declare OFFSET_Z=-0.2;

#declare WallTexture = texture { pigment {granite} }

#declare HexPigment = pigment {hexagon <0.50, 0.30, 0.20>,<0.55, 0.35, 0.25>,<0.52, 0.32, 0.22> scale 0.01}


#declare Ground0Texture = texture
#if (SimpleGround)
//   {color rgb <1,1,1>}      //white

	{ pigment 
	{ cylindrical rotate <90,0,0> scale 100.0 translate <9,12,0>
	   pigment_map {
	        [0.0 color <0.53, 0.33, 0.23>]
	        [1.0 HexPigment]
	      }
         }

    finish {ambient 0.4}
#else
        { pigment
//   {color rgb <1,0.9,0.6>}
	 {
	 granite
		color_map
			{
			[ 0.0 rgb <0.95, 0.90, 0.60> ]
			[ 0.7 rgb <0.76, 0.70, 0.40> ]
			[ 1.0 rgb <0.76, 0.70, 0.40> ]
			}
	 scale .5 					//3
	 }
    normal {bumps 0.2 scale 0.02}
#end
	}

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
	pigment
    {
	 crackle
	color_map
      { [ 0 color Gray40 ]
		  [ 0.1 color Gray80 ]
		  [ 1 color White ] }
    }
}


///////////////////////////////////

/*
#declare HighTexture=texture
	{
	pigment
//   {color rgb <1,0.9,0.6>}
	 {
	 granite
		color_map
			{
			[ 0.0 rgb <0.95, 0.90, 0.60> ]
			[ 0.7 rgb <0.76, 0.70, 0.40> ]
			[ 1.0 rgb <0.76, 0.70, 0.40> ]
			}
	 scale .5  					//3
	 }
    normal {bumps 0.2 scale 0.02}
	}
*/

/////////////////////////////////////////

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

/////////////////////////////////////
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

/////////////////////////////////////

#macro CreateGear(N,R1,R2,W)
prism { linear_spline linear_sweep -W/2, W/2, N
#local A=0;
#while (A<2*pi-0.0001)
 ,<sin(A),cos(A)>*R2
 #local A=A+(2.0*pi/N);  
 ,<sin(A),cos(A)>*R1
 #local A=A+(2.0*pi/N);           
#end
 }
#end

#declare RoboMetal = texture
{
    pigment { rgb <0.45, 0.5, 0.55> }
    finish {
        metallic
        ambient 0.1
        diffuse 0.75
        specular 0.85
        roughness 0.01
        reflection 0.25
        brilliance 1.5
    }
}

#declare MuscleGear = difference
{
CreateGear(60,0.12,0.1,0.03)
#local A=0;
#while (A<6) 
cylinder {<0.065,-0.03,0>,<0.065,0.03,0>,0.028 rotate <0,60*A,0>}
#local A=A+1;
#end
 texture {RoboMetal pigment {color <0.8,0.1,0.1>}}
}

#declare MuscleMotor = union
{
CreateGear(14,0.05,0.03,0.05)
cylinder {<0,-0.03,0>,<0,0.05,0>,0.01}
cylinder {<0,0.03,0>,<0,0.1,0>,0.05}
texture {RoboMetal}
}

//object {MuscleMotor scale 10.0 translate <11,4,1>}

#macro CreatureStickEnd(Location,Rotation)
union
{
union
 {
 sphere { <0,0,0>,0.1 }
 texture {RoboMetal normal { bumps 0.5*field_p_fr scale 0.01 }}
 }

#if (St_Recept_S)
difference {
 cylinder { <-0.15,0,0>,<-0.25,0,0>,0.12 pigment { color rgb <1,0.6,0> }}
#local N=10;
#while (N>0)
 #local N=N-1;
 sphere { <-0.18,0.12,0>,0.019 pigment { color Gray10 } rotate x*N*36 }
 sphere { <-0.22,0.12,0>,0.019 pigment { color Gray10 } rotate x*(18+N*36) }
#end
 }
#end // Recept_S

#if (St_Recept_T)
 #local N=3;
 #while (N>0)
  #local N=N-1;
  union {
    cylinder { <0,0.1,0>,<0.15,0.12,0>,0.01 }
    sphere { <0.15,0.12,0>,0.01 }
    cone { <0.15,0.12,0>,0.01,<0.3,0.15,0>,0.007 }
    sphere { <0.3,0.15,0>,0.02 }
    texture { pigment { color rgb <0.1,0.1,0.1> } 
      finish { phong 1 } }
    rotate <45+120*N,0,0>
    scale 0.9
    }
 #end
#end // St_Recept_T

rotate Rotation*57.3
translate Location
}
#end

/////////////////////////////////////////////////////////////

#declare SmallStick = box {<0,-0.02,-0.015>,<0.23,0.02,0.015>}

#declare G_Ring = difference
{
cylinder {<0,0,-0.01>,<0,0,0.01>,0.1}
cylinder {<0,0,-0.02>,<0,0,0.02>,0.08}
texture {RoboMetal pigment {color <0.1,0.1,0.6>}}
}

#declare StickCylinder = lathe
{
linear_spline 16,
<0,0>,<0.09,0.2>,
<0.09,0.25>,<0.08,0.26>,
<0.08,0.35>,<0.09,0.36>,
<0.09,0.45>,<0.08,0.46>,
<0.08,0.55>,<0.09,0.56>,
<0.09,0.65>,<0.08,0.66>,
<0.08,0.75>,<0.09,0.76>,
<0.09,0.8>,<0,1>
rotate <0,0,-90>
}

#macro CreatureStick(Location1,Location2,Rotation)

union
{
object {StickCylinder  texture {RoboMetal} }

#if (St_BendMuscle)
union
{
 object {MuscleGear scale 1.3 rotate <0,St_BendMusclePos,0> rotate <90,0,0>}
 //object {MuscleMotor rotate <90,0,0> translate <0,-0.185,0>}
 //box {<0,-0.02,-0.02>,<0.22,0.02,0.02> rotate <0,0,55> translate <0,-0.185,0.07>  texture {RoboMetal} }
scale <1.0/vlength(Location2-Location1),1,1>
}
#end // BendMuscle

#if (St_RotMuscle)
union
{
 object {MuscleGear scale 1.3 rotate <0,St_RotMusclePos,0> rotate <0,0,90> translate <0.12,0,0> }
 //object {MuscleMotor rotate <0,0,-90> translate <0.12,-0.185,0>}
 //box {<0,-0.02,-0.02>,<0.22,0.02,0.02> rotate <0,0,55> translate <0.18,-0.185,0>  texture {RoboMetal} }
scale <1.0/vlength(Location2-Location1),1,1>
}
#end

#if (St_Recept_G)
object {G_Ring translate <0.5,0,0>}
object {G_Ring rotate <0,0,90> translate <0.5,0,0>}
object {G_Ring rotate <0,90,0> translate <0.5,0,0>}
#end

scale <vlength(Location2-Location1),1,1>
rotate Rotation*57.3
translate Location1
}
#end


////////////////////////////////////////////////

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
plane { z,OFFSET_Z GetGroundTexture(0) }
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

//media { scattering {1,rgb WaterFogColor*0.01} }

/*
 fog { color WaterFogColor
  distance WaterFogDistance
	fog_type 2
	fog_offset field_World_wrldwat+OFFSET_Z-0.1
	fog_alt 0.1
	rotate <90,0,0>
	}
*/
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

//////////////////////////////

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
  #declare St_RotMusclePos=field_n_s*3.14;
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
