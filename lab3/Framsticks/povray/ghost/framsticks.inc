//
// framsticks.inc       
//
// Framstics spooky style.       
//
// This style requires two files to render:  
// - label.gif - label on a blood bottle
// - rip.gif - caption on each grave
//
// Created by Michal Janiszewski, Poznan, 2007
//                   

#include "colors.inc"
#include "textures.inc"
#include "woods.inc"

#debug concat("Animation Frame #",str(AnimFrame,5,0))

////////////////////////////////
// you can override these settings in your "framsticks-user.inc" file:
////////////////////////////////

#declare UseAreaLight=1;
#declare SmoothHeightfield=1;
#declare SimpleGround=1;  
           
// style-specific settings: choose what you like!
#declare fog_distance = 5;     // fog distance                        
#declare ghost_type = 4;       // determines ghost interior; 1 normal, 4 brighter
#declare sky_type = 2;         // type of sky: 1 black; 2 purple.

////////////////////////////////     


#declare StandardCamera=camera { up <0,0,1> right <4/3,0,0> //sky <0,0,1>
location <0,0,0> look_at <1,1,1> }

#declare GlobalAmbient=3; //orig. 3
 

fog {
        fog_type 2         // ground fog      
        distance fog_distance Gray80         
        fog_offset  0.3    // the height below which the fog has constant density 
        fog_alt 0.45       // where the fog fades away
        up <0, 0, 10>        
}


global_settings
 {
 max_trace_level 100
 adc_bailout 0.1
 ambient_light GlobalAmbient
 assumed_gamma 2.0
 }
 

/////////////////////
// Colors
/////////////////////

/////////////////////
// muscle                             
#declare muscle_t = 0.95;
#declare Muscle1RGB= rgbt <0.1,0.1,0.1,muscle_t>;
#declare Muscle2RGB= rgbt <0,0,0,muscle_t>;
#declare Muscle3RGB= rgbt <0.3,0,0,muscle_t>;   


///////////////////////////////
// world:

#declare WORLD_FLAT=0;
#declare WORLD_SMOOTH=2;

#declare BORDER_NONE=0;
#declare BORDER_FENCE=1;
#declare BORDER_TELEPORT=2;

#declare OFFSET_Z=-0.2;

#declare Ground0Texture = texture
	{ pigment
                #if (SimpleGround)
                   {color rgb <1,1,1>}      //white
                #else
                   {color rgb <1,0.9,0.6>}
                    normal {bumps 0.2 scale 0.02}
                #end
	}

///////////////////////////////// 
#declare GhostTexture=texture
{
	pigment             
     {
	granite
	color_map
      { [ 0 color rgb <0.1,0.15,0.15> ]
		  [ 1 color rgb <0.36, 0.26, 0.25> ] }
	scale 2
    }
  finish {ambient 0.7/GlobalAmbient diffuse 0.3 }
}

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

#macro MoonLight()

//light source
//pinkish and quite dark, the darkness is used so we don't need a seperate absorption statement in the media,
//we can instead balance absorption vs scattering using darker lighting.
//one side effect of this is the moon need a brighter colour, and therefore relatively more of it's ambient colour will be visible.
//To make sure that ambient colour matches with the lighting on the clouds we tint the moon a little pink.
light_source { 6*z+1, .15+x/40 }

//the moon
sphere {
	//position relative to camera is in same direction as light, but further so it's illuminated by the light source.
	//this means we have to have a full moon, otherwise the light and moon directions wouldn't match.
	12*z+2, 1

	//slight pinkish tint to offset the blueish sky.
	//I'd rather have a blueish moon and orange sky, but couldn't fit it in 256 bytes	
	pigment { rgb 6+x }
	
	//normal makes the craters, colour is so bright the bumps saturate out to broad areas of colour, which looks more moon-like I reckon.
	normal { granite -0.9 }
}

#end // MoonLight

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

#macro PurpleSky()
 light_source{<1500,2500,-2500> color White}
// sky ---------------------------------------------------------------
// sky ---------------------------   

