//
// framsticks-wookie.inc
//
// 
// PCMHAIR macro by Chris Colefax http://www.geocities.com/ccolefax/pcm.html
// framsticks-wookie by Marek Lange, fixes by MacKo




/*                                 What you need

   Include Files: PCMHAIR.MCR, PCM.MCR
   Mesh files: sfera.pcm, sfera2.pcm, sfera3.pcm, sfera4.pcm, torus.pcm, walec.pcm, walec1.pcm
*/



//                                      TODO
/*
  0. Changing parameters like hair_length etc. doesn't seem to make any difference. However, when adding macro calls into this file
     (smooth_triangle_hairs_object(), clipped_sphere_hair_object(), triangle_patch_hairs_object(), triangle_cluster_hairs_object()),
     clearly visible long/large hairs are rendered. Even in that situation, looks like hair parameters can't be adjusted correctly :|
  1. Rendering optimization: if the camera is far, use simpler shapes! This will increase rendering speed a lot.
  2. Adjust other sensors and receptors to match wookie style? energy as well?
  3. Ground as grass
  4. Lots of ideas to make them look really animal-like!
*/



/*
-----------------------------------------------------------------------------
               HAIR GROWTH MACRO FILE - QUICK OPTIONS REFERENCE
-----------------------------------------------------------------------------
        for use with the Compressed Mesh Macro File for POV-Ray 3.1

Option Name                     Type/Description                      Default
-----------------------------------------------------------------------------
GENERAL OPTIONS:
 debug_progress         Prints progress info. to debug stream           false
 hair_objects_only      If true, no mesh surface created                false
 hair_mesh_transform    transform {} value for entire mesh and hairs      -

-----------------------------------------------------------------------------
HAIR OBJECT OPTIONS AND HAIR OBJECT MACROS:
 hair_object                       Single object                          -
 hair_objects                      One dimensional array of objects       -

 smooth_triangle_hairs_object()    Macro to set hair_object option       Yes
 clipped_sphere_hair_object()      Macro as above                         -
 triangle_patch_hairs_object()     Macro as above                         -
 triangle_cluster_hairs_object()   Macro as above                         -

-----------------------------------------------------------------------------
HAIR OBJECT MACRO OPTIONS (not all options apply to all four macros):
 hair_radius                Radius of each hair's curve (POV units)      1
 hair_thickness             Max. Thickness of each hair (POV units)      0.02
 hair_thickness_curvature   0 = flat, 1 = highly curved                  0.5
 hair_rotation              Vector, <0,0,0> = perpendicular/sky dir.  <0,0,0>
 hair_triangle_smoothness   Number of triangles per each arc            30
 hairs_per_patch            Number of hairs in single hair object       10
 hair_patch_size            Diameter of patch base (POV units)           0.5
 hair_patch_turb            0 = no size/rotation randomness              0.1
 hair_arc                   Arc length, 1 = 180 degrees                  0.25
 hair_length                Direct length instead of arc (POV units)     0.2

-----------------------------------------------------------------------------
HAIR COVERAGE OPTIONS:
 hair_objects_per_triangle  Hair objects per individual triangle;         1
 hair_object_count          Or, total number of hairs objects created;    -
 hair_objects_per_unit      Or, hair objects per square unit of mesh      -

-----------------------------------------------------------------------------
HAIR ROTATION AND TURBULENCE OPTIONS:
 hair_rotate_to_surface     Choose false, true, -true, or vector        false
 hair_sky                   Rotates hair to direction given              +y
 hair_seed                  Seed for all random aspects of hair          0
 hair_size_turb             0 = no size randomness                       0.1
 hair_rotation_turb         0 = no rotation randomness                   0.1
 hair_wind_rotation         Sets max angle for hair bent by wind         0
 hair_wind_speed            Sets wind rotation cycles per clock unit     1

-----------------------------------------------------------------------------
HAIR TEXTURE AND MATERIAL OPTIONS:
 hair_texture           Applied to each hair object untransformed         -
 hair_pattern_pigment   Each hair uses single colour from pigment map     -
 hair_material          Applied instead of above two options              -

 apply_hair_materials_to_mesh                                           false

-----------------------------------------------------------------------------
*/



#include "colors.inc"
#include "textures.inc"
#include "woods.inc"  
#include "PCMHAIR.MCR" // hair macro

#debug concat("Animation Frame #",str(AnimFrame,5,0))

////////////////////////////////
// you can override these settings in your "framsticks-user.inc" file:

