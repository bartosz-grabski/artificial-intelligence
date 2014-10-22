/*************************************************************************
        COMPRESSED MESH MACRO FILE FOR PERSISTENCE OF VISION 3.1
**************************************************************************

Created by Chris Colefax, 23 September 1998

Updated 22 January 1999: Added mesh_texture, mesh_textures, apply_mesh_textures options;
   Added bend_mesh () deformation macro;
   Added cylinder_mesh () and triangle_mesh () creation macros.

Updated 13 May 1999: Added support for compressed bicubic patch surfaces;
   Removed quick_mesh () and triangle_mesh () macros.

Updated 20 May 1999: Updated for Warp's Mesh Compressor 1.2a

Updated 28 Dec 1999: Updated for Warp's Mesh Compressor 2.1

See "PCM.HTM" for more information

*************************************************************************/

#macro read_mesh (FileName)

// CHECK OPTIONS AND ASSIGN DEFAULTS
// *********************************
   #ifndef (debug_progress) #local debug_progress = false; #end
   #ifndef (declare_only) #local declare_only = false; #end
   #ifndef (max_mesh_count) #local max_mesh_count = 100; #end
   #ifndef (apply_mesh_textures) #local apply_mesh_textures = true; #end

   #if (declare_only) #declare Mesh = array[max_mesh_count] #end
   #if (debug_progress) #debug concat("Reading compressed mesh file \"", FileName, "\":\n") #end

// DEFINE EMPTY LOCAL VARIABLES (FOR #READ)
// ****************************************
   #local Version = "" #local PointCount = 0; #local TriangleCount = 0; #local SmoothTriangleCount = 0;
   #local X = 0; #local Y = 0; #local Z = 0; #local P1 = 0; #local P2 = 0; #local P3 = 0; #local N1 = 0; #local N2 = 0; #local N3 = 0;
   #local PT = 1; #local PF = .01; #local PU = 3; #local PV = 3; #local MeshCount = 0;

// CHECK FILE FORMAT
// *****************
   #fopen File FileName read #while (defined(File))
   #local VerID = 0; #read (File, Version)
   #if (strcmp(Version, "PCM1") = 0) #local VerID = 1; #end
   #if (strcmp(Version, "PCM2") = 0) #local VerID = 2; #end
   #if (strcmp(Version, "PCM3") = 0) #local VerID = 3; #end
   #if (VerID = 0) #warning "PCM.MCR: The mesh file you specified is not in the correct format!\n" #fclose File

// READ POINT LIST
// ***************
   #else #ifdef (File) #read (File, PointCount)
   #if (PointCount > 0) #local PointList = array[PointCount]
      #if (debug_progress) #debug concat("   Reading ", str(PointCount, 0, 0), " vertices...") #end
      #ifdef (deform_mesh) deform_mesh () #else #local Counter = 0; #while (Counter < PointCount) #read (File, X, Y, Z) #local PointList[Counter] = <X, Y, Z>; #local Counter = Counter+1; #end #end
      #if (debug_progress) #debug "done!\r" #end #end

// READ TRIANGLE/PATCH LIST AND CREATE SURFACE
// *******************************************
   #switch (VerID)
   #case (1) #case (3) #read (File, TriangleCount, SmoothTriangleCount)
      #if (debug_progress) #debug concat("   Creating mesh with ", str(TriangleCount, 0, 0), " triangles and ", str(SmoothTriangleCount, 0, 0), " smooth triangles...") #end
      #break
   #case (2) #read (File, PatchCount)
      #if (debug_progress) #debug concat("   Creating mesh with ", str(PatchCount, 0, 0), " bicubic patches...") #end
   #end

   #if (declare_only) #declare Mesh[MeshCount] = #end
   #ifdef (create_mesh) object {create_mesh () #else PCM_create_surface () #end

