//
// framsticks.inc, matrix setup
//
#include "colors.inc"
#include "textures.inc"
#include "woods.inc"

#debug concat("Animation Frame #",str(AnimFrame,5,0))

//#declare NoLight=true;
#declare NoLight=false;
#declare DarkFog=true;
//#declare DarkFog=false;

////////////////////////////////
// you can override these settings in your "framsticks-user.inc" file:

#declare SmoothHeightfield=1;
#declare SimpleGround=0;
// Camera(<position>,<look_at_target>)

////////////////////////////////

#declare StandardCamera=camera { up <0,0,1> right <4/3,0,0> sky <0,0,1>
location <0,0,0> look_at <1,1,1> }

#if (NoLight)
 #declare GlobalAmbient=12;
 #if (DarkFog)
  fog { color Black   distance 10 fog_type 0 }
 #end
#else
 #declare GlobalAmbient=8; //oryg. 3
#end

global_settings
 {
 max_trace_level 100
 adc_bailout 0.1
 ambient_light GlobalAmbient
 assumed_gamma 2.2
 }


///////////////////////////////
// world:

#declare WORLD_FLAT=0;
#declare WORLD_BLOCKS=1;
#declare WORLD_SMOOTH=2;

#declare BORDER_NONE=0;
#declare BORDER_FENCE=1;
#declare BORDER_TELEPORT=2;

#declare OFFSET_Z=-0.15;//-0.2;

#declare WaterTexture = texture
{
 pigment { red 0.5 green 0.5 blue 0.5 filter 0.3 }
 finish { reflection 0.6 brilliance 3}
 normal { bumps 0.5 scale 2 }
}
#declare WaterInterior = interior
{ ior 1.1 caustics 0.3 }

#declare WaterWaveSpeed = <0,0,0.2>;

#macro StandardLight()
 light_source
 { <2000, -1000, 3000> color rgb <1.5,1.5,1.5>
 //shadowless
 }
#end

//////////////////////////////////////////////////////////
// SKY

background { color Black }

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

#declare G1 = pigment
{
gradient y color_map
   {[0.2 rgb Green]
    [0.2 rgb Black]}
    frequency pi*15
}

#declare G2 = pigment
{
gradient z
 pigment_map {
 [0.2 rgb Green]
 [0.2 G1 ]  }
    frequency pi*30
}

#declare partpigment = 
     pigment { G2
       warp {
         spherical
         orientation x
         dist_exp 1 
       }
 }
  
#macro CreatureStickEnd(Location,Rotation)
union {
sphere { <0,0,0>,0.1 texture { pigment { partpigment } } }

rotate Rotation*57.3 
//this pigment/rotation seems not supported (by pov-export or this source?),
//but still gives somehow better look than Part-less Joints

translate Location
}
#end

/////////////////////////////////////////////////////////////

//three grounds: green wide grid lighter, green grid thin sharp, black

/*
#declare GroundGreen = rgb <0,0.2,0>;
#declare GroundLineWidth = 0.1;
#declare GroundGreen2 = rgb <0,0.1,0>;
#declare GroundScale = 2;
*/

#declare GroundGreen = rgb <0,0.3,0>;
#declare GroundLineWidth = 0.04;
#declare GroundGreen2 = rgb Black;
#declare GroundScale = 1;

/*
#declare GroundGreen = rgb Black;
#declare GroundLineWidth = 0.1;
#declare GroundGreen2 = rgb Black;
#declare GroundScale = 1;
*/


#declare GroundTex1 = pigment
{
gradient y color_map
   {[GroundLineWidth rgb GroundGreen]
    [GroundLineWidth rgb GroundGreen2]}
}

#declare GroundTex = pigment
{
gradient x
 pigment_map {
 [GroundLineWidth GroundGreen]
 [GroundLineWidth GroundTex1 ]
             }
    scale GroundScale
}


#declare Wzdluz = pigment
  { radial
    color_map {[0.2 rgb <0,1,0>][0.2 rgb <0,0,0>]}
    frequency 8
    rotate <0,0,90>
  }

#declare Koncowka = pigment
  { cylindrical
    color_map {[0.15 rgb <0,1,0>][0.15 rgb <0,0,0>]}
    scale 0.1
    rotate <0,0,90>
  }

#macro CreatureStick(Location1,Location2,Rotation)
union
{
cylinder { <0,0,0>,<1,0,0>,0.1
   pigment { gradient x
      pigment_map {
      [0.2 Green]
      [0.2 Wzdluz]
          }
       frequency vlength(Location2-Location1)*12
         }
       }
cylinder { <-0.001,0,0>,<0.001,0,0>,0.101 pigment {Koncowka} }
cylinder { <0.999,0,0>,<1.001,0,0>,0.101 pigment {Koncowka} }

scale <vlength(Location2-Location1),1,1>
rotate Rotation*57.3
translate Location1
}
#end

////////////////////////////////////////////////

#macro CommonWorldObjects()
#if (NoLight)
#else
StandardLight()
#end
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
plane { z,OFFSET_Z texture {GroundTex} }
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
	       texture {GroundTex}
    }
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
	       texture {GroundTex}
   }
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
	       texture {GroundTex}
   }
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
translate <0,0,OFFSET_Z>
texture {GroundTex}
}
#end

#macro EndWorld()
#if (field_World_wrldtyp = WORLD_FLAT)
plane { z,OFFSET_Z texture {GroundTex} }
#else
difference {
 plane { z,OFFSET_Z }
 box { <0.01,0.01,-100>,<field_World_wrldsiz-0.01,field_World_wrldsiz-0.01,OFFSET_Z+1> }
 texture {GroundTex}
 }
#end
#switch (field_World_wrldbnd)
#case (BORDER_FENCE)
//Fence()
#break
#case (BORDER_TELEPORT)
//TeleportBoundary()
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
#debug concat("camerrra",str(Loc.x,5,0),str(Loc.y,5,0),str(Loc.z,5,0))

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