#declare UseAreaLight=0;
#declare SmoothHeightfield=1;
#declare SimpleGround=0;
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
                       
                       
///////////////// 
//mark hairy parts  
///////////////
#declare HairyStickEnd = 1;
#declare HairyIngestion = 1;
#declare HairyReceptT = 1;
#declare HairyMuscle1 = 1;
#declare HairyMuscle2 = 1;
#declare HairyStick = 1; 
#declare HairyStaminaRod = 1;                      
                       
//////////////////////////////////////////
// colours
///////////////////////////////////////////
// stick:
#declare OldStickRGB = <0.65,0.65,0.55>;
#declare OldStick2RGB = <0.55,0.55,0.45>;
#declare StickRGB = <0.46,0.57,0.42>;
#declare StickColor = SemiSweetChoc;
#declare StaminaRGB = <0.53,0.65,0.50>;
#declare IngestRGB = <0.93,0.93,0>;
#declare AsimilRGB = <0.14,1,0.14>; 
#declare AsimilColor = MediumWood;

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

#declare Ground0Texture = texture
	{ pigment
#if (SimpleGround)
   {color rgb <1,1,1>}      //white
#else
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

#macro CreatureStickEnd(Location,Rotation)    

 
Reset();

#if (HairyStickEnd)
        /*StickEND settings */
        #declare apply_hair_materials_to_mesh = true;
        #declare hair_pattern_pigment = pigment {color SemiSweetChoc} //bozo color_map {[0 SemiSweetChoc ] [1 MediumWood]}}
        #declare hair_length = 0.1;   
        #declare hair_thickness = 0.005; 
        #declare hair_patch_size = 0.01;    
        #declare hair_objects_per_triangle = 10; 
        //#declare air_patch_turb=0.9;
        #declare hair_rotate_to_surface = true;
        #declare hair_arc = 0.025 ;

        /*STICKEND declaration */
        #declare StickEnd = object {read_mesh ("sfera2.pcm")}; 
#end

Reset();

#if (HairyReceptT)
        /* HairyRecept_T settings */  
        #declare apply_hair_materials_to_mesh = true;
        #declare hair_pattern_pigment = pigment {bozo color_map {[0 Black ] [1 Black]}}
        #declare hair_length = 0.01; //0.1;   
        #declare hair_thickness = 0.01;//0.005; 
        #declare hair_patch_size = 0.01;    
        #declare hair_objects_per_triangle = 0.008; 
        #declare air_patch_turb=0.9;
        #declare hair_rotate_to_surface = false;
        #declare hair_arc = 0.1 ;   
        #declare hair_rotation_turb =1;

         /*HairyRecept_T declaration */
        #declare TouchReceptor = object {read_mesh ("sfera3.pcm")};
        
        //reset settings
        #declare hair_rotation_turb = 0.1;
#end

Reset();

#if (HairyIngestion)
        /* HairyIngestionSettings)*/
        #declare apply_hair_materials_to_mesh = true;
        #declare hair_pattern_pigment = pigment {bozo color_map {[0 CoolCopper] [1 NewTan]}}
        #declare hair_length = 0.01; //0.1;   
        #declare hair_thickness = 0.01;//0.005; 
        #declare hair_patch_size = 0.01;    
        #declare hair_objects_per_triangle = 2; //0.5; 
        #declare air_patch_turb=0.9;
        #declare hair_rotate_to_surface = true;
        #declare hair_arc = 0.02 ;   
        #declare hair_rotation_turb =1;
        
        /* GreenBall Declarations */
        #declare GreenBall = object {read_mesh ("sfera4.pcm")};
        
        //settings reset
        #declare hair_rotation_turb = 0.1;
#end

union { 

#if (HairyStickEnd)
object  { StickEnd translate <0,-0.145,0> scale <1,1,1> }
#else
sphere { <0,0,0>,0.15
 texture
   {
   pigment { color StickRGB }
   normal { bumps 1.5*field_p_fr scale 0.01 }
   finish { phong (1-field_p_fr) }
   }
       }
#end

  
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
    #if (HairyReceptT)
        object { TouchReceptor translate <0.3,0.14,0> } 
    #else
        sphere { <0.3,0.15,0>,0.02 } 
    #end 
    texture { pigment { color rgb <0.1,0.1,0.1> } 
      finish { phong 1 } }
    rotate <45+120*N,0,0>
    }
 #end
#end // St_Recept_T

#if (field_p_ing > 0.05)
 #local N=field_p_ing*50;
 #local R=seed(123);
 #while (N>0)
  #local N=N-1;
  #if (HairyIngestion)
          object {GreenBall translate <0,0,0.14> rotate <0,360*rand(R),360*rand(R)> }
  #else
          sphere { <0,0,0.14> 0.03
          texture { 
        	pigment { color rgb IngestRGB }
        	finish { phong 1 }
        	}
           rotate <0,360*rand(R),360*rand(R)>
            }
  #end 
 #end
#end
rotate Rotation*57.3
translate Location
}
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