// APPLY MESH TEXTURES
// *******************
   #if (apply_mesh_textures)
      #ifdef (mesh_textures) texture {mesh_textures[mod(MeshCount, dimension_size(mesh_textures, 1))]} #else
      #ifdef (mesh_texture) texture {mesh_texture} #end #end #end
      }
   #local MeshCount = MeshCount+1; #if (debug_progress) #debug "done!\n" #end
   #end #end #end

   #if (declare_only & MeshCount > 0)
      #debug "Created Mesh[0] " #if (MeshCount > 1) #debug concat("to Mesh[", str(MeshCount-1, 0, 0), "] ") #end #debug concat("from file \"", FileName, "\"\n")
   #else
      #if (debug_progress) #debug concat("Created ", str(MeshCount, 0, 0), " mesh(es) from file \"", FileName, "\"\n") #end
   #end
#end

// DEFAULT SURFACE CREATION MACRO
// ******************************
#macro PCM_create_surface ()
   #switch (VerID)
   #case (1) mesh {
      #local Counter = 0; #while (Counter < TriangleCount)
         #read (File, P1, P2, P3) triangle {PointList[P1], PointList[P2], PointList[P3]}
      #local Counter = Counter+1; #end
      #local Counter = 0; #while (Counter < SmoothTriangleCount)
         #read (File, P1, N1, P2, N2, P3, N3) smooth_triangle {PointList[P1], PointList[N1], PointList[P2], PointList[N2], PointList[P3], PointList[N3]}
      #local Counter = Counter+1; #end
      #break

   #case (2) union {
      #local Counter = 0; #while (Counter < PatchCount)
         PCM_read_patch_options () bicubic_patch {type PT flatness PF u_steps PU v_steps PV
         #local Counter2 = 0; #while (Counter2 < 16)
            #read (File, P1) PointList[P1] #if (Counter2 < 15) , #end
         #local Counter2 = Counter2+1; #end }
      #local Counter = Counter+1; #end
      #break

   #case (3) mesh {
      #local Counter = 0; #while (Counter < TriangleCount)
         #if (Counter = 0) #read (File, P1, P2, P3) #declare Y = 0;
            #else #read (File, X) #if (X = 0) #read (File, P1, P2, P3) #declare Y = 0;
            #else #if (Y = 0) #declare Y = 1; #declare P1 = P3; #else #declare Y = 0; #declare P2 = P3; #end #declare P3 = X; #end #end
          triangle {PointList[P1-1], PointList[P2-1], PointList[P3-1]}
      #local Counter = Counter+1; #end
      #local Counter = 0; #while (Counter < SmoothTriangleCount)
         #if (Counter = 0) #read (File, P1, P2, P3, N1, N2, N3) #declare Y = 0;
            #else #read (File, X) #if (X = 0) #read (File, P1, P2, P3, N1, N2, N3) #declare Y = 0;
            #else #if (Y = 0) #declare Y = 1; #declare P1 = P3; #declare N1 = N3; #else #declare Y = 0; #declare P2 = P3; #declare N2 = N3; #end #declare P3 = X; #read (File, N3) #end #end
          smooth_triangle {PointList[P1-1], PointList[N1-1], PointList[P2-1], PointList[N2-1], PointList[P3-1], PointList[N3-1]}
      #local Counter = Counter+1; #end
   #end
#end

// BICUBIC PATCH INTERPOLATION MACRO: for use with create_patch () macro options; call with array of 16 points, returns surface point at U, V
// **********************************
#macro PCM_get_patch_point (PList, U, V)
   #local CPoint = <0, 0, 0>; #local C = array[4] {1, 3, 3, 1}
   #local IU = 1-U; #local cu = array[4] {1, U, U*U, U*U*U} #local cuu = array[4] {1, IU, IU*IU, IU*IU*IU}
   #local IV = 1-V; #local cv = array[4] {1, V, V*V, V*V*V} #local cvv = array[4] {1, IV, IV*IV, IV*IV*IV}
   #local I = 0; #while (I < 4) #local J = 0; #while (J < 4)
      #local CPoint = CPoint + (C[I]*C[J] * cu[I]*cuu[3-I] * cv[J]*cvv[3-J] * PList[I + J*4]);
   #local J = J + 1; #end #local I = I + 1; #end
   CPoint
#end

#macro PCM_read_patch_options ()
   #read (File, P1) #if (P1 != -1) #declare PT = P1; #read (File, PF, PU, PV) #end
   PCM_override_patch_options ()
#end

#macro PCM_override_patch_options ()
   #ifdef (pcm_patch_type) #declare PT = pcm_patch_type; #end
   #ifdef (pcm_patch_flatness) #declare PF = pcm_patch_flatness; #end
   #ifdef (pcm_patch_u_steps) #declare PU = pcm_patch_u_steps; #end
   #ifdef (pcm_patch_v_steps) #declare PV = pcm_patch_v_steps; #end
#end

// READ COMPRESSED BICUBIC PATCH DETAILS FROM FILE AND CONVERT TO GRID OF SURFACE POINTS PCM_patch_grid[U][V]
// **********************************************************************************************************
#macro PCM_get_patch_grid (MakePatch)
   #ifdef (PCM_patch_grid) #undef PCM_patch_grid #end
   #local PList = array[16] PCM_read_patch_options ()
   #if (MakePatch != false) bicubic_patch {type PT flatness PF u_steps PU v_steps PV #end
   #local Counter2 = 0; #while (Counter2 < 16)
      #read (File, P1) #local PList[Counter2] = PointList[P1];
      #if (MakePatch != false) PointList[P1] #if (Counter2 < 15) , #else } #end #end
   #local Counter2 = Counter2+1; #end
   #declare PCM_patch_grid = array[PU+1][PV+1]
   #local U = 0; #while (U <= PU) #local V = 0; #while (V <= PV)
      #declare PCM_patch_grid[U][V] = PCM_get_patch_point (PList, U/PU, V/PV);
   #local V = V+1; #end #local U = U+1; #end
#end
 
// MESH DEFORMATION MACROS
// ***********************
#macro wave_mesh (WaveScale, Frequency, Phase)
   #local Phase = Phase*2*pi;
   #local Counter = 0; #while (Counter < PointCount)
      #read (File, X, Y, Z)
      #declare PointList[Counter] = <X, 0, Z>*(1+sin(Y*Frequency+Phase)*WaveScale)+<0, Y, 0>;
   #local Counter = Counter+1; #end
#end

#macro ripple_mesh (RippleAxis, RippleAmount, Frequency, Phase)
   #local Phase = Phase*2*pi;
   #local RippleScale = (<1, 1, 1>-vnormalize(RippleAxis))*RippleAmount;
   #local Counter = 0; #while (Counter < PointCount)
      #read (File, X, Y, Z)
      #declare PointList[Counter] = <X, Y, Z>+(sin(vlength(RippleAxis*<X, Y, Z>)*Frequency+Phase)*RippleScale);
   #local Counter = Counter+1; #end
#end

#macro jitter_mesh (JitterScale, JitterSeed)
   #local R1 = seed(JitterSeed);
   #local Counter = 0; #while (Counter < PointCount)
      #read (File, X, Y, Z)
      #declare PointList[Counter] = <X, Y, Z>+(<rand(R1), rand(R1), rand(R1)>-.5)*JitterScale;
   #local Counter = Counter+1; #end
#end

#macro twist_mesh (TwistAngle, TwistCentre)
   #local Counter = 0; #while (Counter < PointCount)
      #read (File, X, Y, Z)
      #declare PointList[Counter] = <X, Y, Z>-TwistCentre;
      #declare PointList[Counter] = vrotate(PointList[Counter], TwistAngle*PointList[Counter])+TwistCentre;
   #local Counter = Counter+1; #end
#end

#macro bend_mesh (BendPoint1, BendPoint2, BendDirection, BendAngle)
   #local BA = BendAngle/vlength(BendPoint2-BendPoint1);
   #local BX = vcross(vnormalize(BendPoint2-BendPoint1), vnormalize(BendDirection));
   #local Counter = 0; #while (Counter < PointCount)
      #read (File, X, Y, Z)
      #declare PointList[Counter] = <X, Y, Z>-BendPoint1;
      #declare PointList[Counter] = vaxis_rotate(PointList[Counter], BX, vlength(PointList[Counter]*(1-BX))*BA)+BendPoint1;
   #local Counter = Counter+1; #end
#end

// MESH CREATION MACROS
// ********************
#macro object_mesh (SingleObject)
   union { #local Counter = 0; #switch (VerID)
   #case (1) #while (Counter < (TriangleCount+SmoothTriangleCount))
      #if (Counter < TriangleCount) #read (File, P1, P2, P3) #else #read (File, P1, N1, P2, N2, P3, N3) #end
      object {SingleObject translate (PointList[P1]+PointList[P2]+PointList[P3])/3}
   #local Counter = Counter+1; #end #break

   #case (2) #local PList = array[16] #while (Counter < PatchCount)
      PCM_read_patch_options ()
      #declare Counter2 = 0; #while (Counter2 < 16) #read (File, P1) #local PList[Counter2] = PointList[P1]; #declare Counter2 = Counter2+1; #end
      #local U = .5; #while (U < PU) #local V = .5; #while (V < PV) object {SingleObject translate PCM_get_patch_point (PList, U/PU, V/PV)} #local V = V+1; #end #local U = U+1; #end
   #local Counter = Counter+1; #end #break

   #case (3) #while (Counter < (TriangleCount+SmoothTriangleCount))
      #if (Counter = 0 | Counter = TriangleCount) #read (File, P1, P2, P3) #declare Y = 0; #if (Counter = TriangleCount) #read (File, N1, N2, N3) #end
         #else #read (File, X) #if (X = 0) #read (File, P1, P2, P3) #declare Y = 0;        #if (Counter > TriangleCount) #read (File, N1, N2, N3) #end
         #else #if (Y = 0) #declare Y = 1; #declare P1 = P3; #else #declare Y = 0; #declare P2 = P3; #end #declare P3 = X;  #if (Counter > TriangleCount) #read (File, N3) #end #end #end
      object {SingleObject translate (PointList[P1-1]+PointList[P2-1]+PointList[P3-1])/3}
   #local Counter = Counter+1; #end
   #end }
#end

#macro wire_mesh (Thickness)
   union { #local Counter = 0; #switch (VerID)
   #case (1) #while (Counter < (TriangleCount+SmoothTriangleCount))
      #if (Counter < TriangleCount) #read (File, P1, P2, P3) #else #read (File, P1, N1, P2, N2, P3, N3) #end
      #local Centre = (PointList[P1]+PointList[P2]+PointList[P3])/3;
      cylinder {Centre, PointList[P1], Thickness} cylinder {Centre, PointList[P2], Thickness} cylinder {Centre, PointList[P3], Thickness}
   #local Counter = Counter+1; #end #break

   #case (2) #while (Counter < PatchCount) PCM_get_patch_grid (false)
      #local U = 0; #while (U < PU) #local V = 0; #while (V < PV)
         #local Centre = (PCM_patch_grid[U][V] + PCM_patch_grid[U+1][V] + PCM_patch_grid[U][V+1])/3;
         cylinder {Centre, PCM_patch_grid[U][V], Thickness} cylinder {Centre, PCM_patch_grid[U+1][V], Thickness} cylinder {Centre, PCM_patch_grid[U][V+1], Thickness}
         #local Centre = (PCM_patch_grid[U+1][V+1] + PCM_patch_grid[U+1][V] + PCM_patch_grid[U][V+1])/3;
         cylinder {Centre, PCM_patch_grid[U+1][V+1], Thickness} cylinder {Centre, PCM_patch_grid[U+1][V], Thickness} cylinder {Centre, PCM_patch_grid[U][V+1], Thickness}
      #local V = V+1; #end #local U = U+1; #end
   #local Counter = Counter+1; #end #break

   #case (3) #while (Counter < (TriangleCount+SmoothTriangleCount))
      #if (Counter = 0 | Counter = TriangleCount) #read (File, P1, P2, P3) #declare Y = 0; #if (Counter = TriangleCount) #read (File, N1, N2, N3) #end
         #else #read (File, X) #if (X = 0) #read (File, P1, P2, P3) #declare Y = 0;        #if (Counter > TriangleCount) #read (File, N1, N2, N3) #end
         #else #if (Y = 0) #declare Y = 1; #declare P1 = P3; #else #declare Y = 0; #declare P2 = P3; #end #declare P3 = X;  #if (Counter > TriangleCount) #read (File, N3) #end #end #end
      #local Centre = (PointList[P1-1]+PointList[P2-1]+PointList[P3-1])/3;
      cylinder {Centre, PointList[P1-1], Thickness} cylinder {Centre, PointList[P2-1], Thickness} cylinder {Centre, PointList[P3-1], Thickness}
   #local Counter = Counter+1; #end
   #end }
#end

#macro blob_mesh (BlobSize, Threshold, Sturm)
   blob {threshold Threshold #local Counter = 0; #switch (VerID)
   #case (1) #while (Counter < (TriangleCount+SmoothTriangleCount))
      #if (Counter < TriangleCount) #read (File, P1, P2, P3) #else #read (File, P1, N1, P2, N2, P3, N3) #end
      #local Centre = (PointList[P1]+PointList[P2]+PointList[P3])/3;
      #local Size = vlength(PointList[P1]-Centre)+vlength(PointList[P2]-Centre)+vlength(PointList[P3]-Centre);
      sphere {Centre, BlobSize*Size/2, 1}
   #local Counter = Counter+1; #end #break

   #case (2) #while (Counter < PatchCount) PCM_get_patch_grid (false)
      #local U = 0; #while (U < PU) #local V = 0; #while (V < PV)
         #local Centre = (PCM_patch_grid[U][V] + PCM_patch_grid[U+1][V] + PCM_patch_grid[U][V+1] + PCM_patch_grid[U+1][V+1])/4;
         #local Size = vlength(PCM_patch_grid[U+1][V+1] - PCM_patch_grid[U][V]) + vlength(PCM_patch_grid[U+1][V] - PCM_patch_grid[U][V+1]);
         sphere {Centre, BlobSize*Size/2, 1}
      #local V = V+1; #end #local U = U+1; #end
   #local Counter = Counter+1; #end #break

   #case (3) #while (Counter < (TriangleCount+SmoothTriangleCount))
      #if (Counter = 0 | Counter = TriangleCount) #read (File, P1, P2, P3) #declare Y = 0; #if (Counter = TriangleCount) #read (File, N1, N2, N3) #end
         #else #read (File, X) #if (X = 0) #read (File, P1, P2, P3) #declare Y = 0;        #if (Counter > TriangleCount) #read (File, N1, N2, N3) #end
         #else #if (Y = 0) #declare Y = 1; #declare P1 = P3; #else #declare Y = 0; #declare P2 = P3; #end #declare P3 = X;  #if (Counter > TriangleCount) #read (File, N3) #end #end #end
      #local Centre = (PointList[P1-1]+PointList[P2-1]+PointList[P3-1])/3;
      #local Size = vlength(PointList[P1-1]-Centre)+vlength(PointList[P2-1]-Centre)+vlength(PointList[P3-1]-Centre);
      sphere {Centre, BlobSize*Size/2, 1}
   #local Counter = Counter+1; #end
   #end sturm Sturm}
#end

#macro spike_mesh (Length, Thickness)
   union { #local Counter = 0; #switch (VerID)
   #case (1) #while (Counter < (TriangleCount+SmoothTriangleCount))
      #if (Counter < TriangleCount) #read (File, P1, P2, P3) triangle {PointList[P1], PointList[P2], PointList[P3]}
      #else #read (File, P1, N1, P2, N2, P3, N3) smooth_triangle {PointList[P1], PointList[N1], PointList[P2], PointList[N2], PointList[P3], PointList[N3]} #end
      #local Centre = (PointList[P1]+PointList[P2]+PointList[P3])/3;
      #local Size = vlength(PointList[P1]-Centre)+vlength(PointList[P2]-Centre)+vlength(PointList[P3]-Centre);
      #local Direction = vcross(PointList[P2]-PointList[P1], PointList[P3]-PointList[P2]);
      cone {Centre, Size*Thickness/6, Centre+Direction*Length, 0}
   #local Counter = Counter+1; #end #break

   #case (2) #while (Counter < PatchCount) PCM_get_patch_grid (true)
      #local U = 0; #while (U < PU) #local V = 0; #while (V < PV)
         #local Centre = (PCM_patch_grid[U][V] + PCM_patch_grid[U+1][V] + PCM_patch_grid[U][V+1] + PCM_patch_grid[U+1][V+1])/4;
         #local Size = vlength(PCM_patch_grid[U+1][V+1] - PCM_patch_grid[U][V]) + vlength(PCM_patch_grid[U+1][V] - PCM_patch_grid[U][V+1]);
         #local Direction = vcross(PCM_patch_grid[U+1][V+1] - PCM_patch_grid[U][V], PCM_patch_grid[U][V+1] - PCM_patch_grid[U+1][V]);
         cone {Centre, Size*Thickness/6, Centre+Direction*Length, 0}
      #local V = V+1; #end #local U = U+1; #end
   #local Counter = Counter+1; #end #break

   #case (3) #while (Counter < (TriangleCount+SmoothTriangleCount))
      #if (Counter = 0 | Counter = TriangleCount) #read (File, P1, P2, P3) #declare Y = 0; #if (Counter = TriangleCount) #read (File, N1, N2, N3) #end
         #else #read (File, X) #if (X = 0) #read (File, P1, P2, P3) #declare Y = 0;        #if (Counter > TriangleCount) #read (File, N1, N2, N3) #end
         #else #if (Y = 0) #declare Y = 1; #declare P1 = P3; #else #declare Y = 0; #declare P2 = P3; #end #declare P3 = X;  #if (Counter > TriangleCount) #read (File, N3) #end #end #end
      #local Centre = (PointList[P1-1]+PointList[P2-1]+PointList[P3-1])/3;
      #local Size = vlength(PointList[P1-1]-Centre)+vlength(PointList[P2-1]-Centre)+vlength(PointList[P3-1]-Centre);
      #local Direction = vcross(PointList[P2-1]-PointList[P1-1], PointList[P3-1]-PointList[P2-1]);
      cone {Centre, Size*Thickness/6, Centre+Direction*Length, 0}
   #local Counter = Counter+1; #end
   #end }
#end

#macro cylinder_mesh (Thickness, Edge1, Edge2, Edge3)
   union { #local Counter = 0; #switch (VerID)
   #case (1) #while (Counter < (TriangleCount+SmoothTriangleCount))
      #if (Counter < TriangleCount) #read (File, P1, P2, P3) #else #read (File, P1, N1, P2, N2, P3, N3) #end
      #if (Edge1) cylinder {PointList[P1], PointList[P2] Thickness} #end
      #if (Edge2) cylinder {PointList[P2], PointList[P3], Thickness} #end
      #if (Edge3) cylinder {PointList[P3], PointList[P1], Thickness} #end
   #local Counter = Counter+1; #end #break

   #case (2) #while (Counter < PatchCount) PCM_get_patch_grid (false)
      #local U = 0; #while (U < PU) #local V = 0; #while (V < PV)
         #if (Edge1) cylinder {PCM_patch_grid[U][V], PCM_patch_grid[U+1][V] Thickness} cylinder {PCM_patch_grid[U][V+1], PCM_patch_grid[U+1][V+1] Thickness} #end
         #if (Edge2) cylinder {PCM_patch_grid[U][V], PCM_patch_grid[U][V+1], Thickness} cylinder {PCM_patch_grid[U+1][V], PCM_patch_grid[U+1][V+1], Thickness} #end
         #if (Edge3) cylinder {PCM_patch_grid[U][V], PCM_patch_grid[U+1][V+1], Thickness} #end
      #local V = V+1; #end #local U = U+1; #end
   #local Counter = Counter+1; #end #break

   #case (3) #while (Counter < (TriangleCount+SmoothTriangleCount))
      #if (Counter = 0 | Counter = TriangleCount) #read (File, P1, P2, P3) #declare Y = 0; #if (Counter = TriangleCount) #read (File, N1, N2, N3) #end
         #else #read (File, X) #if (X = 0) #read (File, P1, P2, P3) #declare Y = 0;        #if (Counter > TriangleCount) #read (File, N1, N2, N3) #end
         #else #if (Y = 0) #declare Y = 1; #declare P1 = P3; #else #declare Y = 0; #declare P2 = P3; #end #declare P3 = X;  #if (Counter > TriangleCount) #read (File, N3) #end #end #end
      #if (Edge1) cylinder {PointList[P1-1], PointList[P2-1] Thickness} #end
      #if (Edge2) cylinder {PointList[P2-1], PointList[P3-1], Thickness} #end
      #if (Edge3) cylinder {PointList[P3-1], PointList[P1-1], Thickness} #end
   #local Counter = Counter+1; #end
   #end }
#end