#declare skyb = 0.8   ;

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
           [0.00 rgb <0.2, 0.2, 0.1> * skyb]
           [0.50 rgb <0.55, 0, 0.67> * skyb]
           [0.70 rgb <0.40, 0, 0.50>* skyb]
           [0.85 rgb <0.27, 0, 0.33>* skyb]
           [1.0 rgb <0.05,0.05,0.05>* skyb]
    }
    scale <1.5, 0.25, 1.5> rotate z*5]
  [0.60
    bozo
    color_map {  
           [0.00 rgb <0.2, 0.2, 0.1> * skyb]
           [0.50 rgb <0.55, 0, 0.67> * skyb]
           [0.70 rgb <0.40, 0, 0.50>* skyb]
           [0.85 rgb <0.27, 0, 0.33>* skyb]
           [1.0 rgb <0.05,0.05,0.05>* skyb]
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

////////////////////////////////////////////////
// BLOOD BOTTLE
////////////////////////////////////////////////


#declare bottle = lathe {
	quadratic_spline
	15, 
	<0.00, 0.00>, <0.00, 0.00>, <0.50, 0.00>,
	<0.60, 0.09>, <0.60, 2.01>, <0.26, 2.58>,
	<0.21, 3.39>, <0.29, 3.49>, <0.28, 3.60>,
	<0.15, 3.61>, <0.15, 2.68>, <0.38, 2.48>,
	<0.53, 1.99>, <0.55, 0.34>, <0.00, 0.10>
	sturm
}     



#declare glassTexture = 
texture { 
  pigment { rgbf <0.8,0.8,0.9, 1> }//rgbf <0.8,0.6,0.3,0.95> }
  finish { ambient 0.1
           diffuse 0.1
           reflection 0.05
           specular 1
           roughness 0.0001 } }  
           
      

  
#declare label0 = 
texture { 
  pigment { image_map { gif "label.gif"
                        map_type 0 
                        interpolate 2 
                        once } 
            translate <-0.5,1,0>
            scale <217/283,1,1>
           }
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
//AddEnergyComponent(Location,2)

//{
 create_bottle(Location, R)
//}
//}
#end
#end
    
    
#macro create_bottle(Location, R)  // 0 < R <= 1 
	
 union {         
         
         union {          
                object { bottle
                 texture { glassTexture }
                 texture { label0 }
                 interior { ior 1.04 }   
                 
                 scale 0.4
                translate <0,-0.9,0.5>           
               }
                  
                  light_source { 0 color <1,0,0> shadowless 
	        fade_distance 3 fade_power 2 looks_like  {
	       
                 // blood        
                 cylinder {
                 <0,0,0>, <0,2 * R,0>, 0.45
                 pigment { rgbt <1,0,0,0.5> } 
                 hollow
                 scale 0.4  
                    interior {
                      media {
                         intervals 1    // Used to specify the integer number 
                                         // of intervals used to sample the ray 
                                         // default is 10   
                         samples 1,1    // Specifies the minimum and maximum 
                                         // number of samples taken per interval. 
                                         // The default values are samples 1,1. 
                      
                         scattering  {4, rgb<1,0,0>}  // type 1=isotropic
                      }
                   }    
                   
                   finish {
                    ambient 0.1
                   }
                 translate <0,-0.9,0.5>                  
                } // blood
                
                } // light source look like
                }// light source
                
                       
        
        } // union 

         
         scale 0.5 
         rotate <90,0,0>
         
         translate Location + <0,0,0.1>        
 } // union  
#end // create_bottle macro 

   
/////////////////////////////////////

#macro CreatureStickEnd(Location,Rotation)
union {    
        
        #if (field_p_ing > 0.05)
           intersection {
        #end
        
        #declare temp =  min(field_Creature_energy, 10000) / 10000;
        sphere { <0,0,0>,0.15
          hollow
          #declare StickRGB = rgbt <1,0.0,0.2,0.99 - temp * 0.23>;
          texture
           {
           pigment { color StickRGB 
           }
           normal { bumps 1.5*field_p_fr scale 0.01 }
         //  finish { phong (1-field_p_fr) }
           }
          interior {
              media {
                 intervals 1    // Used to specify the integer number 
                                 // of intervals used to sample the ray 
                                 // default is 10   
                 samples 1,1    // Specifies the minimum and maximum 
                                 // number of samples taken per interval. 
                                 // The default values are samples 1,1. 
              
                 scattering  {ghost_type, StickRGB}  // type 1=isotropic
              }
           } 
        }  
        
        #if (field_p_ing > 0.05)
         #local N=field_p_ing*50;
         #local R=seed(123);
         #while (N>0)
          #local N=N-1;
          sphere { <0,0,0.14> 0.03
          texture { 
        	pigment { color  rgbt <0.93,0.0,0,0.95> }
        	finish { phong 0.3 }
        	}
          interior {
              media {
                 intervals 1    // Used to specify the integer number 
                                 // of intervals used to sample the ray 
                                 // default is 10   
                 samples 1,1    // Specifies the minimum and maximum 
                                 // number of samples taken per interval. 
                                 // The default values are samples 1,1. 
              
                 scattering  {ghost_type, rgb<1,1,0>}  // type 1=isotropic
              }               
           }
             inverse              	
           rotate <0,360*rand(R),360*rand(R)>
            } 
            
             
         #end // while 
         }      // intersection
        #end   // if 
   
#declare smell_texture = texture {
  pigment { color rgbt <0,0,0, 0.98> }                                 
}   
   
// smell receptor -> skin with hobnails 
#if (St_Recept_S)
difference { 
   cylinder { <-0.15,0,0>,<-0.3,0,0>,0.13  texture { smell_texture } 
   }
   
                                                                             
   cylinder { <-0.15,0,0>,<-0.3,0,0>,0.12  texture { smell_texture }    
   
   }
                                                                         

#local N=10;
#while (N>0)
 #local N=N-1;   

  union {
  cone {
          <-0.19, 0.12, 0>, 0.005, <-0.19, 0.14, 0>, 0
          texture{ Chrome_Metal  }
          rotate x*N*36 
  }  

  cone {
          <-0.23, 0.12, 0>, 0.005, <-0.23, 0.14, 0>, 0
          texture{ Chrome_Metal  }
        rotate x*(18+N*36)
  }  

  cone {
          <-0.27, 0.12, 0>, 0.005, <-0.27, 0.14, 0>, 0
          texture{ Chrome_Metal  }
          rotate x*N*36 
  }   

  hollow  
  interior {
      media {
         intervals 1    // Used to specify the integer number 
                         // of intervals used to sample the ray 
                         // default is 10   
         samples 1,1    // Specifies the minimum and maximum 
                         // number of samples taken per interval. 
                         // The default values are samples 1,1. 
      
         scattering  {1, rgbt  <1,1,1,0.99>}  // type 1=isotropic
      }
   }    // union 
}

#end

 interior {
      media {
         intervals 1    // Used to specify the integer number 
                         // of intervals used to sample the ray 
                         // default is 10   
         samples 1,1    // Specifies the minimum and maximum 
                         // number of samples taken per interval. 
                         // The default values are samples 1,1. 
      
         scattering  {ghost_type, rgbt <1, 0.2, 0.2, 0.99>}  // type 1=isotropic
      }
   }  
   
 } // difference
#end // Recept_S
                                      
#declare Chrome_Metal_transparent = 
texture {
    pigment { rgbt <0.658824, 0.658824, 0.658824, 0.75> }
    finish {
        ambient 0.3
        diffuse 0.7
        reflection 0.15
        brilliance 8
        specular 0.8
        roughness 0.1   
        phong 1
    }                              
    
}
         
 // sharp, chrome cone         
#if (St_Recept_T)
 #local N=6; 
 #while (N>0)
  #local N=N-1;  
  union {       
    #if (mod(N,2)=0) 
        #local h = 0.3;    
    #else
        #local h = 0.15;    
    #end
    cone { <0,0.1,0>, 0.015, <h,0.15,0>, 0.0 }
    texture { Chrome_Metal_transparent } 
      finish { phong 1 }    
    interior {
      media {
         intervals 1    // Used to specify the integer number 
                         // of intervals used to sample the ray 
                         // default is 10   
         samples 1,1    // Specifies the minimum and maximum 
                         // number of samples taken per interval. 
                         // The default values are samples 1,1. 
      
         scattering  {ghost_type, rgb<1,1,1>}  // type 1=isotropic
      }
   }  
    rotate <45+60*N,0,0>
    }
 #end
#end // St_Recept_T
 


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

#declare Muscle = union
{
cylinder {<0,0,0>,<1,0,0>,0.1}
sphere {<0,0,0>,0.1}
texture {MuscleTexture}  

   interior {
      media {
         intervals 1    // Used to specify the integer number 
                         // of intervals used to sample the ray 
                         // default is 10   
         samples 1,1    // Specifies the minimum and maximum 
                         // number of samples taken per interval. 
                         // The default values are samples 1,1. 
      
         scattering  {ghost_type, rgb<1,1,1>}  // type 1=isotropic
      }
   }

}

#macro CreatureStick(Location1,Location2,Rotation)
union
{
//#if (St_Recept_G)
//union { difference {
//#end      
  
//difference { 
 //difference {
 union {       
        //threshold .99
         cylinder { 
                 <0.14,0,0>, <0.86,0,0>, 0.1
                 }     
       hollow
 //texture {StickTexture} 
 pigment {rgbt <1,1,1,1>}
 interior {
      media {
         intervals 1    // Used to specify the integer number 
                         // of intervals used to sample the ray 
                         // default is 10   
         samples 1,1    // Specifies the minimum and maximum 
                         // number of samples taken per interval. 
                         // The default values are samples 1,1. 
      
         scattering  {ghost_type, rgb<1,1,1>}  // type 1=isotropic
      }
   }
 }   // union
      

/*#if (St_Recept_G)      // Gyroscope
 // cylinder { <0.38,0,0>,<0.62,0,0>,0.11 }
  //}
 cylinder { <0.38,0,0>,<0.62,0,0>,0.11
  texture
    {
    pigment  {color rgbf <1.0, 1.0, 1.0, 0.99>}
    finish   {ambient 0   diffuse 0   reflection 0.5   refraction 1   phong 0.3   phong_size 60}
    }
   interior {ior 1.4}
          }
 sphere { <0.5,0,0>,0.03
          texture { pigment { color rgbt <1,0,0,0.99> } finish { phong 1 } }
        }
 //}
#end // Recept_G*/

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

#if (field_j_stam>0.05)
 #local N=6;
 #while (N>0)
  #local N=N-1;
  cylinder 
  { <0.1,0,0.1>,<0.9,0,0.1>, 0.04 * 0.25
   
   rotate <N*60,0,0>
   
//   texture { 
     pigment { color rgbt <1, 0, 0, max((1 - field_j_stam), 0.5) >}  
     hollow 
     interior {
           media {
           intervals 1    // Used to specify the integer number 
                          // of intervals used to sample the ray 
                         // default is 10   
           samples 1,1    // Specifies the minimum and maximum 
                         // number of samples taken per interval. 
                         // The default values are samples 1,1. 
      
           scattering  {ghost_type, rgb <1,0,0> }// <0.8,0.65,0.50> }  // type 1=isotropic
          }
        }
 //  }
  }
 #end   
#end     
    
 
scale <vlength(Location2-Location1),1,1>
rotate Rotation*57.3
translate Location1
}
#end


////////////////////////////////////////////////
// GRAVE
////////////////////////////////////////////////  
  
// R.I.P caption       
#declare rip = box { <0,0,0> <1,1,0.01>
      pigment{
                 image_map{ gif "rip.gif"
                            map_type 0
                            once
                            transmit 255, 1
                          }
                          
                          //rotate 180
               //  translate<-0.5,-0.5,0>
                 scale<1,1,1>*1                 
                 } // end pigment
     rotate 90                 
              
   translate <0.1581,-0.75, +0.48>  
       
} // rip
            
// GRAVE            
#declare grave = union {  // CSG1
  intersection {  // CSG0 
    union {
            box {  // Box2
              < -0.158000, -0.500000, -0.500000 >, < 0.158000, 0.500000, 0.500000 >
              translate <0.000000,-0.500000,0.000000>   
                          
               
            }
        
            box {  // Box1
              < -0.158000, -0.500000, -0.500000 >, < 0.158000, 0.500000, 0.500000 >
              translate <0.000000,0.500000,0.000000>
              

      
            }      
    }

        sphere {
  0.0, 0.5 
        }

  }   // intersecion

   box {  // Box1
              < -0.162000, -0.500000, -0.500000 >, < 0.158000, 0.500000, 0.500000 >
              translate <0.000000,-0.500000,0.000000>
  }                                                 
  
  object {
    rip 
 
  } 
}       

  
#macro GetGroundTexture(Height)
        #if (Height>field_World_wrldwat)
        #if (Height>(field_World_wrldwat+2))
        texture {GhostTexture}//{HighTexture}
        #else
        texture {GhostTexture}//{Ground0Texture}
        #end
        #else
        texture {GhostTexture}//{UnderwaterTexture}
        #end
#end

#macro CommonWorldObjects()
        #if (sky_type = 2)
                PurpleSky()
                StandardLight()
        #else 
                StandardSky()  
                MoonLight()  
                StandardLight() 
        #end
#end

#declare field_World_wrldtyp=0;
#declare field_World_wrldwat=0;
#declare field_World_wrldbnd=0;

#macro BeginWorld()
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

	
#end     

#local counter = 40;
#local R = seed(452);
#local sign = 1;
# while (counter > 0)
        #local counter = counter - 1;
        object {
          grave
          #local R = seed(R*100);
          texture { PinkAlabaster scale 0.05}
          #if (counter = 10)
          #local sign = -1;
          #end
          translate <(sign * field_World_wrldsiz) * rand(R),0.75,1+ (sign * field_World_wrldsiz) * rand(R)>               
          #local R2 = seed(152);
          rotate <90,0,45 + rand (R)* 360>
        }                     
                
#end //while

     
        
        

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
camera { up <0,0,1> right <4/3,0,0> sky <0,0,1>
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
#declare first = 1;
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