//////////////////////////////////////////////////////////////////////////////////////
/* reset */
        #declare hair_radius                    = 1;
        #declare hair_thickness                 = 0.02;
        #declare hair_thickness_curvature       = 0.5;
        #declare hair_rotation                  = <0,0,0>;
        #declare hair_triangle_smoothness       = 30;
        #declare hairs_per_patch                = 10;
        
        #declare hair_patch_size                = 0.5;
        #declare hair_patch_turb                = 0.1;
        #declare hair_arc                       = 0.25;
        #declare hair_length                    = 0.2;
        #declare hair_objects_per_triangle      = 1;
        //#declare hair_object_count              = 700;  //        Or, total number of hairs objects created;    -
        //#declare hair_objects_per_unit          = 0;    // Or, hair objects per square unit of mesh      -
        
        #declare hair_rotate_to_surface         = false;
        #declare hair_sky                       = +y;
        #declare hair_seed                      = 0 ;
        #declare hair_size_turb                 =0.1;
        #declare hair_rotation_turb             = 0.1;
        #declare hair_wind_rotation             = 0;
        #declare  hair_wind_speed               = 1;
////////////////////////////////////////////////////////////////////////////////////

#if (HairyMuscle1)
        /*Muscle Settings*/        
        #declare apply_hair_materials_to_mesh = true;
        #declare hair_pattern_pigment =         
//        pigment {bozo color_map {[0 Quartz] [1 Wheat]}} //didnt like white/gray muscles
pigment
 {
 gradient x   frequency 6   turbulence 0.1   ramp_wave
 color_map {[0 color Muscle1RGB]  [0.2 color Muscle2RGB]  [1 color Muscle3RGB]}
 scallop_wave   rotate <0,0,90>
 }      
        //#declare hair_length = 0.8; //0.1;   
        //#declare hair_thickness = 0.1;//0.005; 
        #declare hair_patch_size = 0.01;    
        #declare hair_objects_per_triangle = 5; 
        #declare air_patch_turb=0.1;
        #declare hair_rotate_to_surface = false;
        #declare hair_arc = 0.01 ;   
        #declare hair_rotation_turb =1;
        /*Muscle settings end*/
        
        #declare Muscle1 = object {read_mesh ("sfera4.pcm")}  
        #declare Muscle2 = object {read_mesh ("walec2.pcm")}   
        
        #declare Muscle = union {
        object {Muscle1 rotate <0,0,-90> translate <0,0,0> scale <1,1.9,1.9>  scale 0.8    } // translate <0,-0.145,0> } 
        object {Muscle2 rotate <0,0,-90> translate <-0.04,0.0,0> scale <2,2,2> scale 0.8   } // translate <0,-0.145,0> }                            
        }  
#else      
        #declare Muscle = union
        {
        cylinder {<0,0,0>,<1,0,0>,0.1}
        sphere {<0,0,0>,0.1}
        texture {MuscleTexture}
        } 
#end

//////////////////////////////////////////////////////////////////////////////////////
/* reset */
        #declare hair_radius                    = 1;
        #declare hair_thickness                 = 0.02;
        #declare hair_thickness_curvature       = 0.5;
        #declare hair_rotation                  = <0,0,0>;
        #declare hair_triangle_smoothness       = 30;
        #declare hairs_per_patch                = 10;
        
        #declare hair_patch_size                = 0.5;
        #declare hair_patch_turb                = 0.1;
        #declare hair_arc                       = 0.25;
        #declare hair_length                    = 0.2;
        #declare hair_objects_per_triangle      = 1;
        //#declare hair_object_count              = 700;  //        Or, total number of hairs objects created;    -
        //#declare hair_objects_per_unit          = 0;    // Or, hair objects per square unit of mesh      -
        
        #declare hair_rotate_to_surface         = false;
        #declare hair_sky                       = +y;
        #declare hair_seed                      = 0 ;
        #declare hair_size_turb                 =0.1;
        #declare hair_rotation_turb             = 0.1;
        #declare hair_wind_rotation             = 0;
        #declare  hair_wind_speed               = 1;
//////////////////////////////////////////////////////////////////////////////////// 
  
