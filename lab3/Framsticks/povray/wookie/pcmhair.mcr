/*************************************************************************
          HAIR GROWTH MACRO FILE FOR PERSISTENCE OF VISION 3.1
**************************************************************************

Created by Chris Colefax, 22 January 1999

Updated 13 May 1999: Added support for compressed bicubic patch surfaces;
   Added hair_wind_rotation and hair_wind_speed options.

Updated 20 May 1999: Updated for Warp's Mesh Compressor 1.2a

Updated 28 Dec 1999: Updated for Warp's Mesh Compressor 2.1

NOTE: This file requires that PCM.MCR be installed

*************************************************************************/

#ifndef (read_mesh) #include "pcm.mcr" #end

// MAIN HAIR CREATION MACRO FOR create_mesh ()
// *******************************************
#ifdef (create_mesh) #warning "PCMHAIR.MCR: The existing create_mesh () macro has been redefined to use create_hair_mesh ()!\n" #end
#macro create_mesh () create_hair_mesh () #end

#macro create_hair_mesh ()
   #ifdef (hair_objects) #local HOA = dimension_size(hair_objects, 1); #local HOAP = 0;
      #else #local HOA = 0; #ifndef (hair_object) smooth_triangle_hairs_object () #end #end
   #ifndef (hair_objects_only) #local hair_objects_only = false; #end
   #ifndef (hair_rotate_to_surface) #local hair_rotate_to_surface = false; #end
   #ifndef (hair_sky) #local hair_normal_sky = y; #else #local hair_normal_sky = vnormalize (hair_sky); #end
   #ifndef (hair_size_turb) #local hair_size_turb = .1; #end
   #ifndef (hair_rotation_turb) #local hair_rotation_turb = .1; #end
   #ifndef (hair_seed) #local HRand = seed(0); #else #local HRand = seed(hair_seed); #end
   #ifndef (apply_hair_materials_to_mesh) #local apply_hair_materials_to_mesh = false; #end
   #ifndef (hair_objects_per_unit)
      #ifdef (hair_object_count) #switch (VerID)
         #case (1) #local hair_objects_per_triangle = hair_object_count/(TriangleCount+SmoothTriangleCount); #break
         #case (2) #local hair_objects_per_patch = hair_object_count/PatchCount; #end
         #local HairCounter = 1;
      #else #ifndef (hair_objects_per_triangle) #local hair_objects_per_triangle = 1; #end
         #local HairCounter = 0;
      #end #end

   #if (hair_normal_sky.y = 1) #local HSkyRt = false; #else
      #local hair_normrot_x = vlength(hair_normal_sky*<1, 0, 1>); #if (hair_normrot_x != 0 | hair_normal_sky.y != 0) #local hair_normrot_x = degrees(atan2(hair_normrot_x, hair_normal_sky.y)); #end
      #local hair_normrot_y = hair_normal_sky.x; #if (hair_normrot_y != 0 | hair_normal_sky.z != 0) #local hair_normrot_y = degrees(atan2(hair_normrot_y, hair_normal_sky.z)); #end
      #local HSkyRt = true; #end
   #local HMt = defined(hair_material); #local HTx = defined(hair_texture); #local HPP = defined(hair_pattern_pigment); #local MTr = defined(hair_mesh_transform);
   #local HRtS = (vlength(hair_rotate_to_surface*<1, 1, 1>) = 0 ? false : true);

   #if (debug_progress) #debug "\n      PCMHAIR.MCR: Creating hair objects on mesh surface..." #end
   union { #local Area = 0; #local HairCount = 0; #local FlatNorm = <0, 0, 0>; #local Counter = 0;
   #switch (VerID)
   #case (1) #while (Counter < (TriangleCount+SmoothTriangleCount))
      #if (Counter < TriangleCount)
         #read (File, P1, P2, P3) #if (hair_objects_only = false) triangle {PointList[P1], PointList[P2], PointList[P3] #if (MTr) transform hair_mesh_transform #end} #end
         #if (HRtS) #local FlatNorm = vnormalize(hair_rotate_to_surface*vcross(PointList[P2]-PointList[P1], PointList[P3]-PointList[P2])); #end
         PCM_place_hair_objects (PointList[P1], FlatNorm, PointList[P2], FlatNorm, PointList[P3], FlatNorm)
      #else
         #read (File, P1, N1, P2, N2, P3, N3) #if (hair_objects_only = false) smooth_triangle {PointList[P1], PointList[N1], PointList[P2], PointList[N2], PointList[P3], PointList[N3]
         #if (MTr) transform hair_mesh_transform #end } #end
         PCM_place_hair_objects (PointList[P1], PointList[N1], PointList[P2], PointList[N2], PointList[P3], PointList[N3])
      #end
   #local Counter = Counter+1; #end #break

   #case (2) #while (Counter < PatchCount)
      #if (hair_objects_only = false) PCM_get_patch_grid (true) #else PCM_get_patch_grid (false) #end
      #ifdef (hair_object_count) #local hair_objects_per_triangle = hair_objects_per_patch/(PU*PV*2); #end
      #local U = 0; #while (U < PU) #local V = 0; #while (V < PV)
         #if (HRtS) #local FlatNorm = vnormalize(hair_rotate_to_surface*vcross(PCM_patch_grid[U+1][V]-PCM_patch_grid[U][V], PCM_patch_grid[U][V+1]-PCM_patch_grid[U+1][V])); #end
         PCM_place_hair_objects (PCM_patch_grid[U][V], FlatNorm, PCM_patch_grid[U+1][V], FlatNorm, PCM_patch_grid[U][V+1], FlatNorm)
         #if (HRtS) #local FlatNorm = vnormalize(hair_rotate_to_surface*vcross(PCM_patch_grid[U+1][V+1]-PCM_patch_grid[U+1][V], PCM_patch_grid[U][V+1]-PCM_patch_grid[U+1][V+1])); #end
         PCM_place_hair_objects (PCM_patch_grid[U+1][V], FlatNorm, PCM_patch_grid[U+1][V+1], FlatNorm, PCM_patch_grid[U][V+1], FlatNorm)
      #local V = V+1; #end #local U = U+1; #end
   #local Counter = Counter+1; #end #break

   #case (3) #while (Counter < TriangleCount)
         #if (Counter = 0) #read (File, P1, P2, P3) #declare Y = 0;
            #else #read (File, X) #if (X = 0) #read (File, P1, P2, P3) #declare Y = 0;
            #else #if (Y = 0) #declare Y = 1; #declare P1 = P3; #else #declare Y = 0; #declare P2 = P3; #end #declare P3 = X; #end #end
         #if (hair_objects_only = false) triangle {PointList[P1-1], PointList[P2-1], PointList[P3-1] #if (MTr) transform hair_mesh_transform #end} #end
         #if (HRtS) #local FlatNorm = vnormalize(hair_rotate_to_surface*vcross(PointList[P2-1]-PointList[P1-1], PointList[P3-1]-PointList[P2-1])); #end
         PCM_place_hair_objects (PointList[P1-1], FlatNorm, PointList[P2-1], FlatNorm, PointList[P3-1], FlatNorm)
      #local Counter = Counter+1; #end
      #local Counter = 0; #while (Counter < SmoothTriangleCount)
         #if (Counter = 0) #read (File, P1, P2, P3, N1, N2, N3) #declare Y = 0;
            #else #read (File, X) #if (X = 0) #read (File, P1, P2, P3, N1, N2, N3) #declare Y = 0;
            #else #if (Y = 0) #declare Y = 1; #declare P1 = P3; #declare N1 = N3; #else #declare Y = 0; #declare P2 = P3; #declare N2 = N3; #end #declare P3 = X; #read (File, N3) #end #end
         #if (hair_objects_only = false) smooth_triangle {PointList[P1-1], PointList[N1-1], PointList[P2-1], PointList[N2-1], PointList[P3-1], PointList[N3-1]
         #if (MTr) transform hair_mesh_transform #end } #end
         PCM_place_hair_objects (PointList[P1-1], PointList[N1-1], PointList[P2-1], PointList[N2-1], PointList[P3-1], PointList[N3-1])
      #local Counter = Counter+1; #end
   #end

   #if (apply_hair_materials_to_mesh) #declare apply_mesh_textures = false;
      #if (HMt) material {hair_material} #else #if (HTx) texture {hair_texture #end #if (HPP) pigment {hair_pattern_pigment} #end #if (HTx) } #end #end
   #end }

   #if (debug_progress) #debug concat("\r      PCMHAIR.MCR: Created ", str(HairCount, 0, 0), " hair objects on mesh surface...") #end
#end

// TRANSFORMATION MACROS FOR PLACING EACH HAIR OBJECT
// **************************************************
#macro PCM_place_hair_objects (V1, VN1, V2, VN2, V3, VN3)
   #ifdef (hair_objects_per_unit)
      #declare Area = Area+hair_objects_per_unit*1.25*vlength(vcross(V2-V1, V3-V2))/2;
      #declare HairCounter = 1; #while (HairCounter <= Area) PCM_position_hair_object () #declare HairCounter = HairCounter+1; #end
      #declare HairCount = HairCount+HairCounter-1; #if (HairCounter > 1) #declare Area = 0; #end
   #else
      #declare HairCounter = HairCounter + hair_objects_per_triangle; #while (HairCounter >= 1)
          PCM_position_hair_object () #declare HairCount = HairCount+1;
      #declare HairCounter = HairCounter-1; #end
   #end
#end

#macro PCM_position_hair_object ()
   #local W1 = rand(HRand); #local W2 = rand(HRand); #local W3 = rand(HRand); #local HTr = (W1*V1 + W2*V2 + W3*V3)/(W1+W2+W3);
   object {#if (HOA) hair_objects[HOAP] #declare HOAP = mod(HOAP + 1, HOA); #else hair_object #end
      scale 1+(rand(HRand)-.5)*hair_size_turb  rotate (<rand(HRand), rand(HRand), rand(HRand)>-.5)*hair_rotation_turb*360
      #if (HRtS) PCM_orientate_hair_object () #end  #if (HSkyRt) rotate <hair_normrot_x, hair_normrot_y, 0> #end
      translate HTr #if (MTr) transform hair_mesh_transform #end
      #if (HMt) material {hair_material} #else #if (HTx) texture {hair_texture #end #if (HPP) pigment {hair_pattern_pigment scale 100 translate HTr*-99} #end #if (HTx) } #end #end }
#end

#macro PCM_orientate_hair_object ()
   #local SN = vnormalize(hair_rotate_to_surface*(W1*VN1 + W2*VN2 + W3*VN3)/(W1+W2+W3));
   #if (HSkyRt) #local SN = vrotate(vrotate(SN, -y*hair_normrot_y), -x*hair_normrot_x); #end
   #if (SN.y != 1) #if (SN.y = -1) scale -1 #else #local XN = vnormalize(vcross(y, SN)); #local ZN = vnormalize(vcross(XN, SN)); matrix <XN.x, XN.y, XN.z, SN.x, SN.y, SN.z, ZN.x, ZN.y, ZN.z, 0, 0, 0> #end #end
#end


// HAIR OBJECT MACROS
// ******************
#macro smooth_triangle_hairs_object ()
   #ifdef (debug_progress) #if (debug_progress) #debug "\nPCMHAIR.MCR: Creating smooth_triangle_hairs_object ()..." #end #end
   #ifndef (hair_radius) #local hair_radius = 1; #end
   #ifndef (hair_thickness) #local hair_thickness = .02; #end
   #ifndef (hair_thickness_curvature) #local hair_thickness_curvature = .5; #end
   #ifndef (hair_rotation) #local hair_rotation = <0, 0, 0>; #end
   #ifndef (hair_wind_rotation) #local hair_wind_rotation = 0; #end
   #ifndef (hair_wind_speed) #local hair_wind_speed = 1; #end
   #ifndef (hair_triangle_smoothness) #local hair_triangle_smoothness = 30; #end
   #ifndef (hairs_per_patch) #local hairs_per_patch = 10; #end
   #ifndef (hair_patch_size) #local hair_patch_size = .5; #end
   #ifndef (hair_patch_turb) #local hair_patch_turb = .1; #end
   #ifndef (hair_arc) #ifdef (hair_length) #local hair_arc = hair_length/(pi*hair_radius); #else #local hair_arc = .25; #end #end
   #if (hairs_per_patch = 1) #local hair_patch_turb = 0; #local hair_patch_size = 0; #end

   #if (hair_arc <= 0) #declare hair_object_count = 0; #declare hair_object = sphere {0, 0} #else
   #local R1 = seed(0); #local HTh = hair_thickness/(2*hair_radius); #local HAn = degrees(pi*hair_arc);
   #local MC = max(4, hair_triangle_smoothness*hair_arc/2); #local HTC = hair_thickness_curvature;
   #declare hair_object = mesh { #local C1 = 0; #while (C1 < hairs_per_patch)
      #local Sc = hair_radius*(1+(rand(R1)-.5)*hair_patch_turb);
      #local Rt = hair_rotation + (<rand(R1), rand(R1), .5>-.5)*360*hair_patch_turb + vrotate(x, z*(rand(R1) < .5 ? 360 : -360)*(hair_wind_speed*clock+rand(R1)))*hair_wind_rotation*<1, .2, 0>;
      #local Tr = vrotate(x*rand(R1), y*rand(R1)*360)*hair_patch_size/2;
      #local CTh = HTh; #local CRt = <0, 0, 0>; #local C = 0; #while (C < MC)
         #local OTh = CTh; #local ORt = CRt; #local CTh = HTh*pow(1-(C/MC), .5); #local CRt = x*(HAn*C/MC);
         smooth_triangle {vrotate((vrotate(<-OTh, 0, -1>, ORt)+z)*Sc, Rt)+Tr, vrotate(<-HTC, 0, -1>, ORt+Rt), vrotate((vrotate(<-CTh, 0, -1>, CRt)+z)*Sc, Rt)+Tr, vrotate(<-HTC, 0, -1>, CRt+Rt), vrotate((vrotate(<OTh, 0, -1>, ORt)+z)*Sc, Rt)+Tr, vrotate(<HTC, 0, -1>, ORt+Rt)}
         smooth_triangle {vrotate((vrotate(<-CTh, 0, -1>, CRt)+z)*Sc, Rt)+Tr, vrotate(<-HTC, 0, -1>, CRt+Rt), vrotate((vrotate(<CTh, 0, -1>, CRt)+z)*Sc, Rt)+Tr, vrotate(<HTC, 0, -1>, CRt+Rt), vrotate((vrotate(<OTh, 0, -1>, ORt)+z)*Sc, Rt)+Tr, vrotate(<HTC, 0, -1>, ORt+Rt)}
      #local C = C+1; #end
      smooth_triangle {vrotate((vrotate(<-CTh, 0, -1>, CRt)+z)*Sc, Rt)+Tr, vrotate(<-HTC, 0, -1>, CRt+Rt), vrotate((vrotate(<CTh, 0, -1>, CRt)+z)*Sc, Rt)+Tr, vrotate(<HTC, 0, -1>, CRt+Rt), vrotate((vrotate(<0, 0, -1>, x*HAn)+z)*Sc, Rt)+Tr, vrotate(<0, 0, -1>, (x*HAn)+Rt)}
   #local C1 = C1+1; #end hollow} #end
   #ifdef (debug_progress) #if (debug_progress) #debug "done!\n" #end #end
#end

#macro clipped_sphere_hair_object ()
   #ifdef (debug_progress) #if (debug_progress) #debug "\nPCMHAIR.MCR: Creating clipped_sphere_hair_object ()..." #end #end
   #ifndef (hair_radius) #local hair_radius = 1; #end
   #ifndef (hair_thickness) #local hair_thickness = .02; #end
   #ifndef (hair_thickness_curvature) #local hair_thickness_curvature = .5; #end
   #ifndef (hair_rotation) #local hair_rotation = <0, 0, 0>; #end
   #ifndef (hair_arc) #ifdef (hair_length) #local hair_arc = hair_length/(pi*hair_radius); #else #local hair_arc = .25; #end #end
   #if (hair_arc > 1) #warning "PCMHAIR.MCR: The clipped_sphere_hair_object () cannot be longer than one arc!\n" #end

   #if (hair_arc <= 0) #declare hair_object_count = 0; #declare hair_object = sphere {0, 0} #else
   #local HTh = hair_thickness/2; #local HAn = pi*min(1, hair_arc);
   #declare hair_object = sphere {z*hair_radius, hair_radius scale <pow(HTh, .6)/(hair_radius*(hair_thickness_curvature+1e-3)), 1, 1>
      clipped_by {box {<-HTh, 0, 0>, <HTh, hair_radius*(hair_arc > .5 ? 1 : sin(HAn)), hair_radius*(1-cos(HAn))>}}
      hollow rotate hair_rotation} #end
   #ifdef (debug_progress) #if (debug_progress) #debug "done!\n" #end #end
#end

#macro triangle_patch_hairs_object ()
   #ifdef (debug_progress) #if (debug_progress) #debug "\nPCMHAIR.MCR: Creating triangle_patch_hairs_object ()..." #end #end
   #ifndef (hair_thickness) #local hair_thickness = .02; #end
   #ifndef (hair_length) #local hair_length = .2; #end
   #ifndef (hair_rotation) #local hair_rotation = <0, 0, 0>; #end
   #ifndef (hair_wind_rotation) #local hair_wind_rotation = 0; #end
   #ifndef (hair_wind_speed) #local hair_wind_speed = 1; #end
   #ifndef (hairs_per_patch) #local hairs_per_patch = 100; #end
   #ifndef (hair_patch_size) #local hair_patch_size = .5; #end
   #ifndef (hair_patch_turb) #local hair_patch_turb = .1; #end

   #if (hair_length <= 0) #declare hair_object_count = 0; #declare hair_object = sphere {0, 0}
   #else #local R1 = seed(0); #local HTh = hair_thickness/2;
   #declare hair_object = mesh { #local C = 0; #while (C < hairs_per_patch)
      #local Sc = 1+(rand(R1)-.5)*hair_patch_turb;
      #local Rt = hair_rotation + (<rand(R1), rand(R1), .5>-.5)*360*hair_patch_turb + vrotate(x, z*(rand(R1) < .5 ? 360 : -360)*(hair_wind_speed*clock+rand(R1)))*hair_wind_rotation*<1, .2, 0>;
      #local Tr = vrotate(x*rand(R1), y*rand(R1)*360)*hair_patch_size/2;
      triangle {vrotate(-x*HTh*Sc, Rt)+Tr, vrotate(x*HTh*Sc, Rt)+Tr, vrotate(y*hair_length*Sc, Rt)+Tr}
   #local C = C+1; #end hollow} #end
   #ifdef (debug_progress) #if (debug_progress) #debug "done!\n" #end #end
#end

#macro triangle_cluster_hairs_object ()
   #ifdef (debug_progress) #if (debug_progress) #debug "\nPCMHAIR.MCR: Creating triangle_cluster_hairs_object ()..." #end #end
   #ifndef (hair_thickness) #local hair_thickness = .02; #end
   #ifndef (hair_length) #local hair_length = .2; #end
   #ifndef (hairs_per_patch) #local hairs_per_patch = 100; #end

   #if (hair_length <= 0) #declare hair_object_count = 0; #declare hair_object = sphere {0, 0}
   #else #local R1 = seed(0); #local HTh = hair_thickness/2;
   #declare hair_object = mesh { #local C = 0; #while (C < hairs_per_patch)
      #local Rt = (<rand(R1), rand(R1), rand(R1)>-.5)*360; triangle {vrotate(-x*HTh, Rt), vrotate(x*HTh, Rt), vrotate(y*hair_length, Rt)}
   #local C = C+1; #end hollow} #end
   #ifdef (debug_progress) #if (debug_progress) #debug "done!\n" #end #end
#end