#if (HairyMuscle2)  
        /*SpeedMuscle Settings*/          
        #declare apply_hair_materials_to_mesh = true;
        #declare hair_pattern_pigment = 
//        pigment {bozo color_map {[0 Quartz] [1 Wheat]}} //didnt like white/gray muscles
pigment
 {
 gradient x   frequency 6   turbulence 0.1   ramp_wave
 color_map {[0 color Muscle1RGB]  [0.2 color Muscle2RGB]  [1 color Muscle3RGB]}
 scallop_wave   rotate <0,0,90>
 }
        #declare hair_length = 1; //0.1;   
        #declare hair_thickness = 1;//0.005; 
        #declare hair_patch_size = 0.1;    
        #declare hair_objects_per_triangle = 10; 
        //#declare air_patch_turb=0.1;
        #declare hair_rotate_to_surface = false;
        //#declare hair_arc = 0.2 ;   
        #declare hair_rotation_turb =0.1;  
        #declare Muscle3 = object {read_mesh ("torus2.pcm")} 
        #declare MuscleTorus = object {Muscle3 rotate <0,0,90> translate <0.13,0,0> scale <1.6,1.7,1.6> scale 0.8}
       
        //reset
        #declare hair_rotation_turb = 0.1;
#end

#macro CreatureStick(Location1,Location2,Rotation)  


#local Chlorofil = pigment
  {
  gradient x
  frequency 2
  turbulence 2
  color_map {[0 StickColor]  [0.95-St_Asimil StickColor]  [1.05-St_Asimil AsimilColor]}
  triangle_wave
  }
#local StickTexture=texture
{
pigment {Chlorofil}
finish { phong 1 }
}  



Reset();
#if (HairyStick)
        #declare apply_hair_materials_to_mesh = true;
        #declare hair_pattern_pigment = pigment {bozo color_map {[0 SemiSweetChoc ] [0.4-St_Asimil SemiSweetChoc] [1.05-St_Asimil DarkOliveGreen]}}
        #declare hair_length = 0.5;   
        #declare hair_thickness = 0.005; 
        #declare hair_patch_size = 0.01;    
        #declare hair_objects_per_triangle = 100; //0 ;// 100; 
        //#declare air_patch_turb=0.9;
        #declare hair_rotate_to_surface = false;
        #declare hair_arc = 0.025 ;
        #declare MyTube = object {read_mesh ("walec2.pcm")}
#end


union
{


#if (HairyStick)   
        object { MyTube rotate < 0,0,-90> translate < 0, 0 , 0> scale <1,1.6,1.6> }//scale <0.1,0.1 ,0.15>  } // wzor <0,-0.05,-0.05>,<1,0.05,0.05>
#else
        cylinder { <0,0,0>,<1,0,0>,0.1 texture {StickTexture} }
#end
#if (St_Recept_G)  

union { difference {
#end
 //
 #if (HairyStick)   
        object { MyTube rotate < 0,0,-90> translate < 0, 0 , 0> scale <1,1.6,1.6> }//scale <0.1,0.1 ,0.15>  } // wzor <0,-0.05,-0.05>,<1,0.05,0.05>
#else
        cylinder { <0,0,0>,<1,0,0>,0.1 texture {StickTexture} }
#end
  //cylinder { <0,0,0>,<1,0,0>,0.1 texture {StickTexture} }   // srednica 0.1

 #if (St_Recept_G)
 cylinder { <0.38,0,0>,<0.62,0,0>,0.10 }
  }                                             // 0.11
 cylinder { <0.38,0,0>,<0.62,0,0>,0.10                 //0.11
  texture
    {
    pigment  {color rgbf <1.0, 1.0, 1.0, 0.9>}
    //finish   {ambient 0   diffuse 0   reflection 0.5   refraction 1   phong 0.3   phong_size 60}
    finish   {ambient 1   diffuse 0.5   reflection 0.0   refraction 1 }
    }
   interior {ior 1.4}
          }
 sphere { <0.5,0,0>,0.03
          texture { pigment { color rgb <1,0,0> } finish { phong 1 } }
        }
 }  
  
#end // Recept_G

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

#if (St_RotMuscle)  
#if (HairyMuscle2)
MuscleTorus 
#else

 torus { 0.12, 0.1*St_Speed
texture
{
  pigment
   {
   radial
   frequency 25
   turbulence 0.1
   ramp_wave
    color_map { [ 0 color Muscle1RGB ]  [ 0.2 color Muscle2RGB ]  [ 1 color Muscle3RGB ] }
   }

   finish { phong 0.6 }
   } 
   rotate <0,0,90>
   translate <0.13,0,0>
  } 
#end    
#end



#declare hair_pattern_pigment = pigment {bozo
  color_map {[0 Silver ] [1 Silver]} }

Reset();

#if (HairyStaminaRod)
        #declare apply_hair_materials_to_mesh = true;
        #declare hair_pattern_pigment = pigment {color Bronze2} //bozo color_map {[0 White] [1 MediumSpringGreen]}}
        #declare hair_length = 0.5;   
        #declare hair_thickness = 0.005; 
        #declare hair_patch_size = 0.01;    
        #declare hair_objects_per_triangle = 10; 
        //#declare air_patch_turb=0.9;
        #declare hair_rotate_to_surface = false;
        #declare hair_arc = 0.025 ;                     
                                
                                                    
        #declare MyTube1 = object {read_mesh ("walec2.pcm")} 
#end  
 
#if (field_j_stam>0.05)
 #local N=6;
 #while (N>0)
  #local N=N-1;
  
  #if (HairyStaminaRod)
 object { MyTube1 
        rotate < 0,0,-90> 
        scale <1,field_j_stam*1,field_j_stam*1>
        translate < 0,0,0.1>  
        rotate <60*N,0,0> 
         } 
 #else     
  cylinder 
   { <0,0,0.1>,<1,0,0.1>,field_j_stam*0.04
   rotate <N*60,0,0>
  texture { pigment { color rgb StaminaRGB } }
   }
 #end
    
 #end
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
#if (i<n)
#local zrx=MapData[i+1][j];
#else
#local zrx=MapData[i][j];
#end
#if (j<n)
#local zry=MapData[i][j+1];
#else
#local zry=MapData[i][j];
#end
<(zlx-zrx)/d,(zly-zry)/d,2.0>
#end

#macro MakeSmoothWorld()
#local i=0;
#local j=0;
#local X=0;
#local Y=0;
#local n=dimension_size(MapData,1)-1;
#local d=field_World_wrldsiz/n;
mesh {
#while (i<n)
  #declare j=0;
  #declare Y=0;
  #while (j<n)

#if (SmoothHeightfield)
smooth_triangle { <X,Y,MapData[j][i]>,SmoothMapNormal(j,i),
		  <X+d,Y,MapData[j][i+1]>,SmoothMapNormal(j,i+1),
		  <X+d,Y+d,MapData[j+1][i+1]>,SmoothMapNormal(j+1,i+1) }
smooth_triangle { <X,Y,MapData[j][i]>,SmoothMapNormal(j,i),
		  <X+d,Y+d,MapData[j+1][i+1]>,SmoothMapNormal(j+1,i+1),
		  <X,Y+d,MapData[j+1][i]>,SmoothMapNormal(j+1,i) }
#else
triangle { <X,Y,MapData[j][i]>,<X+d,Y,MapData[j][i+1]>,<X+d,Y+d,MapData[j+1][i+1]> }
triangle { <X,Y,MapData[j][i]>,<X+d,Y+d,MapData[j+1][i+1]>,<X,Y+d,MapData[j+1][i]> }
#end

    #declare j=j+1; #declare Y=Y+d;
  #end
  #declare i=i+1; #declare X=X+d;
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


#macro Reset()     
//////////////////////////////////////////////////////////////////////////////////////
/* resetowanie resetowanie */
        #declare hair_radius                    = 1;
        #declare hair_thickness                 = 0.02;
        #declare hair_thickness_curvature       = 0.5;
        #declare hair_rotation                  = <0,0,0>;
        #declare hair_triangle_smoothness       = 30;
        #declare hairs_per_patch                = 10;
        
        #declare hair_patch_size                = 0.5;
        #declare hair_patch_turb                = 0.1;
        #declare hair_arc                       = 0.25;
        #declare hair_length                    = 0.2;
        #declare hair_objects_per_triangle      = 1;
        //#declare hair_object_count              = 700;  //        Or, total number of hairs objects created;    -
        //#declare hair_objects_per_unit          = 0;    // Or, hair objects per square unit of mesh      -
        
        #declare hair_rotate_to_surface         = false;
        #declare hair_sky                       = +y;
        #declare hair_seed                      = 0 ;
        #declare hair_size_turb                 =0.1;
        #declare hair_rotation_turb             = 0.1;
        #declare hair_wind_rotation             = 0;
        #declare  hair_wind_speed               = 1;
////////////////////////////////////////////////////////////////////////////////////
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
