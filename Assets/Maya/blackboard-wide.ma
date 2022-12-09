//Maya ASCII 2015 scene
//Name: blackboard-wide.ma
//Last modified: Sat, Aug 19, 2017 12:44:46 PM
//Codeset: 1252
requires maya "2015";
requires -nodeType "mentalrayFramebuffer" -nodeType "mentalrayOptions" -nodeType "mentalrayGlobals"
		 -nodeType "mentalrayItemsList" -dataType "byteArray" "Mayatomr" "2015.0 - 3.12.1.16 ";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2015";
fileInfo "version" "2015";
fileInfo "cutIdentifier" "201405190330-916664";
fileInfo "osv" "Microsoft Windows 7 Business Edition, 64-bit Windows 7 Service Pack 1 (Build 7601)\n";
fileInfo "license" "student";
createNode transform -s -n "persp";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 86.233544816335652 110.13438967999916 333.45965864362614 ;
	setAttr ".r" -type "double3" -11.138352728730517 359.39999999990647 -2.4849445959822795e-017 ;
createNode camera -s -n "perspShape" -p "persp";
	setAttr -k off ".v";
	setAttr ".fl" 34.999999999999993;
	setAttr ".coi" 295.56092645489315;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".tp" -type "double3" 1.0951738444060197 17.693531036376953 -0.11625327969943555 ;
	setAttr ".hc" -type "string" "viewSet -p %camera";
createNode transform -s -n "top";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -119.91534298835521 555.90862209089516 121.42498223214055 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	setAttr -k off ".v";
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 620.03805861510102;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
createNode transform -s -n "front";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 1.0111219334298909 63.669517517089844 683.1837766825638 ;
createNode camera -s -n "frontShape" -p "front";
	setAttr -k off ".v";
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 120.69176037409875;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
createNode transform -s -n "side";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 821.85408772179017 15.063214309579223 -2.0116434277358337 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	setAttr -k off ".v";
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 390.81474241412815;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
createNode transform -n "walls";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 135.84411577917447 89.027563150529147 138.02879205537695 ;
createNode mesh -n "wallsShape" -p "walls";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.32977116107940674 0.57656323909759521 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "skirting_and_door_frame";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -1.7153749876898132 4.1760727693097337 147.90504428622802 ;
createNode mesh -n "skirting_and_door_frameShape" -p "skirting_and_door_frame";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.5 0.5 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".ndt" 2;
	setAttr ".dn" yes;
createNode transform -n "pCube3";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0.76396559587260526 45.376168307992828 58.020323832319406 ;
	setAttr ".s" -type "double3" 1 1.0000000000000004 1 ;
createNode transform -n "transform2" -p "pCube3";
createNode mesh -n "pCubeShape3" -p "transform2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.5 0.5 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "pCube4";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0.76396559587260526 69.420071768319673 58.020323832319406 ;
	setAttr ".s" -type "double3" 1 1.0000000000000004 1 ;
createNode mesh -n "polySurfaceShape1" -p "pCube4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.5 0.375 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.375 0 0.625 0 0.375
		 0.25 0.625 0.25 0.375 0.5 0.625 0.5 0.375 0.75 0.625 0.75 0.375 1 0.625 1 0.875 0
		 0.875 0.25 0.125 0 0.125 0.25;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".pt[0:7]" -type "float3"  1.966804 5.4836273e-006 -11.61937 
		-6.2098465 5.4836273e-006 -11.61937 1.966804 -19.304605 -11.61937 -6.2098465 -19.304605 
		-11.61937 1.966804 -19.304605 10.859759 -6.2098465 -19.304605 10.859759 1.966804 
		5.4836273e-006 10.859759 -6.2098465 5.4836273e-006 10.859759;
	setAttr -s 8 ".vt[0:7]"  -7.21816969 -12.21536446 39.97755432 7.21816969 -12.21536446 39.97755432
		 -7.21816969 12.21536446 39.97755432 7.21816969 12.21536446 39.97755432 -7.21816969 12.21536446 -39.97755432
		 7.21816969 12.21536446 -39.97755432 -7.21816969 -12.21536446 -39.97755432 7.21816969 -12.21536446 -39.97755432;
	setAttr -s 12 ".ed[0:11]"  0 1 0 2 3 0 4 5 0 6 7 0 0 2 0 1 3 0 2 4 0
		 3 5 0 4 6 0 5 7 0 6 0 0 7 1 0;
	setAttr -s 6 -ch 24 ".fc[0:5]" -type "polyFaces" 
		f 4 0 5 -2 -5
		mu 0 4 0 1 3 2
		f 4 1 7 -3 -7
		mu 0 4 2 3 5 4
		f 4 2 9 -4 -9
		mu 0 4 4 5 7 6
		f 4 3 11 -1 -11
		mu 0 4 6 7 9 8
		f 4 -12 -10 -8 -6
		mu 0 4 1 10 11 3
		f 4 10 4 6 8
		mu 0 4 12 0 2 13;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "transform1" -p "pCube4";
createNode mesh -n "pCubeShape4" -p "transform1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.5 0.5 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "window_frame";
	setAttr ".v" no;
createNode mesh -n "window_frameShape" -p "window_frame";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "window_frame1";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 431.64305679431499 0 0 ;
createNode transform -n "polySurface1" -p "window_frame1";
	setAttr ".t" -type "double3" 1.8465552077534539 -2.1273015654206375 -115.19057078437376 ;
	setAttr ".rp" -type "double3" -1.5676768063777899 37.85066362836573 55.179079296443199 ;
	setAttr ".sp" -type "double3" -1.5676768063777899 37.85066362836573 55.179079296443199 ;
createNode transform -n "transform6" -p "|window_frame1|polySurface1";
createNode mesh -n "polySurfaceShape2" -p "transform6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.74946165084838867 0.54366147518157959 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 7 ".pt";
	setAttr ".pt[0]" -type "float3" 0 2.8948133 -4.3666058 ;
	setAttr ".pt[1]" -type "float3" -2.0319221 2.8948133 -4.3666058 ;
	setAttr ".pt[2]" -type "float3" 0 0 -4.3666058 ;
	setAttr ".pt[3]" -type "float3" -2.0319221 0 -4.3666058 ;
	setAttr ".pt[5]" -type "float3" -2.0319221 0 0 ;
	setAttr ".pt[6]" -type "float3" 0 2.8948133 0 ;
	setAttr ".pt[7]" -type "float3" -2.0319221 2.8948133 0 ;
createNode transform -n "polySurface2" -p "window_frame1";
	setAttr ".t" -type "double3" 0 0 -131.01785264469927 ;
	setAttr ".s" -type "double3" 1 1 1.2486904136388408 ;
createNode transform -n "transform5" -p "polySurface2";
createNode mesh -n "polySurfaceShape3" -p "transform5";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" -0.5270845890045166 0.5 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 16 ".pt";
	setAttr ".pt[2]" -type "float3" 0 -1.8194191 0 ;
	setAttr ".pt[3]" -type "float3" 0 -1.8194191 0 ;
	setAttr ".pt[4]" -type "float3" 0 -1.8194191 0 ;
	setAttr ".pt[5]" -type "float3" 0 -1.8194191 0 ;
	setAttr ".pt[8]" -type "float3" 0 -1.8194191 0 ;
	setAttr ".pt[9]" -type "float3" 0 -1.8194191 0 ;
	setAttr ".pt[10]" -type "float3" 0 0 2.1572673 ;
	setAttr ".pt[11]" -type "float3" 0 0 2.1572673 ;
	setAttr ".pt[12]" -type "float3" 0 -1.8194191 0 ;
	setAttr ".pt[13]" -type "float3" 0 -1.8194191 0 ;
	setAttr ".pt[14]" -type "float3" 0 0 -3.5055594 ;
	setAttr ".pt[15]" -type "float3" 0 0 -3.5055594 ;
	setAttr ".pt[16]" -type "float3" 0 0 2.1572673 ;
	setAttr ".pt[17]" -type "float3" 0 0 2.1572673 ;
	setAttr ".pt[22]" -type "float3" 0 0 -3.5055594 ;
	setAttr ".pt[23]" -type "float3" 0 0 -3.5055594 ;
createNode transform -n "transform3" -p "window_frame1";
createNode mesh -n "window_frame1Shape" -p "transform3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".iog[0].og[1].gcl" -type "componentList" 1 "f[0:55]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.75766059756278992 0.17511451244354248 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 52 ".uvst[0].uvsp[0:51]" -type "float2" 0.40806496 -1.086107612
		 0.65752661 -1.03424716 0.39166707 -0.29715323 0.64112872 -0.2452929 0.37526917 0.49180111
		 0.62473083 0.54366148 0.35887128 1.2807554 0.60833293 1.33261585 0.34247339 2.069709778
		 0.59193504 2.12157011 0.90698826 -0.98238683 0.89059031 -0.19343251 0.15860334 -1.13796794
		 0.14220545 -0.34901363 -0.65208459 0 -0.40208459 0 -0.65208459 0.25 -0.40208459 0.25
		 -0.65208459 0.27287853 -0.40208465 0.27287853 -0.65208459 0.5 -0.40208459 0.5 -0.65208459
		 0.75 -0.40208459 0.75 -0.65208459 0.97712147 -0.40208459 0.97712147 -0.65208459 1
		 -0.40208459 1 -0.37920606 0 -0.37920606 0.25 -0.67496312 0 -0.67496312 0.25 -0.65208459
		 0.47012731 -0.40208459 0.47012731 -0.87221193 0 -0.87221193 0.25 -0.65208459 0.77987272
		 -0.40208459 0.77987272 -0.65208459 0.97712147 -0.40208459 0.97712147 -0.18195724
		 0.25 -0.18195724 0 -0.90208459 0 -0.90208459 0.25 -0.65208459 0.75 -0.40208459 0.75
		 -0.65208459 0.77987272 -0.40208459 0.77987272 -0.15208459 0.25 -0.15208459 0 -0.40208459
		 1 -0.65208459 1;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".vt[0:31]"  -6.45420408 33.16080856 97.99787903 3.7390933 33.16080856 97.99787903
		 -6.45420408 41.32653809 97.99787903 3.7390933 41.32653809 97.99787903 -6.45420408 41.32653809 18.042770386
		 3.7390933 41.32653809 18.042770386 -6.45420408 33.16080856 18.042770386 3.7390933 33.16080856 18.042770386
		 -4.48728561 90.10401917 86.37850952 1.77241898 90.10401917 86.37850952 -4.48728561 95.23013306 86.37850952
		 1.77241898 95.23013306 86.37850952 -4.48728561 95.23013306 28.90253067 1.77241898 95.23013306 28.90253067
		 -4.48728561 90.10401917 28.90253067 1.77241898 90.10401917 28.90253067 1.77241898 95.23013306 81.11863708
		 -4.48728561 95.23013306 81.11863708 -4.48728561 90.10401917 81.11863708 1.77241898 90.10401917 81.11863708
		 1.77241898 95.23013306 35.7703743 -4.48728561 95.23013306 35.7703743 -4.48728561 90.10401917 35.7703743
		 1.77241898 90.10401917 35.7703743 -4.48728561 39.26678467 81.11863708 1.77241898 39.26678467 81.11863708
		 1.77241898 39.26678467 86.37850952 -4.48728561 39.26678467 86.37850952 -4.48728561 39.038330078 28.90253067
		 1.77241898 39.038330078 28.90253067 1.77241898 39.038330078 35.7703743 -4.48728561 39.038330078 35.7703743;
	setAttr -s 84 ".ed[0:83]"  0 1 0 2 3 0 4 5 0 6 7 0 0 2 0 1 3 0 2 4 0
		 3 5 0 4 6 0 5 7 0 6 0 0 7 1 0 1 2 1 3 4 1 5 6 1 7 0 1 7 3 1 0 4 1 8 9 0 10 11 0 12 13 0
		 14 15 0 8 10 0 9 11 0 10 17 0 11 16 0 12 14 0 13 15 0 14 22 0 15 23 0 16 20 0 17 21 0
		 16 17 1 18 8 0 17 18 1 19 9 0 18 19 0 19 16 1 20 13 0 21 12 0 20 21 1 22 18 0 21 22 1
		 23 19 0 22 23 0 23 20 1 18 24 0 19 25 0 24 25 0 9 26 0 25 26 0 8 27 0 27 26 0 24 27 0
		 14 28 0 15 29 0 28 29 0 23 30 0 29 30 0 22 31 0 31 30 0 28 31 0 9 10 1 11 17 1 13 14 1
		 25 27 1 19 11 1 8 17 1 16 21 1 18 21 1 23 18 1 19 20 1 20 12 1 22 12 1 29 31 1 23 13 1
		 19 24 1 9 25 1 8 26 1 18 27 1 15 28 1 23 29 1 22 30 1 14 31 1;
	setAttr -s 56 -ch 168 ".fc[0:55]" -type "polyFaces" 
		f 3 0 12 -5
		mu 0 3 0 1 2
		f 3 -13 5 -2
		mu 0 3 2 1 3
		f 3 1 13 -7
		mu 0 3 2 3 4
		f 3 -14 7 -3
		mu 0 3 4 3 5
		f 3 2 14 -9
		mu 0 3 4 5 6
		f 3 -15 9 -4
		mu 0 3 6 5 7
		f 3 3 15 -11
		mu 0 3 6 7 8
		f 3 -16 11 -1
		mu 0 3 8 7 9
		f 3 -12 16 -6
		mu 0 3 1 10 3
		f 3 -17 -10 -8
		mu 0 3 3 10 11
		f 3 10 17 8
		mu 0 3 12 0 13
		f 3 -18 4 6
		mu 0 3 13 0 2
		f 3 18 62 -23
		mu 0 3 14 15 16
		f 3 -63 23 -20
		mu 0 3 16 15 17
		f 3 19 63 -25
		mu 0 3 16 17 18
		f 3 -64 25 32
		mu 0 3 18 17 19
		f 3 20 64 -27
		mu 0 3 20 21 22
		f 3 -65 27 -22
		mu 0 3 22 21 23
		f 3 48 65 -54
		mu 0 3 24 25 26
		f 3 -66 50 -53
		mu 0 3 26 25 27
		f 3 -36 66 -24
		mu 0 3 15 28 17
		f 3 -67 37 -26
		mu 0 3 17 28 29
		f 3 33 67 34
		mu 0 3 30 14 31
		f 3 -68 22 24
		mu 0 3 31 14 16
		f 3 -33 68 -32
		mu 0 3 18 19 32
		f 3 -69 30 40
		mu 0 3 32 19 33
		f 3 41 69 42
		mu 0 3 34 30 35
		f 3 -70 -35 31
		mu 0 3 35 30 31
		f 3 44 70 -42
		mu 0 3 36 37 38
		f 3 -71 43 -37
		mu 0 3 38 37 39
		f 3 -38 71 -31
		mu 0 3 29 28 40
		f 3 -72 -44 45
		mu 0 3 40 28 41
		f 3 -41 72 -40
		mu 0 3 32 33 20
		f 3 -73 38 -21
		mu 0 3 20 33 21
		f 3 28 73 26
		mu 0 3 42 34 43
		f 3 -74 -43 39
		mu 0 3 43 34 35
		f 3 56 74 -62
		mu 0 3 44 45 46
		f 3 -75 58 -61
		mu 0 3 46 45 47
		f 3 -46 75 -39
		mu 0 3 40 41 48
		f 3 -76 -30 -28
		mu 0 3 48 41 49
		f 3 36 76 -47
		mu 0 3 38 39 24
		f 3 -77 47 -49
		mu 0 3 24 39 25
		f 3 35 77 -48
		mu 0 3 39 50 25
		f 3 -78 49 -51
		mu 0 3 25 50 27
		f 3 -19 78 -50
		mu 0 3 50 51 27
		f 3 -79 51 52
		mu 0 3 27 51 26
		f 3 -34 79 -52
		mu 0 3 51 38 26
		f 3 -80 46 53
		mu 0 3 26 38 24
		f 3 21 80 -55
		mu 0 3 22 23 44
		f 3 -81 55 -57
		mu 0 3 44 23 45
		f 3 29 81 -56
		mu 0 3 23 37 45
		f 3 -82 57 -59
		mu 0 3 45 37 47
		f 3 -45 82 -58
		mu 0 3 37 36 47
		f 3 -83 59 60
		mu 0 3 47 36 46
		f 3 -29 83 -60
		mu 0 3 36 22 46
		f 3 -84 54 61
		mu 0 3 46 22 44;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "pCube5";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 403.33241877964525 64.568470637198658 -58.877627432537551 ;
createNode transform -n "transform4" -p "pCube5";
createNode mesh -n "pCubeShape5" -p "transform4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.9950711727142334 0.0013405978679656982 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "pCube6";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 143.30397121319339 -19.75132101569292 325.22431243124919 ;
createNode mesh -n "pCubeShape6" -p "pCube6";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.50391316413879395 0.50018835067749023 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "polySurface1";
	setAttr ".t" -type "double3" -429.53562304879944 0 66.873809576459792 ;
	setAttr ".r" -type "double3" 0 -90.014269383429465 0 ;
	setAttr ".rp" -type "double3" 431.59779258506575 65.022929991083686 -60.404713737626302 ;
	setAttr ".sp" -type "double3" 431.59779258506575 65.022929991083686 -60.404713737626302 ;
createNode mesh -n "polySurface1Shape" -p "|polySurface1";
	setAttr -k off ".v";
	setAttr -s 4 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" -0.27014642953872681 0.49180108308792114 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "pCube7";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -122.73679106243407 32.842935136945201 121.95922510387692 ;
createNode transform -n "transform12" -p "pCube7";
createNode mesh -n "pCubeShape7" -p "transform12";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.47895288467407227 0.60792738199234009 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "pCube8";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -122.73679106243407 7.8471795801870812 121.95922510387692 ;
	setAttr ".s" -type "double3" 0.81555620425524034 4.3612924960166328 0.79903990761831523 ;
createNode transform -n "transform11" -p "pCube8";
createNode mesh -n "pCubeShape8" -p "transform11";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".iog[0].og[1].gcl" -type "componentList" 1 "f[0:5]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.27780640125274658 0.35140544176101685 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.30851471 0.040656488
		 0.56942731 0.070957005 0.2778064 0.35140544 0.53871906 0.38170594 0.25573304 0.39476445
		 0.50573301 0.39476445 0.19726884 0.10221671 0.44726881 0.10221671 0.25573304 1.0070157051
		 0.50573301 1.0070157051 0.79602081 0.097271964 0.76531255 0.40802091 0.047602028
		 0.010355961 0.016893746 0.32110491;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".pt[0:7]" -type "float3"  -3.4703965 4.366312 0 3.0539479 
		4.366312 0 -2.7763171 4.366312 -1.2401721 2.3588841 4.366312 -0.65065539 -2.7763171 
		4.366312 1.2401723 1.6657898 4.366312 1.2401723 -3.4703965 4.366312 0 3.0539479 4.366312 
		0;
	setAttr -s 8 ".vt[0:7]"  -24.17559624 -1.15879476 18.77675056 24.17559624 -1.15879476 18.77675056
		 -24.17559624 1.15879476 18.77675056 24.17559624 1.15879476 18.77675056 -24.17559624 1.15879476 -18.77675056
		 24.17559624 1.15879476 -18.77675056 -24.17559624 -1.15879476 -18.77675056 24.17559624 -1.15879476 -18.77675056;
	setAttr -s 12 ".ed[0:11]"  0 1 0 2 3 0 4 5 0 6 7 0 0 2 0 1 3 0 2 4 0
		 3 5 0 4 6 0 5 7 0 6 0 0 7 1 0;
	setAttr -s 6 -ch 24 ".fc[0:5]" -type "polyFaces" 
		f 4 0 5 -2 -5
		mu 0 4 0 1 3 2
		f 4 1 7 -3 -7
		mu 0 4 2 3 5 4
		f 4 2 9 -4 -9
		mu 0 4 4 5 7 6
		f 4 3 11 -1 -11
		mu 0 4 6 7 9 8
		f 4 -12 -10 -8 -6
		mu 0 4 1 10 11 3
		f 4 10 4 6 8
		mu 0 4 12 0 2 13;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "pCylinder1";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -214.87060344334282 41.676197356733567 31.838748924432196 ;
	setAttr ".rp" -type "double3" 110.25223844624442 -29.326043151447109 102.05839551589557 ;
	setAttr ".sp" -type "double3" 110.25223844624442 -29.326043151447109 102.05839551589557 ;
createNode transform -n "transform10" -p "pCylinder1";
createNode mesh -n "pCylinderShape1" -p "transform10";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.4401041716337204 0.30203323066234589 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".pt[0:7]" -type "float3"  121.05875 1.3853323 116.28771 
		119.2571 1.3853323 86.825653 93.513283 1.3853323 102.91747 121.72271 -60.819489 114.70129 
		116.90592 -60.819489 87.865387 94.01387 -60.819489 103.92594 0 0 0 0 0 0;
createNode transform -n "pCylinder2";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -214.02905852745869 41.676197356733567 8.5067256565830718 ;
	setAttr ".r" -type "double3" 0 92.532652790026731 0 ;
	setAttr ".rp" -type "double3" 110.25223844624442 -29.326043151447109 102.05839551589557 ;
	setAttr ".sp" -type "double3" 110.25223844624442 -29.326043151447109 102.05839551589557 ;
createNode transform -n "transform9" -p "pCylinder2";
createNode mesh -n "pCylinderShape2" -p "transform9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".iog[0].og[1].gcl" -type "componentList" 1 "f[0:4]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.4401041716337204 0.30203323066234589 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.9553858 0.043512441
		 0.9553858 0.070198089 0.97849619 0.056855261 0.9507637 0.07226225 0.9589808 0.07226225
		 0.96719778 0.07226225 0.97541487 0.07226225 0.9507637 0.10933164 0.9589808 0.10933164
		 0.96719778 0.10933164 0.97541487 0.10933164 0.9553858 0.11130311 0.9553858 0.13798879
		 0.97849619 0.12464595;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".pt[0:7]" -type "float3"  121.05875 1.3853323 116.28771 
		119.2571 1.3853323 86.825653 93.513283 1.3853323 102.91747 121.72271 -60.819489 114.70129 
		116.90592 -60.819489 87.865387 94.01387 -60.819489 103.92594 0 0 0 0 0 0;
	setAttr -s 6 ".vt[0:5]"  -9.13056183 -41.67619705 -15.81460094 -9.13056374 -41.67619705 15.81459808
		 18.26112556 -41.67619705 0 -9.13056183 41.67619705 -15.81460094 -9.13056374 41.67619705 15.81459808
		 18.26112556 41.67619705 0;
	setAttr -s 9 ".ed[0:8]"  0 1 0 1 2 0 2 0 0 3 4 0 4 5 0 5 3 0 0 3 0
		 1 4 0 2 5 0;
	setAttr -s 5 -ch 18 ".fc[0:4]" -type "polyFaces" 
		f 4 0 7 -4 -7
		mu 0 4 3 4 8 7
		f 4 1 8 -5 -8
		mu 0 4 4 5 9 8
		f 4 2 6 -6 -9
		mu 0 4 5 6 10 9
		f 3 -1 -3 -2
		mu 0 3 1 0 2
		f 3 5 3 4
		mu 0 3 13 12 11;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "pCylinder3";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -252.22910332188749 41.676197356733567 8.1902859208535403 ;
	setAttr ".r" -type "double3" 0 181.96845630878249 0 ;
	setAttr ".rp" -type "double3" 110.25223844624442 -29.326043151447109 102.05839551589557 ;
	setAttr ".sp" -type "double3" 110.25223844624442 -29.326043151447109 102.05839551589557 ;
createNode transform -n "transform8" -p "pCylinder3";
createNode mesh -n "pCylinderShape3" -p "transform8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".iog[0].og[1].gcl" -type "componentList" 1 "f[0:4]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.88979583978652954 0.083734914660453796 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.87601352 0.025442965
		 0.87601352 0.058373086 0.91046929 0.041908026 0.86912239 0.060920246 0.88137335 0.060920246
		 0.89362425 0.060920246 0.90587521 0.060920246 0.86912239 0.10666393 0.88137335 0.10666393
		 0.89362425 0.10666393 0.90587521 0.10666393 0.87601352 0.10909672 0.87601352 0.14202684
		 0.91046929 0.12556177;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".pt[0:7]" -type "float3"  121.05875 1.3853323 116.28771 
		119.2571 1.3853323 86.825653 93.513283 1.3853323 102.91747 121.72271 -60.819489 114.70129 
		116.90592 -60.819489 87.865387 94.01387 -60.819489 103.92594 0 0 0 0 0 0;
	setAttr -s 6 ".vt[0:5]"  -9.13056183 -41.67619705 -15.81460094 -9.13056374 -41.67619705 15.81459808
		 18.26112556 -41.67619705 0 -9.13056183 41.67619705 -15.81460094 -9.13056374 41.67619705 15.81459808
		 18.26112556 41.67619705 0;
	setAttr -s 9 ".ed[0:8]"  0 1 0 1 2 0 2 0 0 3 4 0 4 5 0 5 3 0 0 3 0
		 1 4 0 2 5 0;
	setAttr -s 5 -ch 18 ".fc[0:4]" -type "polyFaces" 
		f 4 0 7 -4 -7
		mu 0 4 3 4 8 7
		f 4 1 8 -5 -8
		mu 0 4 4 5 9 8
		f 4 2 6 -6 -9
		mu 0 4 5 6 10 9
		f 3 -1 -3 -2
		mu 0 3 1 0 2
		f 3 5 3 4
		mu 0 3 13 12 11;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "pCylinder4";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -251.96159881835132 41.676197356733567 31.101321568577319 ;
	setAttr ".r" -type "double3" 0 273.51828896125727 0 ;
	setAttr ".rp" -type "double3" 110.25223844624442 -29.326043151447109 102.05839551589557 ;
	setAttr ".sp" -type "double3" 110.25223844624442 -29.326043151447109 102.05839551589557 ;
createNode transform -n "transform7" -p "pCylinder4";
createNode mesh -n "pCylinderShape4" -p "transform7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".iog[0].og[1].gcl" -type "componentList" 1 "f[0:4]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.9646299793086559 0.090750616781040461 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.9553858 0.043512441
		 0.9553858 0.070198089 0.97849619 0.056855261 0.9507637 0.07226225 0.9589808 0.07226225
		 0.96719778 0.07226225 0.97541487 0.07226225 0.9507637 0.10933164 0.9589808 0.10933164
		 0.96719778 0.10933164 0.97541487 0.10933164 0.9553858 0.11130311 0.9553858 0.13798879
		 0.97849619 0.12464595;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".pt[0:7]" -type "float3"  121.05875 1.3853323 116.28771 
		119.2571 1.3853323 86.825653 93.513283 1.3853323 102.91747 121.72271 -60.819489 114.70129 
		116.90592 -60.819489 87.865387 94.01387 -60.819489 103.92594 0 0 0 0 0 0;
	setAttr -s 6 ".vt[0:5]"  -9.13056183 -41.67619705 -15.81460094 -9.13056374 -41.67619705 15.81459808
		 18.26112556 -41.67619705 0 -9.13056183 41.67619705 -15.81460094 -9.13056374 41.67619705 15.81459808
		 18.26112556 41.67619705 0;
	setAttr -s 9 ".ed[0:8]"  0 1 0 1 2 0 2 0 0 3 4 0 4 5 0 5 3 0 0 3 0
		 1 4 0 2 5 0;
	setAttr -s 5 -ch 18 ".fc[0:4]" -type "polyFaces" 
		f 4 0 7 -4 -7
		mu 0 4 3 4 8 7
		f 4 1 8 -5 -8
		mu 0 4 4 5 9 8
		f 4 2 6 -6 -9
		mu 0 4 5 6 10 9
		f 3 -1 -3 -2
		mu 0 3 1 0 2
		f 3 5 3 4
		mu 0 3 13 12 11;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "pCube9";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 123.83196354777516 0 -122.07548179532444 ;
	setAttr ".rp" -type "double3" -122.16620235243735 -0.064758772270927523 122.764888894143 ;
	setAttr ".sp" -type "double3" -122.16620235243735 -0.064758772270927523 122.764888894143 ;
createNode mesh -n "pCube9Shape" -p "pCube9";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.47895288467407227 0.4912718590348959 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode mentalrayItemsList -s -n "mentalrayItemsList";
createNode mentalrayGlobals -s -n "mentalrayGlobals";
createNode mentalrayOptions -s -n "miDefaultOptions";
	addAttr -ci true -m -sn "stringOptions" -ln "stringOptions" -at "compound" -nc 
		3;
	addAttr -ci true -sn "name" -ln "name" -dt "string" -p "stringOptions";
	addAttr -ci true -sn "value" -ln "value" -dt "string" -p "stringOptions";
	addAttr -ci true -sn "type" -ln "type" -dt "string" -p "stringOptions";
	setAttr -s 48 ".stringOptions";
	setAttr ".stringOptions[0].name" -type "string" "rast motion factor";
	setAttr ".stringOptions[0].value" -type "string" "1.0";
	setAttr ".stringOptions[0].type" -type "string" "scalar";
	setAttr ".stringOptions[1].name" -type "string" "rast transparency depth";
	setAttr ".stringOptions[1].value" -type "string" "8";
	setAttr ".stringOptions[1].type" -type "string" "integer";
	setAttr ".stringOptions[2].name" -type "string" "rast useopacity";
	setAttr ".stringOptions[2].value" -type "string" "true";
	setAttr ".stringOptions[2].type" -type "string" "boolean";
	setAttr ".stringOptions[3].name" -type "string" "importon";
	setAttr ".stringOptions[3].value" -type "string" "false";
	setAttr ".stringOptions[3].type" -type "string" "boolean";
	setAttr ".stringOptions[4].name" -type "string" "importon density";
	setAttr ".stringOptions[4].value" -type "string" "1.0";
	setAttr ".stringOptions[4].type" -type "string" "scalar";
	setAttr ".stringOptions[5].name" -type "string" "importon merge";
	setAttr ".stringOptions[5].value" -type "string" "0.0";
	setAttr ".stringOptions[5].type" -type "string" "scalar";
	setAttr ".stringOptions[6].name" -type "string" "importon trace depth";
	setAttr ".stringOptions[6].value" -type "string" "0";
	setAttr ".stringOptions[6].type" -type "string" "integer";
	setAttr ".stringOptions[7].name" -type "string" "importon traverse";
	setAttr ".stringOptions[7].value" -type "string" "true";
	setAttr ".stringOptions[7].type" -type "string" "boolean";
	setAttr ".stringOptions[8].name" -type "string" "shadowmap pixel samples";
	setAttr ".stringOptions[8].value" -type "string" "3";
	setAttr ".stringOptions[8].type" -type "string" "integer";
	setAttr ".stringOptions[9].name" -type "string" "ambient occlusion";
	setAttr ".stringOptions[9].value" -type "string" "false";
	setAttr ".stringOptions[9].type" -type "string" "boolean";
	setAttr ".stringOptions[10].name" -type "string" "ambient occlusion rays";
	setAttr ".stringOptions[10].value" -type "string" "64";
	setAttr ".stringOptions[10].type" -type "string" "integer";
	setAttr ".stringOptions[11].name" -type "string" "ambient occlusion cache";
	setAttr ".stringOptions[11].value" -type "string" "false";
	setAttr ".stringOptions[11].type" -type "string" "boolean";
	setAttr ".stringOptions[12].name" -type "string" "ambient occlusion cache density";
	setAttr ".stringOptions[12].value" -type "string" "1.0";
	setAttr ".stringOptions[12].type" -type "string" "scalar";
	setAttr ".stringOptions[13].name" -type "string" "ambient occlusion cache points";
	setAttr ".stringOptions[13].value" -type "string" "64";
	setAttr ".stringOptions[13].type" -type "string" "integer";
	setAttr ".stringOptions[14].name" -type "string" "irradiance particles";
	setAttr ".stringOptions[14].value" -type "string" "false";
	setAttr ".stringOptions[14].type" -type "string" "boolean";
	setAttr ".stringOptions[15].name" -type "string" "irradiance particles rays";
	setAttr ".stringOptions[15].value" -type "string" "256";
	setAttr ".stringOptions[15].type" -type "string" "integer";
	setAttr ".stringOptions[16].name" -type "string" "irradiance particles interpolate";
	setAttr ".stringOptions[16].value" -type "string" "1";
	setAttr ".stringOptions[16].type" -type "string" "integer";
	setAttr ".stringOptions[17].name" -type "string" "irradiance particles interppoints";
	setAttr ".stringOptions[17].value" -type "string" "64";
	setAttr ".stringOptions[17].type" -type "string" "integer";
	setAttr ".stringOptions[18].name" -type "string" "irradiance particles indirect passes";
	setAttr ".stringOptions[18].value" -type "string" "0";
	setAttr ".stringOptions[18].type" -type "string" "integer";
	setAttr ".stringOptions[19].name" -type "string" "irradiance particles scale";
	setAttr ".stringOptions[19].value" -type "string" "1.0";
	setAttr ".stringOptions[19].type" -type "string" "scalar";
	setAttr ".stringOptions[20].name" -type "string" "irradiance particles env";
	setAttr ".stringOptions[20].value" -type "string" "true";
	setAttr ".stringOptions[20].type" -type "string" "boolean";
	setAttr ".stringOptions[21].name" -type "string" "irradiance particles env rays";
	setAttr ".stringOptions[21].value" -type "string" "256";
	setAttr ".stringOptions[21].type" -type "string" "integer";
	setAttr ".stringOptions[22].name" -type "string" "irradiance particles env scale";
	setAttr ".stringOptions[22].value" -type "string" "1";
	setAttr ".stringOptions[22].type" -type "string" "integer";
	setAttr ".stringOptions[23].name" -type "string" "irradiance particles rebuild";
	setAttr ".stringOptions[23].value" -type "string" "true";
	setAttr ".stringOptions[23].type" -type "string" "boolean";
	setAttr ".stringOptions[24].name" -type "string" "irradiance particles file";
	setAttr ".stringOptions[24].value" -type "string" "";
	setAttr ".stringOptions[24].type" -type "string" "string";
	setAttr ".stringOptions[25].name" -type "string" "geom displace motion factor";
	setAttr ".stringOptions[25].value" -type "string" "1.0";
	setAttr ".stringOptions[25].type" -type "string" "scalar";
	setAttr ".stringOptions[26].name" -type "string" "contrast all buffers";
	setAttr ".stringOptions[26].value" -type "string" "true";
	setAttr ".stringOptions[26].type" -type "string" "boolean";
	setAttr ".stringOptions[27].name" -type "string" "finalgather normal tolerance";
	setAttr ".stringOptions[27].value" -type "string" "25.842";
	setAttr ".stringOptions[27].type" -type "string" "scalar";
	setAttr ".stringOptions[28].name" -type "string" "trace camera clip";
	setAttr ".stringOptions[28].value" -type "string" "false";
	setAttr ".stringOptions[28].type" -type "string" "boolean";
	setAttr ".stringOptions[29].name" -type "string" "unified sampling";
	setAttr ".stringOptions[29].value" -type "string" "true";
	setAttr ".stringOptions[29].type" -type "string" "boolean";
	setAttr ".stringOptions[30].name" -type "string" "samples quality";
	setAttr ".stringOptions[30].value" -type "string" "0.25 0.25 0.25 0.25";
	setAttr ".stringOptions[30].type" -type "string" "color";
	setAttr ".stringOptions[31].name" -type "string" "samples min";
	setAttr ".stringOptions[31].value" -type "string" "1.0";
	setAttr ".stringOptions[31].type" -type "string" "scalar";
	setAttr ".stringOptions[32].name" -type "string" "samples max";
	setAttr ".stringOptions[32].value" -type "string" "100.0";
	setAttr ".stringOptions[32].type" -type "string" "scalar";
	setAttr ".stringOptions[33].name" -type "string" "samples error cutoff";
	setAttr ".stringOptions[33].value" -type "string" "0.0 0.0 0.0 0.0";
	setAttr ".stringOptions[33].type" -type "string" "color";
	setAttr ".stringOptions[34].name" -type "string" "samples per object";
	setAttr ".stringOptions[34].value" -type "string" "false";
	setAttr ".stringOptions[34].type" -type "string" "boolean";
	setAttr ".stringOptions[35].name" -type "string" "progressive";
	setAttr ".stringOptions[35].value" -type "string" "false";
	setAttr ".stringOptions[35].type" -type "string" "boolean";
	setAttr ".stringOptions[36].name" -type "string" "progressive max time";
	setAttr ".stringOptions[36].value" -type "string" "0";
	setAttr ".stringOptions[36].type" -type "string" "integer";
	setAttr ".stringOptions[37].name" -type "string" "progressive subsampling size";
	setAttr ".stringOptions[37].value" -type "string" "4";
	setAttr ".stringOptions[37].type" -type "string" "integer";
	setAttr ".stringOptions[38].name" -type "string" "iray";
	setAttr ".stringOptions[38].value" -type "string" "false";
	setAttr ".stringOptions[38].type" -type "string" "boolean";
	setAttr ".stringOptions[39].name" -type "string" "light relative scale";
	setAttr ".stringOptions[39].value" -type "string" "0.31831";
	setAttr ".stringOptions[39].type" -type "string" "scalar";
	setAttr ".stringOptions[40].name" -type "string" "trace camera motion vectors";
	setAttr ".stringOptions[40].value" -type "string" "false";
	setAttr ".stringOptions[40].type" -type "string" "boolean";
	setAttr ".stringOptions[41].name" -type "string" "ray differentials";
	setAttr ".stringOptions[41].value" -type "string" "true";
	setAttr ".stringOptions[41].type" -type "string" "boolean";
	setAttr ".stringOptions[42].name" -type "string" "environment lighting mode";
	setAttr ".stringOptions[42].value" -type "string" "off";
	setAttr ".stringOptions[42].type" -type "string" "string";
	setAttr ".stringOptions[43].name" -type "string" "environment lighting quality";
	setAttr ".stringOptions[43].value" -type "string" "0.2";
	setAttr ".stringOptions[43].type" -type "string" "scalar";
	setAttr ".stringOptions[44].name" -type "string" "environment lighting shadow";
	setAttr ".stringOptions[44].value" -type "string" "transparent";
	setAttr ".stringOptions[44].type" -type "string" "string";
	setAttr ".stringOptions[45].name" -type "string" "environment lighting resolution";
	setAttr ".stringOptions[45].value" -type "string" "512";
	setAttr ".stringOptions[45].type" -type "string" "integer";
	setAttr ".stringOptions[46].name" -type "string" "environment lighting shader samples";
	setAttr ".stringOptions[46].value" -type "string" "2";
	setAttr ".stringOptions[46].type" -type "string" "integer";
	setAttr ".stringOptions[47].name" -type "string" "environment lighting scale";
	setAttr ".stringOptions[47].value" -type "string" "1.0 1.0 1.0";
	setAttr ".stringOptions[47].type" -type "string" "color";
createNode mentalrayFramebuffer -s -n "miDefaultFramebuffer";
createNode lightLinker -s -n "lightLinker1";
	setAttr -s 10 ".lnk";
	setAttr -s 10 ".slnk";
createNode displayLayerManager -n "layerManager";
createNode displayLayer -n "defaultLayer";
createNode renderLayerManager -n "renderLayerManager";
createNode renderLayer -n "defaultRenderLayer";
	setAttr ".g" yes;
createNode shadingEngine -n "pz_fence:pz_fence";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "pz_fence:materialInfo1";
createNode lambert -n "pz_fence:pz_fence1";
	setAttr ".ambc" -type "float3" 0.40000001 0.40000001 0.40000001 ;
createNode file -n "pz_fence:pz_fence1F";
	setAttr ".ftn" -type "string" "pz-fence.png";
createNode place2dTexture -n "pz_fence:pz_fence1P2D";
createNode shadingEngine -n "cube_0_5:lambert2SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "cube_0_5:materialInfo1";
createNode lambert -n "cube_0_5:lambert2SG1";
createNode file -n "cube_0_5:lambert2SG1F";
	setAttr ".ftn" -type "string" "plat_wood.png";
createNode place2dTexture -n "cube_0_5:lambert2SG1P2D";
createNode polyCube -n "polyCube1";
	setAttr ".w" 277.507329505972;
	setAttr ".h" 178.05512630105829;
	setAttr ".d" 279.84209042877052;
	setAttr ".cuv" 4;
createNode polyNormal -n "polyNormal1";
	setAttr ".ics" -type "componentList" 1 "f[*]";
	setAttr ".nm" 4;
createNode deleteComponent -n "deleteComponent1";
	setAttr ".dc" -type "componentList" 1 "f[1]";
createNode deleteComponent -n "deleteComponent2";
	setAttr ".dc" -type "componentList" 1 "f[0]";
createNode polyCube -n "polyCube2";
	setAttr ".w" 2.760937178339077;
	setAttr ".h" 8.3521455386195811;
	setAttr ".d" 299.03763107569182;
	setAttr ".cuv" 4;
createNode polySplitRing -n "polySplitRing1";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 2 "e[6:7]" "e[10:11]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 391.39726073777626 4.1760727693097905 76.833833307612679 1;
	setAttr ".wt" 0.99138814210891724;
	setAttr ".dr" no;
	setAttr ".re" 7;
	setAttr ".sma" 29.999999999999996;
	setAttr ".p[0]"  0 0 1;
	setAttr ".fq" yes;
createNode polyExtrudeFace -n "polyExtrudeFace1";
	setAttr ".ics" -type "componentList" 1 "f[9]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 391.39726073777626 4.1760727693097905 76.833833307612679 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 392.77774 4.1760726 -71.397354 ;
	setAttr ".rs" 32789;
	setAttr ".lt" -type "double3" -5.7424298734704776e-014 1.0020585855404484e-016 189.38394961637152 ;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" 392.77772934472512 1.7180612843503695e-007 -72.684980779301384 ;
	setAttr ".cbx" -type "double3" 392.77772934472512 8.3521453668134527 -70.109724431645134 ;
createNode polySplitRing -n "polySplitRing2";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 3 "e[20:21]" "e[23]" "e[25]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 391.39726073777626 4.1760727693097905 76.833833307612679 1;
	setAttr ".wt" 0.98662596940994263;
	setAttr ".dr" no;
	setAttr ".re" 21;
	setAttr ".sma" 29.999999999999996;
	setAttr ".p[0]"  0 0 1;
	setAttr ".fq" yes;
createNode polyExtrudeEdge -n "polyExtrudeEdge1";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 4 "e[22]" "e[28]" "e[33]" "e[35]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 391.39726073777626 4.1760727693097905 76.833833307612679 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 580.89526 4.1760726 -70.109726 ;
	setAttr ".rs" 56036;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" 579.62887389695595 1.7180612843503695e-007 -70.109724431645134 ;
	setAttr ".cbx" -type "double3" 582.16169555222939 8.3521453668134527 -70.109724431645134 ;
createNode polyExtrudeFace -n "polyExtrudeFace2";
	setAttr ".ics" -type "componentList" 1 "f[17]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 391.39726073777626 4.1760727693097905 76.833833307612679 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 580.89526 4.1760726 -70.109726 ;
	setAttr ".rs" 64541;
	setAttr ".lt" -type "double3" 0 0 42.412643818944623 ;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" 579.62885863816689 1.7180612843503695e-007 -70.109724431645134 ;
	setAttr ".cbx" -type "double3" 582.16169555222939 8.3521453668134527 -70.109724431645134 ;
createNode polySplitRing -n "polySplitRing3";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 3 "e[44:45]" "e[47]" "e[49]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 391.39726073777626 4.1760727693097905 76.833833307612679 1;
	setAttr ".wt" 0.84509873390197754;
	setAttr ".dr" no;
	setAttr ".re" 49;
	setAttr ".sma" 29.999999999999996;
	setAttr ".p[0]"  0 0 1;
	setAttr ".fq" yes;
createNode polyExtrudeFace -n "polyExtrudeFace3";
	setAttr ".cch" yes;
	setAttr ".ics" -type "componentList" 1 "f[29]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 391.39726073777626 4.1760727693097905 76.833833307612679 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 580.89526 8.3521452 -30.981966 ;
	setAttr ".rs" 43854;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" 579.62885863816689 8.3521453668134527 -34.266851812016228 ;
	setAttr ".cbx" -type "double3" 582.16169555222939 8.3521453668134527 -27.697080999027946 ;
createNode polyExtrudeFace -n "polyExtrudeFace4";
	setAttr ".ics" -type "componentList" 1 "f[29]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 391.39726073777626 4.1760727693097905 76.833833307612679 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 580.89526 8.3521452 -30.981966 ;
	setAttr ".rs" 35766;
	setAttr ".lt" -type "double3" 0 3.7272866705371619e-015 80.786206860534179 ;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" 579.62885863816689 8.3521453668134527 -34.266851812016228 ;
	setAttr ".cbx" -type "double3" 582.16169555222939 8.3521453668134527 -27.697080999027946 ;
createNode polySplitRing -n "polySplitRing4";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 3 "e[68:69]" "e[71]" "e[73]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 391.39726073777626 4.1760727693097905 76.833833307612679 1;
	setAttr ".wt" 0.92444896697998047;
	setAttr ".dr" no;
	setAttr ".re" 71;
	setAttr ".sma" 29.999999999999996;
	setAttr ".p[0]"  0 0 1;
	setAttr ".fq" yes;
createNode polyExtrudeEdge -n "polyExtrudeEdge2";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 4 "e[74]" "e[76]" "e[81]" "e[83]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 391.39726073777626 4.1760727693097905 76.833833307612679 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 580.89526 86.086609 -27.697081 ;
	setAttr ".rs" 37177;
	setAttr ".c[0]"  0 1 1;
	setAttr ".sma" 0;
	setAttr ".cbn" -type "double3" 579.62885863816689 83.034867935325423 -27.697080999027946 ;
	setAttr ".cbx" -type "double3" 582.16169555222939 89.138353042747298 -27.697080999027946 ;
createNode polyExtrudeEdge -n "polyExtrudeEdge3";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 3 "e[86]" "e[88]" "e[90:91]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 391.39726073777626 4.1760727693097905 76.833833307612679 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 580.89526 86.086609 -27.697081 ;
	setAttr ".rs" 60712;
	setAttr ".lt" -type "double3" 0 0 51.699749525070906 ;
	setAttr ".c[0]"  0 1 1;
	setAttr ".sma" 0;
	setAttr ".cbn" -type "double3" 579.62885863816689 83.034867935325423 -27.697080999027946 ;
	setAttr ".cbx" -type "double3" 582.16169555222939 89.138353042747298 -27.697080999027946 ;
createNode script -n "uiConfigurationScriptNode";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"top\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n"
		+ "                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 0\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"vp2Renderer\" \n                -objectFilterShowInHUD 1\n"
		+ "                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n"
		+ "                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n"
		+ "            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n"
		+ "            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"side\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"smoothShaded\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 0\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n"
		+ "                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"vp2Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n"
		+ "                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n"
		+ "                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"side\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n"
		+ "            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n"
		+ "            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n"
		+ "\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"front\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 0\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n"
		+ "                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"vp2Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n"
		+ "                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n"
		+ "                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n"
		+ "            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n"
		+ "            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n"
		+ "            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"smoothShaded\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n"
		+ "                -bufferMode \"double\" \n                -twoSidedLighting 0\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 1\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"vp2Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n"
		+ "                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n"
		+ "                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n"
		+ "            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 1\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n"
		+ "            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n"
		+ "            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n                -docTag \"isolOutln_fromSeln\" \n                -showShapes 0\n"
		+ "                -showReferenceNodes 1\n                -showReferenceMembers 1\n                -showAttributes 0\n                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 1\n                -showAssets 1\n                -showContainedOnly 1\n                -showPublishedAsConnected 0\n                -showContainerContents 1\n                -ignoreDagHierarchy 0\n                -expandConnections 0\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"defaultSetFilter\" \n                -showSetMembers 1\n"
		+ "                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 0\n                -ignoreHiddenAttribute 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -docTag \"isolOutln_fromSeln\" \n"
		+ "            -showShapes 0\n            -showReferenceNodes 1\n            -showReferenceMembers 1\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n"
		+ "            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"graphEditor\" -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n"
		+ "            outlinerEditor -e \n                -showShapes 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n"
		+ "                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n"
		+ "                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showReferenceNodes 0\n"
		+ "                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n"
		+ "                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -autoFit 0\n"
		+ "                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dopeSheetPanel\" -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n"
		+ "            outlinerEditor -e \n                -showShapes 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n"
		+ "                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n"
		+ "                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n"
		+ "                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n"
		+ "                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"clipEditorPanel\" -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n"
		+ "                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"sequenceEditorPanel\" -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n"
		+ "                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperGraphPanel\" -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n"
		+ "                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n"
		+ "                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperShadePanel\" -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n"
		+ "\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"visorPanel\" -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"nodeEditorPanel\" (localizedPanelLabel(\"Node Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"nodeEditorPanel\" -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n"
		+ "                -defaultPinnedState 0\n                -ignoreAssets 1\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -keyReleaseCommand \"nodeEdKeyReleaseCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                $editorName;;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n"
		+ "                -autoSizeNodes 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -defaultPinnedState 0\n                -ignoreAssets 1\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -keyReleaseCommand \"nodeEdKeyReleaseCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                $editorName;;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"createNodePanel\" (localizedPanelLabel(\"Create Node\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"createNodePanel\" -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Texture Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"polyTexturePlacementPanel\" -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"renderWindowPanel\" -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"blendShapePanel\" (localizedPanelLabel(\"Blend Shape\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tblendShapePanel -unParent -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tblendShapePanel -edit -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" == $panelName) {\n"
		+ "\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynRelEdPanel\" -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"relationshipPanel\" -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"referenceEditorPanel\" -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"componentEditorPanel\" -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynPaintScriptedPanelType\" -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"scriptEditorPanel\" -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n"
		+ "        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-defaultImage \"vacantCell.xP:/\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"quad\\\" -ps 1 50 50 -ps 2 50 50 -ps 3 50 50 -ps 4 50 50 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Top View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Top View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera top` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Top View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera top` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 1\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 1\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Side View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Side View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera side` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Side View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera side` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Front View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Front View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera front` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Front View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera front` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        setFocus `paneLayout -q -p1 $gMainPane`;\n        sceneUIReplacement -deleteRemaining;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 120 -ast 1 -aet 200 ";
	setAttr ".st" 6;
createNode polySplitRing -n "polySplitRing5";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 3 "e[92:93]" "e[95]" "e[97]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 391.39726073777626 4.1760727693097905 76.833833307612679 1;
	setAttr ".wt" 0.89815741777420044;
	setAttr ".dr" no;
	setAttr ".re" 97;
	setAttr ".sma" 29.999999999999996;
	setAttr ".p[0]"  0 0 1;
	setAttr ".fq" yes;
createNode polyExtrudeFace -n "polyExtrudeFace5";
	setAttr ".ics" -type "componentList" 1 "f[53]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 391.39726073777626 4.1760727693097905 76.833833307612679 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 580.89526 83.034866 20.810911 ;
	setAttr ".rs" 49980;
	setAttr ".lt" -type "double3" 0 2.8627701467181001e-015 -83.107229433992984 ;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" 579.62885863816689 83.034867935325423 17.619149774897835 ;
	setAttr ".cbx" -type "double3" 582.16169555222939 83.034867935325423 24.002671808589241 ;
createNode polyTweak -n "polyTweak1";
	setAttr ".uopa" yes;
	setAttr -s 4 ".tk[52:55]" -type "float3"  0 0 -1.11828589 0 0 -1.11828589
		 0 0 -1.11828589 0 0 -1.11828589;
createNode polySplitRing -n "polySplitRing6";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 3 "e[108:109]" "e[111]" "e[113]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 391.39726073777626 4.1760727693097905 76.833833307612679 1;
	setAttr ".wt" 0.91541481018066406;
	setAttr ".dr" no;
	setAttr ".re" 113;
	setAttr ".sma" 29.999999999999996;
	setAttr ".p[0]"  0 0 1;
	setAttr ".fq" yes;
createNode polyExtrudeFace -n "polyExtrudeFace6";
	setAttr ".ics" -type "componentList" 1 "f[58]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 391.39726073777626 4.1760727693097905 76.833833307612679 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 580.89526 4.1158338 24.002672 ;
	setAttr ".rs" 64835;
	setAttr ".lt" -type "double3" 0 5.0983302963132042e-017 -247.00223438976818 ;
	setAttr ".c[0]"  0 1 1;
	setAttr ".sma" 0;
	setAttr ".cbn" -type "double3" 579.62885863816689 -0.072364635322777815 24.002671808589241 ;
	setAttr ".cbx" -type "double3" 582.16169555222939 8.3040324975507573 24.002671808589241 ;
createNode polyTweak -n "polyTweak2";
	setAttr ".uopa" yes;
	setAttr -s 4 ".tk[60:63]" -type "float3"  0 1.34675586 0 0 1.34675586
		 0 0 1.34675586 0 0 1.34675586 0;
createNode polySplitRing -n "polySplitRing7";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 2 "e[5:6]" "e[9:10]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 135.84411577917447 89.027563150529147 138.02879205537695 1;
	setAttr ".wt" 0.3220900297164917;
	setAttr ".re" 6;
	setAttr ".sma" 29.999999999999996;
	setAttr ".p[0]"  0 0 1;
	setAttr ".fq" yes;
createNode polySplitRing -n "polySplitRing8";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 2 "e[9:11]" "e[16]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 135.84411577917447 89.027563150529147 138.02879205537695 1;
	setAttr ".wt" 0.49681857228279114;
	setAttr ".re" 11;
	setAttr ".sma" 29.999999999999996;
	setAttr ".p[0]"  0 0 1;
	setAttr ".fq" yes;
createNode polySplitRing -n "polySplitRing9";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 6 "e[3:4]" "e[7:8]" "e[13]" "e[17]" "e[20]" "e[24]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 135.84411577917447 89.027563150529147 138.02879205537695 1;
	setAttr ".wt" 0.44341328740119934;
	setAttr ".re" 8;
	setAttr ".sma" 29.999999999999996;
	setAttr ".p[0]"  0 0 1;
	setAttr ".fq" yes;
createNode polyTweak -n "polyTweak3";
	setAttr ".uopa" yes;
	setAttr -s 8 ".tk[8:15]" -type "float3"  0 0 -95.50197601 0 0 -95.50197601
		 0 0 -95.50197601 0 0 -95.50197601 0 0 -53.30342484 0 0 -53.30342484 0 0 -53.30342484
		 0 0 -53.30342484;
createNode polyCube -n "polyCube3";
	setAttr ".ax" -type "double3" 0 1.0000000000000002 2.2204460492503131e-016 ;
	setAttr ".w" 14.436339730031193;
	setAttr ".h" 24.430728773899034;
	setAttr ".d" 79.955112350942301;
	setAttr ".cuv" 4;
createNode polySplitRing -n "polySplitRing10";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 2 "e[6:7]" "e[10:11]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1.0000000000000004 0 0 0 0 1 0 352.98321714906137 36.259267847276355 157.75470952471883 1;
	setAttr ".wt" 0.091514147818088531;
	setAttr ".re" 7;
	setAttr ".sma" 29.999999999999996;
	setAttr ".p[0]"  0 0 1;
	setAttr ".fq" yes;
createNode polySplitRing -n "polySplitRing11";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "e[10:13]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1.0000000000000004 0 0 0 0 1 0 352.98321714906137 36.259267847276355 157.75470952471883 1;
	setAttr ".wt" 0.86847269535064697;
	setAttr ".dr" no;
	setAttr ".re" 12;
	setAttr ".sma" 29.999999999999996;
	setAttr ".p[0]"  0 0 1;
	setAttr ".fq" yes;
createNode polyExtrudeFace -n "polyExtrudeFace7";
	setAttr ".ics" -type "componentList" 1 "f[3]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1.0000000000000004 0 0 0 0 1 0 352.98321714906137 36.259267847276355 157.75470952471883 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 350.86169 24.043909 183.48296 ;
	setAttr ".rs" 51968;
	setAttr ".lt" -type "double3" 0 -3.9830214683905466e-015 17.937934000852351 ;
	setAttr ".c[0]"  0 1 1;
	setAttr ".sma" 0;
	setAttr ".cbn" -type "double3" 347.73185148744028 24.043909113145489 180.85302762469442 ;
	setAttr ".cbx" -type "double3" 353.99154034165781 24.043909113145489 186.11289433917196 ;
createNode polyExtrudeFace -n "polyExtrudeFace8";
	setAttr ".ics" -type "componentList" 1 "f[12]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1.0000000000000004 0 0 0 0 1 0 352.98321714906137 36.259267847276355 157.75470952471883 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 350.86169 24.043909 132.07083 ;
	setAttr ".rs" 57005;
	setAttr ".lt" -type "double3" 0 -4.0337480142371771e-015 18.166386053825029 ;
	setAttr ".c[0]"  0 1 1;
	setAttr ".sma" 0;
	setAttr ".cbn" -type "double3" 347.73184385804575 24.043909113145489 128.63691167315633 ;
	setAttr ".cbx" -type "double3" 353.9915484478895 24.043909113145489 135.50475530108602 ;
createNode polyTweak -n "polyTweak4";
	setAttr ".uopa" yes;
	setAttr -s 8 ".tk[16:23]" -type "float3"  0 -13.87170315 0 0 -13.87170315
		 0 0 -13.87170315 0 0 -13.87170315 0 0 -13.87170315 0 0 -13.87170315 0 0 -13.87170315
		 0 0 -13.87170315 0;
createNode deleteComponent -n "deleteComponent3";
	setAttr ".dc" -type "componentList" 1 "f[15]";
createNode polySplitRing -n "polySplitRing12";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 6 "e[3]" "e[17]" "e[24]" "e[30]" "e[32]" "e[34]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 135.84411577917447 89.027563150529147 138.02879205537695 1;
	setAttr ".wt" 0.51469868421554565;
	setAttr ".dr" no;
	setAttr ".re" 3;
	setAttr ".sma" 29.999999999999996;
	setAttr ".p[0]"  0 0 1;
	setAttr ".fq" yes;
createNode deleteComponent -n "deleteComponent4";
	setAttr ".dc" -type "componentList" 1 "e[48]";
createNode deleteComponent -n "deleteComponent5";
	setAttr ".dc" -type "componentList" 1 "e[49]";
createNode deleteComponent -n "deleteComponent6";
	setAttr ".dc" -type "componentList" 1 "f[11]";
createNode deleteComponent -n "deleteComponent7";
	setAttr ".dc" -type "componentList" 1 "e[33]";
createNode lambert -n "lambert2";
createNode shadingEngine -n "lambert2SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
createNode file -n "file1";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//wallpaper1.png";
createNode place2dTexture -n "place2dTexture1";
createNode polyTweak -n "polyTweak5";
	setAttr ".uopa" yes;
	setAttr -s 4 ".tk";
	setAttr ".tk[17]" -type "float3" 0 6.847764 -8.9020929 ;
	setAttr ".tk[18]" -type "float3" 0 6.5053759 -8.2173166 ;
	setAttr ".tk[25]" -type "float3" 0 -4.7934351 -8.2173166 ;
	setAttr ".tk[26]" -type "float3" 0 -5.4782114 -7.5325413 ;
createNode deleteComponent -n "deleteComponent8";
	setAttr ".dc" -type "componentList" 1 "f[1]";
createNode deleteComponent -n "deleteComponent9";
	setAttr ".dc" -type "componentList" 1 "f[4]";
createNode deleteComponent -n "deleteComponent10";
	setAttr ".dc" -type "componentList" 1 "f[6]";
createNode polyTweakUV -n "polyTweakUV1";
	setAttr ".uopa" yes;
	setAttr -s 38 ".uvtk[0:37]" -type "float2" 11.001958847 -2.23679924
		 -3.18679285 -2.31500077 11.001958847 3.14961147 -3.18679285 3.21812725 -8.93329906
		 -2.82390308 -3.45778108 -2.82390308 -8.93329906 2.65161467 -3.45778108 2.65161467
		 2.34633517 -2.31500077 2.34633517 3.21812725 3.52731323 -2.23679924 3.52731323 3.14961147
		 -1.40462792 3.21812725 -1.40462792 -2.31500077 8.5944519 -2.23679924 8.5944519 3.14961147
		 0.4589206 3.21812725 0.4589206 -2.31500077 6.077003002 -2.23679924 6.077003002 3.14961147
		 11.001958847 0.76120579 8.5944519 0.76120579 6.077003002 0.76120579 -8.93329906 -0.39598599
		 3.52731323 0.76120579 2.34633517 0.7646659 -3.45778108 -0.39598599 0.4589206 0.7646659
		 -1.40462661 0.7646659 -3.18679285 0.7646659 11.001958847 -0.69372988 8.5944519 -0.69372988
		 6.077003002 -0.69372988 -8.93329906 1.083019853 3.52731323 -0.69372988 2.34633517
		 -0.72990066 -3.45778108 1.083019853 0.4589206 -0.72990066;
createNode lambert -n "white_wood";
createNode shadingEngine -n "lambert3SG";
	setAttr ".ihi" 0;
	setAttr -s 6 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 5 ".gn";
createNode materialInfo -n "materialInfo2";
createNode file -n "file2";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//white-wood.png";
createNode place2dTexture -n "place2dTexture2";
createNode polyTweakUV -n "polyTweakUV2";
	setAttr ".uopa" yes;
	setAttr -s 69 ".uvtk";
	setAttr ".uvtk[30]" -type "float2" -0.16801292 -0.16801292 ;
	setAttr ".uvtk[31]" -type "float2" 0.16801286 -0.16801292 ;
	setAttr ".uvtk[32]" -type "float2" 0.16801286 0.16801286 ;
	setAttr ".uvtk[33]" -type "float2" -0.16801292 0.16801286 ;
	setAttr ".uvtk[34]" -type "float2" -0.16801292 -0.16801292 ;
	setAttr ".uvtk[35]" -type "float2" 0.16801286 -0.16801292 ;
	setAttr ".uvtk[36]" -type "float2" 0.16801286 0.16801286 ;
	setAttr ".uvtk[37]" -type "float2" -0.16801292 0.16801286 ;
	setAttr ".uvtk[38]" -type "float2" -0.16801292 -0.16801292 ;
	setAttr ".uvtk[39]" -type "float2" 0.16801286 -0.16801292 ;
	setAttr ".uvtk[40]" -type "float2" 0.16801286 0.16801286 ;
	setAttr ".uvtk[41]" -type "float2" -0.16801292 0.16801286 ;
	setAttr ".uvtk[42]" -type "float2" -0.16801292 -0.16801292 ;
	setAttr ".uvtk[43]" -type "float2" 0.16801286 -0.16801292 ;
	setAttr ".uvtk[44]" -type "float2" 0.16801286 0.16801286 ;
	setAttr ".uvtk[45]" -type "float2" -0.16801292 0.16801286 ;
	setAttr ".uvtk[66]" -type "float2" -0.16801292 -0.16801292 ;
	setAttr ".uvtk[67]" -type "float2" 0.16801286 -0.16801292 ;
	setAttr ".uvtk[68]" -type "float2" 0.16801286 0.16801286 ;
	setAttr ".uvtk[69]" -type "float2" -0.16801292 0.16801286 ;
	setAttr ".uvtk[70]" -type "float2" -0.16801292 -0.16801292 ;
	setAttr ".uvtk[71]" -type "float2" 0.16801286 -0.16801292 ;
	setAttr ".uvtk[72]" -type "float2" 0.16801286 0.16801286 ;
	setAttr ".uvtk[73]" -type "float2" -0.16801292 0.16801286 ;
	setAttr ".uvtk[74]" -type "float2" -0.16801292 -0.16801292 ;
	setAttr ".uvtk[75]" -type "float2" 0.16801286 -0.16801292 ;
	setAttr ".uvtk[76]" -type "float2" 0.16801286 0.16801286 ;
	setAttr ".uvtk[77]" -type "float2" -0.16801292 0.16801286 ;
	setAttr ".uvtk[78]" -type "float2" -0.16801292 -0.16801292 ;
	setAttr ".uvtk[79]" -type "float2" 0.16801286 -0.16801292 ;
	setAttr ".uvtk[80]" -type "float2" 0.16801286 0.16801286 ;
	setAttr ".uvtk[81]" -type "float2" -0.16801292 0.16801286 ;
	setAttr ".uvtk[82]" -type "float2" -0.16801292 -0.16801292 ;
	setAttr ".uvtk[83]" -type "float2" 0.16801286 -0.16801292 ;
	setAttr ".uvtk[84]" -type "float2" 0.16801286 0.16801286 ;
	setAttr ".uvtk[85]" -type "float2" -0.16801292 0.16801286 ;
	setAttr ".uvtk[86]" -type "float2" -0.16801292 -0.16801292 ;
	setAttr ".uvtk[87]" -type "float2" 0.16801286 -0.16801292 ;
	setAttr ".uvtk[88]" -type "float2" 0.16801286 0.16801286 ;
	setAttr ".uvtk[89]" -type "float2" -0.16801292 0.16801286 ;
	setAttr ".uvtk[90]" -type "float2" -0.16801292 -0.16801292 ;
	setAttr ".uvtk[91]" -type "float2" 0.16801286 -0.16801292 ;
	setAttr ".uvtk[92]" -type "float2" 0.16801286 0.16801286 ;
	setAttr ".uvtk[93]" -type "float2" -0.16801292 0.16801286 ;
	setAttr ".uvtk[94]" -type "float2" -0.16801292 -0.16801292 ;
	setAttr ".uvtk[95]" -type "float2" 0.16801286 -0.16801292 ;
	setAttr ".uvtk[96]" -type "float2" 0.16801286 0.16801286 ;
	setAttr ".uvtk[97]" -type "float2" -0.16801292 0.16801286 ;
	setAttr ".uvtk[98]" -type "float2" -0.16801292 0.13379115 ;
	setAttr ".uvtk[99]" -type "float2" -0.16801292 0.13379115 ;
	setAttr ".uvtk[100]" -type "float2" 0.16801286 0.13379115 ;
	setAttr ".uvtk[101]" -type "float2" 0.16801286 0.13379115 ;
	setAttr ".uvtk[102]" -type "float2" 0.16801286 0.13379115 ;
	setAttr ".uvtk[103]" -type "float2" -0.16801292 0.13379115 ;
	setAttr ".uvtk[104]" -type "float2" 0.16801286 0.13379115 ;
	setAttr ".uvtk[105]" -type "float2" -0.16801292 0.13379115 ;
	setAttr ".uvtk[106]" -type "float2" -0.16801292 0.13379115 ;
	setAttr ".uvtk[107]" -type "float2" 0.16801286 0.13379115 ;
	setAttr ".uvtk[108]" -type "float2" 0.16801286 0.16801286 ;
	setAttr ".uvtk[109]" -type "float2" -0.16801292 0.16801286 ;
	setAttr ".uvtk[110]" -type "float2" -0.16801292 0.16801286 ;
	setAttr ".uvtk[111]" -type "float2" 0.16801286 0.16801286 ;
	setAttr ".uvtk[112]" -type "float2" 0.16801286 0.13379115 ;
	setAttr ".uvtk[113]" -type "float2" -0.16801292 0.13379115 ;
	setAttr ".uvtk[114]" -type "float2" 0.16801286 0.16801286 ;
	setAttr ".uvtk[115]" -type "float2" -0.16801292 0.16801286 ;
	setAttr ".uvtk[116]" -type "float2" -0.16801292 0.16801286 ;
	setAttr ".uvtk[117]" -type "float2" 0.16801286 0.16801286 ;
createNode polyTriangulate -n "polyTriangulate1";
	setAttr ".ics" -type "componentList" 1 "f[*]";
createNode polyTriangulate -n "polyTriangulate2";
	setAttr ".ics" -type "componentList" 1 "f[*]";
createNode polyTweak -n "polyTweak6";
	setAttr ".uopa" yes;
	setAttr -s 56 ".tk[12:67]" -type "float3"  85.50759125 0 0 85.50759125
		 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0
		 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0
		 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125
		 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0
		 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0
		 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125
		 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0
		 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0
		 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125
		 0 0 85.50759125 0 0 85.50759125 0 0 85.50759125 0 -43.47821426 85.50759125 0 -43.47821426
		 85.50759125 0 -43.47821426 85.50759125 0 -43.47821426;
createNode polyTriangulate -n "polyTriangulate3";
	setAttr ".ics" -type "componentList" 1 "f[*]";
createNode polyTweak -n "polyTweak7";
	setAttr ".uopa" yes;
	setAttr -s 8 ".tk[0:7]" -type "float3"  0 5.4836273e-006 0 -4.24304199
		 5.4836273e-006 0 0 -16.26499557 0 -4.24304199 -16.26499557 0 0 -16.26499557 0 -4.24304199
		 -16.26499557 0 0 5.4836273e-006 0 -4.24304199 5.4836273e-006 0;
createNode polyTriangulate -n "polyTriangulate4";
	setAttr ".ics" -type "componentList" 1 "f[*]";
createNode polyTweak -n "polyTweak8";
	setAttr ".uopa" yes;
	setAttr -s 24 ".tk[0:23]" -type "float3"  0.00012207031 32.89929962
		 3.8146973e-006 0.00012207031 32.89929962 3.8146973e-006 0.00012207031 32.89929962
		 3.8146973e-006 0.00012207031 32.89929962 3.8146973e-006 0.00012207031 32.89929962
		 3.8146973e-006 0.00012207031 32.89929962 3.8146973e-006 0.00012207031 32.89929962
		 3.8146973e-006 0.00012207031 32.89929962 3.8146973e-006 0.00012207031 32.89929962
		 3.8146973e-006 0.00012207031 32.89929962 3.8146973e-006 0.00012207031 32.89929962
		 3.8146973e-006 0.00012207031 32.89929962 3.8146973e-006 0.00012207031 32.89929962
		 3.8146973e-006 0.00012207031 32.89929962 3.8146973e-006 0.00012207031 32.89929962
		 3.8146973e-006 0.00012207031 32.89929962 3.8146973e-006 0.00012207031 3.8146973e-006
		 3.8146973e-006 0.00012207031 3.8146973e-006 3.8146973e-006 0.00012207031 3.8146973e-006
		 3.8146973e-006 0.00012207031 3.8146973e-006 3.8146973e-006 0.00012207031 3.8146973e-006
		 3.8146973e-006 0.00012207031 3.8146973e-006 3.8146973e-006 0.00012207031 3.8146973e-006
		 3.8146973e-006 0.00012207031 3.8146973e-006 3.8146973e-006;
createNode polyUnite -n "polyUnite1";
	setAttr -s 2 ".ip";
	setAttr -s 2 ".im";
createNode groupId -n "groupId1";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts1";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:11]";
createNode groupId -n "groupId2";
	setAttr ".ihi" 0;
createNode groupId -n "groupId3";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts2";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:43]";
createNode groupId -n "groupId4";
	setAttr ".ihi" 0;
createNode groupId -n "groupId5";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts3";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:55]";
createNode polyNormal -n "polyNormal2";
	setAttr ".ics" -type "componentList" 2 "f[92:115]" "f[118:131]";
	setAttr ".nm" 4;
createNode lambert -n "wood";
createNode shadingEngine -n "lambert4SG";
	setAttr ".ihi" 0;
	setAttr -s 5 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 5 ".gn";
createNode materialInfo -n "materialInfo3";
createNode file -n "file3";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//wood1.png";
createNode place2dTexture -n "place2dTexture3";
createNode file -n "file4";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//wood2.png";
createNode place2dTexture -n "place2dTexture4";
createNode polySeparate -n "polySeparate1";
	setAttr ".ic" 2;
	setAttr -s 2 ".out";
createNode groupId -n "groupId6";
	setAttr ".ihi" 0;
createNode groupId -n "groupId7";
	setAttr ".ihi" 0;
createNode groupId -n "groupId8";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts4";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:11]";
createNode groupId -n "groupId9";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts5";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:43]";
createNode polyCube -n "polyCube4";
	setAttr ".w" 52.377931334530274;
	setAttr ".h" 52.721339558123091;
	setAttr ".d" 63.965261523959782;
	setAttr ".cuv" 4;
createNode deleteComponent -n "deleteComponent11";
	setAttr ".dc" -type "componentList" 2 "f[0:3]" "f[5]";
createNode lambert -n "lambert5";
createNode shadingEngine -n "lambert5SG";
	setAttr ".ihi" 0;
	setAttr -s 3 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 3 ".gn";
createNode materialInfo -n "materialInfo4";
createNode file -n "blackboard";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//blackboard.png";
createNode place2dTexture -n "place2dTexture5";
createNode polyTweakUV -n "polyTweakUV3";
	setAttr ".uopa" yes;
	setAttr -s 4 ".uvtk[0:3]" -type "float2" -0.62306297 0.0013405979
		 -0.62306297 0.74447477 0.12007117 0.0013405979 0.12007117 0.74447477;
createNode polyTweakUV -n "polyTweakUV4";
	setAttr ".uopa" yes;
	setAttr -s 38 ".uvtk[0:37]" -type "float2" -0.21109319 -0.84437263 0.21109316
		 -0.84437263 -0.21109319 -0.42218632 0.21109316 -0.42218632 -0.21109319 -0.38355029
		 0.21109307 -0.38355029 -0.21109319 0 0.21109316 0 -0.21109319 0.42218637 0.21109316
		 0.42218637 -0.21109319 0.80573666 0.21109316 0.80573666 -0.21109319 0.84437263 0.21109316
		 0.84437263 0.24972916 -0.84437263 0.24972916 -0.42218632 -0.24972916 -0.84437263
		 -0.24972916 -0.42218632 -0.21109319 -0.050447375 0.21109316 -0.050447375 -0.5828321
		 -0.84437263 -0.5828321 -0.42218632 -0.21109319 0.47263378 0.21109316 0.47263378 -0.21109319
		 0.80573666 0.21109316 0.80573666 0.58283216 -0.42218632 0.58283216 -0.84437263 -0.63327944
		 -0.84437263 -0.63327944 -0.42218632 -0.21109319 0.42218637 0.21109316 0.42218637
		 -0.21109319 0.47263378 0.21109316 0.47263378 0.63327944 -0.42218632 0.63327944 -0.84437263
		 0.21109316 0.84437263 -0.21109319 0.84437263;
createNode polyCube -n "polyCube5";
	setAttr ".w" 104.99501328653173;
	setAttr ".h" 39.50264203138584;
	setAttr ".d" 106.82267334398159;
	setAttr ".cuv" 4;
createNode deleteComponent -n "deleteComponent12";
	setAttr ".dc" -type "componentList" 2 "f[0]" "f[2:5]";
createNode lambert -n "lambert6";
createNode shadingEngine -n "lambert6SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo5";
createNode file -n "file5";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//floor1.png";
createNode place2dTexture -n "place2dTexture6";
createNode polyTweakUV -n "polyTweakUV5";
	setAttr ".uopa" yes;
	setAttr -s 4 ".uvtk[0:3]" -type "float2" -5.75977755 -5.63850355 5.76760387
		 -5.63850355 -5.75977755 5.88888025 5.76760387 5.88888025;
createNode polyUnite -n "polyUnite2";
	setAttr -s 3 ".ip";
	setAttr -s 3 ".im";
createNode groupId -n "groupId10";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts6";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0]";
createNode groupId -n "groupId11";
	setAttr ".ihi" 0;
createNode groupId -n "groupId12";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts7";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:55]";
createNode groupId -n "groupId13";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts8";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[56]";
createNode polyTriangulate -n "polyTriangulate5";
	setAttr ".ics" -type "componentList" 1 "f[*]";
createNode polyTweak -n "polyTweak9";
	setAttr ".uopa" yes;
	setAttr -s 4 ".tk[0:3]" -type "float3"  -131.35173035 0 -33.635746
		 288.24786377 3.5527137e-015 -33.635746 -131.35168457 0 -290.44525146 288.24789429
		 3.5527137e-015 -290.44525146;
createNode polyCube -n "polyCube6";
	setAttr ".w" 48.351192852466269;
	setAttr ".h" 2.3175894449114622;
	setAttr ".d" 37.553502228555544;
	setAttr ".cuv" 4;
createNode polyCylinder -n "polyCylinder1";
	setAttr ".r" 18.261126462257909;
	setAttr ".h" 83.352394713467135;
	setAttr ".sa" 3;
	setAttr ".sc" 1;
	setAttr ".cuv" 3;
createNode deleteComponent -n "deleteComponent13";
	setAttr ".dc" -type "componentList" 0;
createNode deleteComponent -n "deleteComponent14";
	setAttr ".dc" -type "componentList" 1 "e[12:14]";
createNode deleteComponent -n "deleteComponent15";
	setAttr ".dc" -type "componentList" 1 "e[9:11]";
createNode lambert -n "desk";
createNode shadingEngine -n "lambert7SG";
	setAttr ".ihi" 0;
	setAttr -s 13 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 13 ".gn";
createNode materialInfo -n "materialInfo6";
createNode file -n "file6";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//desk1.png";
createNode place2dTexture -n "place2dTexture7";
createNode polyTweakUV -n "polyTweakUV6";
	setAttr ".uopa" yes;
	setAttr -s 14 ".uvtk[0:13]" -type "float2" 0.45413852 0.0045094565 0.45413852
		 -0.23319337 0.25421929 -0.11434197 0.49412239 -0.25157976 0.42304003 -0.25157976
		 0.35195759 -0.25157976 0.28087518 -0.25157976 0.49412239 -0.5817759 0.42304003 -0.5817759
		 0.35195759 -0.5817759 0.28087518 -0.5817759 0.45413852 -0.59933674 0.45413852 -0.83703959
		 0.25421929 -0.71818817;
createNode polyTweakUV -n "polyTweakUV7";
	setAttr ".uopa" yes;
	setAttr -s 12 ".uvtk";
	setAttr ".uvtk[0]" -type "float2" 0 0.34186068 ;
	setAttr ".uvtk[1]" -type "float2" 0 0.34186068 ;
	setAttr ".uvtk[2]" -type "float2" 0.0023385752 0.19643968 ;
	setAttr ".uvtk[3]" -type "float2" 0.0023385752 0.19643968 ;
	setAttr ".uvtk[4]" -type "float2" 0.0070156925 0.30401385 ;
	setAttr ".uvtk[5]" -type "float2" -0.0023386111 0.31804523 ;
	setAttr ".uvtk[6]" -type "float2" 0.0023385743 0.20111683 ;
	setAttr ".uvtk[7]" -type "float2" 0.0023385743 0.20111683 ;
	setAttr ".uvtk[10]" -type "float2" 0 0.34186068 ;
	setAttr ".uvtk[11]" -type "float2" -0.0023385398 0.20536421 ;
	setAttr ".uvtk[12]" -type "float2" 0 0.34186068 ;
	setAttr ".uvtk[13]" -type "float2" 0 0.20068707 ;
createNode polyMapCut -n "polyMapCut1";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 3 "e[0]" "e[3]" "e[8:11]";
createNode polyTweakUV -n "polyTweakUV8";
	setAttr ".uopa" yes;
	setAttr -s 6 ".uvtk";
	setAttr ".uvtk[6]" -type "float2" 0.0070157135 0.042094216 ;
	setAttr ".uvtk[7]" -type "float2" 0.0070156837 0.042094216 ;
	setAttr ".uvtk[8]" -type "float2" 0.0070157135 0.042094216 ;
	setAttr ".uvtk[9]" -type "float2" 0.0070156837 0.042094216 ;
	setAttr ".uvtk[14]" -type "float2" 0.0070156837 0.042094216 ;
	setAttr ".uvtk[15]" -type "float2" 0.0070157135 0.042094216 ;
createNode polyMapCut -n "polyMapCut2";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 3 "e[0]" "e[3]" "e[10:11]";
createNode polyTweakUV -n "polyTweakUV9";
	setAttr ".uopa" yes;
	setAttr -s 4 ".uvtk";
	setAttr ".uvtk[6]" -type "float2" 0.53699183 -0.61163986 ;
	setAttr ".uvtk[8]" -type "float2" 0.5880996 -0.65827382 ;
	setAttr ".uvtk[9]" -type "float2" 0.34698686 -0.79617792 ;
	setAttr ".uvtk[14]" -type "float2" 0.29587904 -0.74954402 ;
createNode polyMapCut -n "polyMapCut3";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 2 "e[2:3]" "e[8:9]";
createNode polyTweakUV -n "polyTweakUV10";
	setAttr ".uopa" yes;
	setAttr -s 4 ".uvtk";
	setAttr ".uvtk[4]" -type "float2" -0.32694626 0.037740588 ;
	setAttr ".uvtk[7]" -type "float2" 0.30356055 -0.037740529 ;
	setAttr ".uvtk[15]" -type "float2" -0.32110286 -0.037740529 ;
	setAttr ".uvtk[16]" -type "float2" 0.27434403 0.032142699 ;
createNode polyMapCut -n "polyMapCut4";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 2 "e[1:2]" "e[6:7]";
createNode polyTweakUV -n "polyTweakUV11";
	setAttr ".uopa" yes;
	setAttr -s 12 ".uvtk";
	setAttr ".uvtk[0]" -type "float2" -0.32260877 0.51682353 ;
	setAttr ".uvtk[1]" -type "float2" 0.28051454 0.51682353 ;
	setAttr ".uvtk[2]" -type "float2" -0.34787291 0 ;
	setAttr ".uvtk[3]" -type "float2" 0.28615636 0.51682353 ;
	setAttr ".uvtk[5]" -type "float2" 0.33485645 0 ;
	setAttr ".uvtk[10]" -type "float2" 0.88363791 0.51682353 ;
	setAttr ".uvtk[11]" -type "float2" 0.87799621 0.51682353 ;
	setAttr ".uvtk[12]" -type "float2" -0.92573214 0.51682353 ;
	setAttr ".uvtk[13]" -type "float2" -0.92573214 0.51682353 ;
	setAttr ".uvtk[17]" -type "float2" -0.33485657 0 ;
	setAttr ".uvtk[18]" -type "float2" 0.34787291 0 ;
	setAttr ".uvtk[19]" -type "float2" -0.31696695 0.51682353 ;
createNode polyUnite -n "polyUnite3";
	setAttr -s 6 ".ip";
	setAttr -s 6 ".im";
createNode groupId -n "groupId14";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts9";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:5]";
createNode groupId -n "groupId15";
	setAttr ".ihi" 0;
createNode groupId -n "groupId16";
	setAttr ".ihi" 0;
createNode groupId -n "groupId17";
	setAttr ".ihi" 0;
createNode groupId -n "groupId18";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts10";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:4]";
createNode groupId -n "groupId19";
	setAttr ".ihi" 0;
createNode groupId -n "groupId20";
	setAttr ".ihi" 0;
createNode groupId -n "groupId21";
	setAttr ".ihi" 0;
createNode groupId -n "groupId22";
	setAttr ".ihi" 0;
createNode groupId -n "groupId23";
	setAttr ".ihi" 0;
createNode groupId -n "groupId24";
	setAttr ".ihi" 0;
createNode groupId -n "groupId25";
	setAttr ".ihi" 0;
createNode groupId -n "groupId26";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts11";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:31]";
createNode polyMapCut -n "polyMapCut5";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 3 "e[12]" "e[15]" "e[22:23]";
createNode polyTweakUV -n "polyTweakUV12";
	setAttr ".uopa" yes;
	setAttr -s 5 ".uvtk";
	setAttr ".uvtk[27]" -type "float2" 0.7195484 0.11773558 ;
	setAttr ".uvtk[28]" -type "float2" 0.4590897 -0.64625198 ;
	setAttr ".uvtk[29]" -type "float2" 0.67018282 -0.64625198 ;
	setAttr ".uvtk[90]" -type "float2" 0.50845522 0.11773558 ;
createNode polyTriangulate -n "polyTriangulate6";
	setAttr ".ics" -type "componentList" 1 "f[*]";
createNode polyTriangulate -n "polyTriangulate7";
	setAttr ".ics" -type "componentList" 1 "f[*]";
createNode polyTweak -n "polyTweak10";
	setAttr ".uopa" yes;
	setAttr -s 32 ".tk";
	setAttr ".tk[0]" -type "float3" 0.0057624183 0 23.137802 ;
	setAttr ".tk[1]" -type "float3" 0.0057624183 0 23.137802 ;
	setAttr ".tk[2]" -type "float3" 0.0057624183 0 23.137802 ;
	setAttr ".tk[3]" -type "float3" 0.0057624183 0 23.137802 ;
	setAttr ".tk[4]" -type "float3" -0.0059560766 0 -23.915401 ;
	setAttr ".tk[5]" -type "float3" -0.0059560766 0 -23.915401 ;
	setAttr ".tk[6]" -type "float3" -0.0059560766 0 -23.915401 ;
	setAttr ".tk[7]" -type "float3" -0.0059560766 0 -23.915401 ;
	setAttr ".tk[8]" -type "float3" 0.0057624183 0 23.137802 ;
	setAttr ".tk[9]" -type "float3" 0.0057624183 0 23.137802 ;
	setAttr ".tk[10]" -type "float3" 0.0057624183 0 23.137802 ;
	setAttr ".tk[11]" -type "float3" 0.0057624183 0 23.137802 ;
	setAttr ".tk[12]" -type "float3" -0.0059560766 0 -23.915401 ;
	setAttr ".tk[13]" -type "float3" -0.0059560766 0 -23.915401 ;
	setAttr ".tk[14]" -type "float3" -0.0059560766 0 -23.915401 ;
	setAttr ".tk[15]" -type "float3" -0.0059560766 0 -23.915401 ;
	setAttr ".tk[18]" -type "float3" 0.0057624183 0 23.137802 ;
	setAttr ".tk[19]" -type "float3" 0.0057624183 0 23.137802 ;
	setAttr ".tk[22]" -type "float3" -0.0059560766 0 -23.915401 ;
	setAttr ".tk[23]" -type "float3" -0.0059560766 0 -23.915401 ;
	setAttr ".tk[24]" -type "float3" 0.0057624183 0 23.137802 ;
	setAttr ".tk[25]" -type "float3" 0.0057624183 0 23.137802 ;
	setAttr ".tk[26]" -type "float3" 0.0057624183 0 23.137802 ;
	setAttr ".tk[27]" -type "float3" 0.0057624183 0 23.137802 ;
	setAttr ".tk[28]" -type "float3" -0.0059560766 0 -23.915401 ;
	setAttr ".tk[29]" -type "float3" -0.0059560766 0 -23.915401 ;
	setAttr ".tk[30]" -type "float3" -0.0059560766 0 -23.915401 ;
	setAttr ".tk[31]" -type "float3" -0.0059560766 0 -23.915401 ;
	setAttr ".tk[32]" -type "float3" 0.0057624183 0 23.137802 ;
	setAttr ".tk[33]" -type "float3" 0.0057624183 0 23.137802 ;
	setAttr ".tk[34]" -type "float3" -0.0059560766 0 -23.915401 ;
	setAttr ".tk[35]" -type "float3" -0.0059560766 0 -23.915401 ;
select -ne :time1;
	setAttr ".o" 1;
	setAttr ".unw" 1;
select -ne :renderPartition;
	setAttr -s 10 ".st";
select -ne :renderGlobalsList1;
select -ne :defaultShaderList1;
	setAttr -s 10 ".s";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
	setAttr -s 9 ".u";
select -ne :defaultRenderingList1;
select -ne :defaultTextureList1;
	setAttr -s 9 ".tx";
select -ne :initialShadingGroup;
	setAttr ".ro" yes;
select -ne :initialParticleSE;
	setAttr ".ro" yes;
select -ne :defaultResolution;
	setAttr ".pa" 1;
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
select -ne :hardwareRenderingGlobals;
	setAttr ".otfna" -type "stringArray" 22 "NURBS Curves" "NURBS Surfaces" "Polygons" "Subdiv Surface" "Particles" "Particle Instance" "Fluids" "Strokes" "Image Planes" "UI" "Lights" "Cameras" "Locators" "Joints" "IK Handles" "Deformers" "Motion Trails" "Components" "Hair Systems" "Follicles" "Misc. UI" "Ornaments"  ;
	setAttr ".otfva" -type "Int32Array" 22 0 1 1 1 1 1
		 1 1 1 0 0 0 0 0 0 0 0 0
		 0 0 0 0 ;
select -ne :defaultHardwareRenderGlobals;
	setAttr ".res" -type "string" "ntsc_4d 646 485 1.333";
select -ne :ikSystem;
	setAttr -s 4 ".sol";
connectAttr "polyTriangulate1.out" "wallsShape.i";
connectAttr "polyTweakUV1.uvtk[0]" "wallsShape.uvst[0].uvtw";
connectAttr "polyNormal2.out" "skirting_and_door_frameShape.i";
connectAttr "polyTweakUV2.uvtk[0]" "skirting_and_door_frameShape.uvst[0].uvtw";
connectAttr "groupId1.id" "pCubeShape3.iog.og[0].gid";
connectAttr "lambert3SG.mwc" "pCubeShape3.iog.og[0].gco";
connectAttr "groupParts1.og" "pCubeShape3.i";
connectAttr "groupId2.id" "pCubeShape3.ciog.cog[0].cgid";
connectAttr "groupId3.id" "pCubeShape4.iog.og[0].gid";
connectAttr "lambert3SG.mwc" "pCubeShape4.iog.og[0].gco";
connectAttr "groupParts2.og" "pCubeShape4.i";
connectAttr "groupId4.id" "pCubeShape4.ciog.cog[0].cgid";
connectAttr "groupParts3.og" "window_frameShape.i";
connectAttr "groupId5.id" "window_frameShape.iog.og[0].gid";
connectAttr "lambert3SG.mwc" "window_frameShape.iog.og[0].gco";
connectAttr "groupParts4.og" "polySurfaceShape2.i";
connectAttr "groupId8.id" "polySurfaceShape2.iog.og[0].gid";
connectAttr "lambert4SG.mwc" "polySurfaceShape2.iog.og[0].gco";
connectAttr "polyTweakUV4.out" "polySurfaceShape3.i";
connectAttr "groupId9.id" "polySurfaceShape3.iog.og[0].gid";
connectAttr "lambert4SG.mwc" "polySurfaceShape3.iog.og[0].gco";
connectAttr "polyTweakUV4.uvtk[0]" "polySurfaceShape3.uvst[0].uvtw";
connectAttr "groupId6.id" "window_frame1Shape.iog.og[1].gid";
connectAttr "lambert4SG.mwc" "window_frame1Shape.iog.og[1].gco";
connectAttr "groupId7.id" "window_frame1Shape.ciog.cog[0].cgid";
connectAttr "groupId10.id" "pCubeShape5.iog.og[0].gid";
connectAttr "lambert5SG.mwc" "pCubeShape5.iog.og[0].gco";
connectAttr "groupParts6.og" "pCubeShape5.i";
connectAttr "polyTweakUV3.uvtk[0]" "pCubeShape5.uvst[0].uvtw";
connectAttr "groupId11.id" "pCubeShape5.ciog.cog[0].cgid";
connectAttr "polyTriangulate5.out" "pCubeShape6.i";
connectAttr "polyTweakUV5.uvtk[0]" "pCubeShape6.uvst[0].uvtw";
connectAttr "polyTriangulate7.out" "polySurface1Shape.i";
connectAttr "groupId12.id" "polySurface1Shape.iog.og[0].gid";
connectAttr "lambert4SG.mwc" "polySurface1Shape.iog.og[0].gco";
connectAttr "groupId13.id" "polySurface1Shape.iog.og[1].gid";
connectAttr "lambert5SG.mwc" "polySurface1Shape.iog.og[1].gco";
connectAttr "groupId14.id" "pCubeShape7.iog.og[1].gid";
connectAttr "lambert7SG.mwc" "pCubeShape7.iog.og[1].gco";
connectAttr "groupParts9.og" "pCubeShape7.i";
connectAttr "polyTweakUV11.uvtk[0]" "pCubeShape7.uvst[0].uvtw";
connectAttr "groupId15.id" "pCubeShape7.ciog.cog[1].cgid";
connectAttr "groupId16.id" "pCubeShape8.iog.og[1].gid";
connectAttr "lambert7SG.mwc" "pCubeShape8.iog.og[1].gco";
connectAttr "groupId17.id" "pCubeShape8.ciog.cog[1].cgid";
connectAttr "groupId18.id" "pCylinderShape1.iog.og[1].gid";
connectAttr "lambert7SG.mwc" "pCylinderShape1.iog.og[1].gco";
connectAttr "groupParts10.og" "pCylinderShape1.i";
connectAttr "polyTweakUV6.uvtk[0]" "pCylinderShape1.uvst[0].uvtw";
connectAttr "groupId19.id" "pCylinderShape1.ciog.cog[1].cgid";
connectAttr "groupId20.id" "pCylinderShape2.iog.og[1].gid";
connectAttr "lambert7SG.mwc" "pCylinderShape2.iog.og[1].gco";
connectAttr "groupId21.id" "pCylinderShape2.ciog.cog[1].cgid";
connectAttr "groupId22.id" "pCylinderShape3.iog.og[1].gid";
connectAttr "lambert7SG.mwc" "pCylinderShape3.iog.og[1].gco";
connectAttr "groupId23.id" "pCylinderShape3.ciog.cog[1].cgid";
connectAttr "groupId24.id" "pCylinderShape4.iog.og[1].gid";
connectAttr "lambert7SG.mwc" "pCylinderShape4.iog.og[1].gco";
connectAttr "groupId25.id" "pCylinderShape4.ciog.cog[1].cgid";
connectAttr "polyTriangulate6.out" "pCube9Shape.i";
connectAttr "groupId26.id" "pCube9Shape.iog.og[0].gid";
connectAttr "lambert7SG.mwc" "pCube9Shape.iog.og[0].gco";
connectAttr "polyTweakUV12.uvtk[0]" "pCube9Shape.uvst[0].uvtw";
connectAttr ":mentalrayGlobals.msg" ":mentalrayItemsList.glb";
connectAttr ":miDefaultOptions.msg" ":mentalrayItemsList.opt" -na;
connectAttr ":miDefaultFramebuffer.msg" ":mentalrayItemsList.fb" -na;
connectAttr ":miDefaultOptions.msg" ":mentalrayGlobals.opt";
connectAttr ":miDefaultFramebuffer.msg" ":mentalrayGlobals.fb";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "pz_fence:pz_fence.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "cube_0_5:lambert2SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "lambert2SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "lambert3SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "lambert4SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "lambert5SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "lambert6SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "lambert7SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "pz_fence:pz_fence.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "cube_0_5:lambert2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "lambert2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "lambert3SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "lambert4SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "lambert5SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "lambert6SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "lambert7SG.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr "pz_fence:pz_fence1.oc" "pz_fence:pz_fence.ss";
connectAttr "pz_fence:pz_fence.msg" "pz_fence:materialInfo1.sg";
connectAttr "pz_fence:pz_fence1.msg" "pz_fence:materialInfo1.m";
connectAttr "pz_fence:pz_fence1F.msg" "pz_fence:materialInfo1.t" -na;
connectAttr "pz_fence:pz_fence1F.oc" "pz_fence:pz_fence1.c";
connectAttr "pz_fence:pz_fence1P2D.c" "pz_fence:pz_fence1F.c";
connectAttr "pz_fence:pz_fence1P2D.tf" "pz_fence:pz_fence1F.tf";
connectAttr "pz_fence:pz_fence1P2D.rf" "pz_fence:pz_fence1F.rf";
connectAttr "pz_fence:pz_fence1P2D.s" "pz_fence:pz_fence1F.s";
connectAttr "pz_fence:pz_fence1P2D.wu" "pz_fence:pz_fence1F.wu";
connectAttr "pz_fence:pz_fence1P2D.wv" "pz_fence:pz_fence1F.wv";
connectAttr "pz_fence:pz_fence1P2D.re" "pz_fence:pz_fence1F.re";
connectAttr "pz_fence:pz_fence1P2D.of" "pz_fence:pz_fence1F.of";
connectAttr "pz_fence:pz_fence1P2D.r" "pz_fence:pz_fence1F.ro";
connectAttr "pz_fence:pz_fence1P2D.o" "pz_fence:pz_fence1F.uv";
connectAttr "pz_fence:pz_fence1P2D.ofs" "pz_fence:pz_fence1F.fs";
connectAttr "cube_0_5:lambert2SG1.oc" "cube_0_5:lambert2SG.ss";
connectAttr "cube_0_5:lambert2SG.msg" "cube_0_5:materialInfo1.sg";
connectAttr "cube_0_5:lambert2SG1.msg" "cube_0_5:materialInfo1.m";
connectAttr "cube_0_5:lambert2SG1F.msg" "cube_0_5:materialInfo1.t" -na;
connectAttr "cube_0_5:lambert2SG1F.oc" "cube_0_5:lambert2SG1.c";
connectAttr "cube_0_5:lambert2SG1P2D.c" "cube_0_5:lambert2SG1F.c";
connectAttr "cube_0_5:lambert2SG1P2D.tf" "cube_0_5:lambert2SG1F.tf";
connectAttr "cube_0_5:lambert2SG1P2D.rf" "cube_0_5:lambert2SG1F.rf";
connectAttr "cube_0_5:lambert2SG1P2D.s" "cube_0_5:lambert2SG1F.s";
connectAttr "cube_0_5:lambert2SG1P2D.wu" "cube_0_5:lambert2SG1F.wu";
connectAttr "cube_0_5:lambert2SG1P2D.wv" "cube_0_5:lambert2SG1F.wv";
connectAttr "cube_0_5:lambert2SG1P2D.re" "cube_0_5:lambert2SG1F.re";
connectAttr "cube_0_5:lambert2SG1P2D.of" "cube_0_5:lambert2SG1F.of";
connectAttr "cube_0_5:lambert2SG1P2D.r" "cube_0_5:lambert2SG1F.ro";
connectAttr "cube_0_5:lambert2SG1P2D.o" "cube_0_5:lambert2SG1F.uv";
connectAttr "cube_0_5:lambert2SG1P2D.ofs" "cube_0_5:lambert2SG1F.fs";
connectAttr "polyCube1.out" "polyNormal1.ip";
connectAttr "polyNormal1.out" "deleteComponent1.ig";
connectAttr "deleteComponent1.og" "deleteComponent2.ig";
connectAttr "polyCube2.out" "polySplitRing1.ip";
connectAttr "skirting_and_door_frameShape.wm" "polySplitRing1.mp";
connectAttr "polySplitRing1.out" "polyExtrudeFace1.ip";
connectAttr "skirting_and_door_frameShape.wm" "polyExtrudeFace1.mp";
connectAttr "polyExtrudeFace1.out" "polySplitRing2.ip";
connectAttr "skirting_and_door_frameShape.wm" "polySplitRing2.mp";
connectAttr "polySplitRing2.out" "polyExtrudeEdge1.ip";
connectAttr "skirting_and_door_frameShape.wm" "polyExtrudeEdge1.mp";
connectAttr "polyExtrudeEdge1.out" "polyExtrudeFace2.ip";
connectAttr "skirting_and_door_frameShape.wm" "polyExtrudeFace2.mp";
connectAttr "polyExtrudeFace2.out" "polySplitRing3.ip";
connectAttr "skirting_and_door_frameShape.wm" "polySplitRing3.mp";
connectAttr "polySplitRing3.out" "polyExtrudeFace3.ip";
connectAttr "skirting_and_door_frameShape.wm" "polyExtrudeFace3.mp";
connectAttr "polyExtrudeFace3.out" "polyExtrudeFace4.ip";
connectAttr "skirting_and_door_frameShape.wm" "polyExtrudeFace4.mp";
connectAttr "polyExtrudeFace4.out" "polySplitRing4.ip";
connectAttr "skirting_and_door_frameShape.wm" "polySplitRing4.mp";
connectAttr "polySplitRing4.out" "polyExtrudeEdge2.ip";
connectAttr "skirting_and_door_frameShape.wm" "polyExtrudeEdge2.mp";
connectAttr "polyExtrudeEdge2.out" "polyExtrudeEdge3.ip";
connectAttr "skirting_and_door_frameShape.wm" "polyExtrudeEdge3.mp";
connectAttr "polyExtrudeEdge3.out" "polySplitRing5.ip";
connectAttr "skirting_and_door_frameShape.wm" "polySplitRing5.mp";
connectAttr "polyTweak1.out" "polyExtrudeFace5.ip";
connectAttr "skirting_and_door_frameShape.wm" "polyExtrudeFace5.mp";
connectAttr "polySplitRing5.out" "polyTweak1.ip";
connectAttr "polyExtrudeFace5.out" "polySplitRing6.ip";
connectAttr "skirting_and_door_frameShape.wm" "polySplitRing6.mp";
connectAttr "polyTweak2.out" "polyExtrudeFace6.ip";
connectAttr "skirting_and_door_frameShape.wm" "polyExtrudeFace6.mp";
connectAttr "polySplitRing6.out" "polyTweak2.ip";
connectAttr "deleteComponent2.og" "polySplitRing7.ip";
connectAttr "wallsShape.wm" "polySplitRing7.mp";
connectAttr "polySplitRing7.out" "polySplitRing8.ip";
connectAttr "wallsShape.wm" "polySplitRing8.mp";
connectAttr "polyTweak3.out" "polySplitRing9.ip";
connectAttr "wallsShape.wm" "polySplitRing9.mp";
connectAttr "polySplitRing8.out" "polyTweak3.ip";
connectAttr "polySurfaceShape1.o" "polySplitRing10.ip";
connectAttr "pCubeShape4.wm" "polySplitRing10.mp";
connectAttr "polySplitRing10.out" "polySplitRing11.ip";
connectAttr "pCubeShape4.wm" "polySplitRing11.mp";
connectAttr "polySplitRing11.out" "polyExtrudeFace7.ip";
connectAttr "pCubeShape4.wm" "polyExtrudeFace7.mp";
connectAttr "polyExtrudeFace7.out" "polyExtrudeFace8.ip";
connectAttr "pCubeShape4.wm" "polyExtrudeFace8.mp";
connectAttr "polySplitRing9.out" "polyTweak4.ip";
connectAttr "polyTweak4.out" "deleteComponent3.ig";
connectAttr "deleteComponent3.og" "polySplitRing12.ip";
connectAttr "wallsShape.wm" "polySplitRing12.mp";
connectAttr "polySplitRing12.out" "deleteComponent4.ig";
connectAttr "deleteComponent4.og" "deleteComponent5.ig";
connectAttr "deleteComponent5.og" "deleteComponent6.ig";
connectAttr "deleteComponent6.og" "deleteComponent7.ig";
connectAttr "file1.oc" "lambert2.c";
connectAttr "lambert2.oc" "lambert2SG.ss";
connectAttr "wallsShape.iog" "lambert2SG.dsm" -na;
connectAttr "lambert2SG.msg" "materialInfo1.sg";
connectAttr "lambert2.msg" "materialInfo1.m";
connectAttr "file1.msg" "materialInfo1.t" -na;
connectAttr "place2dTexture1.c" "file1.c";
connectAttr "place2dTexture1.tf" "file1.tf";
connectAttr "place2dTexture1.rf" "file1.rf";
connectAttr "place2dTexture1.mu" "file1.mu";
connectAttr "place2dTexture1.mv" "file1.mv";
connectAttr "place2dTexture1.s" "file1.s";
connectAttr "place2dTexture1.wu" "file1.wu";
connectAttr "place2dTexture1.wv" "file1.wv";
connectAttr "place2dTexture1.re" "file1.re";
connectAttr "place2dTexture1.of" "file1.of";
connectAttr "place2dTexture1.r" "file1.ro";
connectAttr "place2dTexture1.n" "file1.n";
connectAttr "place2dTexture1.vt1" "file1.vt1";
connectAttr "place2dTexture1.vt2" "file1.vt2";
connectAttr "place2dTexture1.vt3" "file1.vt3";
connectAttr "place2dTexture1.vc1" "file1.vc1";
connectAttr "place2dTexture1.o" "file1.uv";
connectAttr "place2dTexture1.ofs" "file1.fs";
connectAttr "deleteComponent7.og" "polyTweak5.ip";
connectAttr "polyTweak5.out" "deleteComponent8.ig";
connectAttr "deleteComponent8.og" "deleteComponent9.ig";
connectAttr "deleteComponent9.og" "deleteComponent10.ig";
connectAttr "deleteComponent10.og" "polyTweakUV1.ip";
connectAttr "file2.oc" "white_wood.c";
connectAttr "white_wood.oc" "lambert3SG.ss";
connectAttr "skirting_and_door_frameShape.iog" "lambert3SG.dsm" -na;
connectAttr "pCubeShape3.iog.og[0]" "lambert3SG.dsm" -na;
connectAttr "pCubeShape3.ciog.cog[0]" "lambert3SG.dsm" -na;
connectAttr "pCubeShape4.iog.og[0]" "lambert3SG.dsm" -na;
connectAttr "pCubeShape4.ciog.cog[0]" "lambert3SG.dsm" -na;
connectAttr "window_frameShape.iog.og[0]" "lambert3SG.dsm" -na;
connectAttr "groupId1.msg" "lambert3SG.gn" -na;
connectAttr "groupId2.msg" "lambert3SG.gn" -na;
connectAttr "groupId3.msg" "lambert3SG.gn" -na;
connectAttr "groupId4.msg" "lambert3SG.gn" -na;
connectAttr "groupId5.msg" "lambert3SG.gn" -na;
connectAttr "lambert3SG.msg" "materialInfo2.sg";
connectAttr "white_wood.msg" "materialInfo2.m";
connectAttr "file2.msg" "materialInfo2.t" -na;
connectAttr "place2dTexture2.c" "file2.c";
connectAttr "place2dTexture2.tf" "file2.tf";
connectAttr "place2dTexture2.rf" "file2.rf";
connectAttr "place2dTexture2.mu" "file2.mu";
connectAttr "place2dTexture2.mv" "file2.mv";
connectAttr "place2dTexture2.s" "file2.s";
connectAttr "place2dTexture2.wu" "file2.wu";
connectAttr "place2dTexture2.wv" "file2.wv";
connectAttr "place2dTexture2.re" "file2.re";
connectAttr "place2dTexture2.of" "file2.of";
connectAttr "place2dTexture2.r" "file2.ro";
connectAttr "place2dTexture2.n" "file2.n";
connectAttr "place2dTexture2.vt1" "file2.vt1";
connectAttr "place2dTexture2.vt2" "file2.vt2";
connectAttr "place2dTexture2.vt3" "file2.vt3";
connectAttr "place2dTexture2.vc1" "file2.vc1";
connectAttr "place2dTexture2.o" "file2.uv";
connectAttr "place2dTexture2.ofs" "file2.fs";
connectAttr "polyExtrudeFace6.out" "polyTweakUV2.ip";
connectAttr "polyTweakUV1.out" "polyTriangulate1.ip";
connectAttr "polyTweak6.out" "polyTriangulate2.ip";
connectAttr "polyTweakUV2.out" "polyTweak6.ip";
connectAttr "polyTweak7.out" "polyTriangulate3.ip";
connectAttr "polyCube3.out" "polyTweak7.ip";
connectAttr "polyTweak8.out" "polyTriangulate4.ip";
connectAttr "polyExtrudeFace8.out" "polyTweak8.ip";
connectAttr "pCubeShape3.o" "polyUnite1.ip[0]";
connectAttr "pCubeShape4.o" "polyUnite1.ip[1]";
connectAttr "pCubeShape3.wm" "polyUnite1.im[0]";
connectAttr "pCubeShape4.wm" "polyUnite1.im[1]";
connectAttr "polyTriangulate3.out" "groupParts1.ig";
connectAttr "groupId1.id" "groupParts1.gi";
connectAttr "polyTriangulate4.out" "groupParts2.ig";
connectAttr "groupId3.id" "groupParts2.gi";
connectAttr "polyUnite1.out" "groupParts3.ig";
connectAttr "groupId5.id" "groupParts3.gi";
connectAttr "polyTriangulate2.out" "polyNormal2.ip";
connectAttr "file4.oc" "wood.c";
connectAttr "wood.oc" "lambert4SG.ss";
connectAttr "window_frame1Shape.iog.og[1]" "lambert4SG.dsm" -na;
connectAttr "window_frame1Shape.ciog.cog[0]" "lambert4SG.dsm" -na;
connectAttr "polySurfaceShape2.iog.og[0]" "lambert4SG.dsm" -na;
connectAttr "polySurfaceShape3.iog.og[0]" "lambert4SG.dsm" -na;
connectAttr "polySurface1Shape.iog.og[0]" "lambert4SG.dsm" -na;
connectAttr "groupId6.msg" "lambert4SG.gn" -na;
connectAttr "groupId7.msg" "lambert4SG.gn" -na;
connectAttr "groupId8.msg" "lambert4SG.gn" -na;
connectAttr "groupId9.msg" "lambert4SG.gn" -na;
connectAttr "groupId12.msg" "lambert4SG.gn" -na;
connectAttr "lambert4SG.msg" "materialInfo3.sg";
connectAttr "wood.msg" "materialInfo3.m";
connectAttr "file4.msg" "materialInfo3.t" -na;
connectAttr "place2dTexture3.c" "file3.c";
connectAttr "place2dTexture3.tf" "file3.tf";
connectAttr "place2dTexture3.rf" "file3.rf";
connectAttr "place2dTexture3.mu" "file3.mu";
connectAttr "place2dTexture3.mv" "file3.mv";
connectAttr "place2dTexture3.s" "file3.s";
connectAttr "place2dTexture3.wu" "file3.wu";
connectAttr "place2dTexture3.wv" "file3.wv";
connectAttr "place2dTexture3.re" "file3.re";
connectAttr "place2dTexture3.of" "file3.of";
connectAttr "place2dTexture3.r" "file3.ro";
connectAttr "place2dTexture3.n" "file3.n";
connectAttr "place2dTexture3.vt1" "file3.vt1";
connectAttr "place2dTexture3.vt2" "file3.vt2";
connectAttr "place2dTexture3.vt3" "file3.vt3";
connectAttr "place2dTexture3.vc1" "file3.vc1";
connectAttr "place2dTexture3.o" "file3.uv";
connectAttr "place2dTexture3.ofs" "file3.fs";
connectAttr "place2dTexture4.c" "file4.c";
connectAttr "place2dTexture4.tf" "file4.tf";
connectAttr "place2dTexture4.rf" "file4.rf";
connectAttr "place2dTexture4.mu" "file4.mu";
connectAttr "place2dTexture4.mv" "file4.mv";
connectAttr "place2dTexture4.s" "file4.s";
connectAttr "place2dTexture4.wu" "file4.wu";
connectAttr "place2dTexture4.wv" "file4.wv";
connectAttr "place2dTexture4.re" "file4.re";
connectAttr "place2dTexture4.of" "file4.of";
connectAttr "place2dTexture4.r" "file4.ro";
connectAttr "place2dTexture4.n" "file4.n";
connectAttr "place2dTexture4.vt1" "file4.vt1";
connectAttr "place2dTexture4.vt2" "file4.vt2";
connectAttr "place2dTexture4.vt3" "file4.vt3";
connectAttr "place2dTexture4.vc1" "file4.vc1";
connectAttr "place2dTexture4.o" "file4.uv";
connectAttr "place2dTexture4.ofs" "file4.fs";
connectAttr "window_frame1Shape.o" "polySeparate1.ip";
connectAttr "polySeparate1.out[0]" "groupParts4.ig";
connectAttr "groupId8.id" "groupParts4.gi";
connectAttr "polySeparate1.out[1]" "groupParts5.ig";
connectAttr "groupId9.id" "groupParts5.gi";
connectAttr "polyCube4.out" "deleteComponent11.ig";
connectAttr "blackboard.oc" "lambert5.c";
connectAttr "lambert5.oc" "lambert5SG.ss";
connectAttr "pCubeShape5.iog.og[0]" "lambert5SG.dsm" -na;
connectAttr "pCubeShape5.ciog.cog[0]" "lambert5SG.dsm" -na;
connectAttr "polySurface1Shape.iog.og[1]" "lambert5SG.dsm" -na;
connectAttr "groupId10.msg" "lambert5SG.gn" -na;
connectAttr "groupId11.msg" "lambert5SG.gn" -na;
connectAttr "groupId13.msg" "lambert5SG.gn" -na;
connectAttr "lambert5SG.msg" "materialInfo4.sg";
connectAttr "lambert5.msg" "materialInfo4.m";
connectAttr "blackboard.msg" "materialInfo4.t" -na;
connectAttr "place2dTexture5.c" "blackboard.c";
connectAttr "place2dTexture5.tf" "blackboard.tf";
connectAttr "place2dTexture5.rf" "blackboard.rf";
connectAttr "place2dTexture5.mu" "blackboard.mu";
connectAttr "place2dTexture5.mv" "blackboard.mv";
connectAttr "place2dTexture5.s" "blackboard.s";
connectAttr "place2dTexture5.wu" "blackboard.wu";
connectAttr "place2dTexture5.wv" "blackboard.wv";
connectAttr "place2dTexture5.re" "blackboard.re";
connectAttr "place2dTexture5.of" "blackboard.of";
connectAttr "place2dTexture5.r" "blackboard.ro";
connectAttr "place2dTexture5.n" "blackboard.n";
connectAttr "place2dTexture5.vt1" "blackboard.vt1";
connectAttr "place2dTexture5.vt2" "blackboard.vt2";
connectAttr "place2dTexture5.vt3" "blackboard.vt3";
connectAttr "place2dTexture5.vc1" "blackboard.vc1";
connectAttr "place2dTexture5.o" "blackboard.uv";
connectAttr "place2dTexture5.ofs" "blackboard.fs";
connectAttr "deleteComponent11.og" "polyTweakUV3.ip";
connectAttr "groupParts5.og" "polyTweakUV4.ip";
connectAttr "polyCube5.out" "deleteComponent12.ig";
connectAttr "file5.oc" "lambert6.c";
connectAttr "lambert6.oc" "lambert6SG.ss";
connectAttr "pCubeShape6.iog" "lambert6SG.dsm" -na;
connectAttr "lambert6SG.msg" "materialInfo5.sg";
connectAttr "lambert6.msg" "materialInfo5.m";
connectAttr "file5.msg" "materialInfo5.t" -na;
connectAttr "place2dTexture6.c" "file5.c";
connectAttr "place2dTexture6.tf" "file5.tf";
connectAttr "place2dTexture6.rf" "file5.rf";
connectAttr "place2dTexture6.mu" "file5.mu";
connectAttr "place2dTexture6.mv" "file5.mv";
connectAttr "place2dTexture6.s" "file5.s";
connectAttr "place2dTexture6.wu" "file5.wu";
connectAttr "place2dTexture6.wv" "file5.wv";
connectAttr "place2dTexture6.re" "file5.re";
connectAttr "place2dTexture6.of" "file5.of";
connectAttr "place2dTexture6.r" "file5.ro";
connectAttr "place2dTexture6.n" "file5.n";
connectAttr "place2dTexture6.vt1" "file5.vt1";
connectAttr "place2dTexture6.vt2" "file5.vt2";
connectAttr "place2dTexture6.vt3" "file5.vt3";
connectAttr "place2dTexture6.vc1" "file5.vc1";
connectAttr "place2dTexture6.o" "file5.uv";
connectAttr "place2dTexture6.ofs" "file5.fs";
connectAttr "deleteComponent12.og" "polyTweakUV5.ip";
connectAttr "polySurfaceShape2.o" "polyUnite2.ip[0]";
connectAttr "polySurfaceShape3.o" "polyUnite2.ip[1]";
connectAttr "pCubeShape5.o" "polyUnite2.ip[2]";
connectAttr "polySurfaceShape2.wm" "polyUnite2.im[0]";
connectAttr "polySurfaceShape3.wm" "polyUnite2.im[1]";
connectAttr "pCubeShape5.wm" "polyUnite2.im[2]";
connectAttr "polyTweakUV3.out" "groupParts6.ig";
connectAttr "groupId10.id" "groupParts6.gi";
connectAttr "polyUnite2.out" "groupParts7.ig";
connectAttr "groupId12.id" "groupParts7.gi";
connectAttr "groupParts7.og" "groupParts8.ig";
connectAttr "groupId13.id" "groupParts8.gi";
connectAttr "polyTweak9.out" "polyTriangulate5.ip";
connectAttr "polyTweakUV5.out" "polyTweak9.ip";
connectAttr "polyCylinder1.out" "deleteComponent13.ig";
connectAttr "deleteComponent13.og" "deleteComponent14.ig";
connectAttr "deleteComponent14.og" "deleteComponent15.ig";
connectAttr "file6.oc" "desk.c";
connectAttr "desk.oc" "lambert7SG.ss";
connectAttr "pCubeShape7.iog.og[1]" "lambert7SG.dsm" -na;
connectAttr "pCubeShape7.ciog.cog[1]" "lambert7SG.dsm" -na;
connectAttr "pCubeShape8.iog.og[1]" "lambert7SG.dsm" -na;
connectAttr "pCubeShape8.ciog.cog[1]" "lambert7SG.dsm" -na;
connectAttr "pCylinderShape1.iog.og[1]" "lambert7SG.dsm" -na;
connectAttr "pCylinderShape1.ciog.cog[1]" "lambert7SG.dsm" -na;
connectAttr "pCylinderShape2.iog.og[1]" "lambert7SG.dsm" -na;
connectAttr "pCylinderShape2.ciog.cog[1]" "lambert7SG.dsm" -na;
connectAttr "pCylinderShape3.iog.og[1]" "lambert7SG.dsm" -na;
connectAttr "pCylinderShape3.ciog.cog[1]" "lambert7SG.dsm" -na;
connectAttr "pCylinderShape4.iog.og[1]" "lambert7SG.dsm" -na;
connectAttr "pCylinderShape4.ciog.cog[1]" "lambert7SG.dsm" -na;
connectAttr "pCube9Shape.iog.og[0]" "lambert7SG.dsm" -na;
connectAttr "groupId14.msg" "lambert7SG.gn" -na;
connectAttr "groupId15.msg" "lambert7SG.gn" -na;
connectAttr "groupId16.msg" "lambert7SG.gn" -na;
connectAttr "groupId17.msg" "lambert7SG.gn" -na;
connectAttr "groupId18.msg" "lambert7SG.gn" -na;
connectAttr "groupId19.msg" "lambert7SG.gn" -na;
connectAttr "groupId20.msg" "lambert7SG.gn" -na;
connectAttr "groupId21.msg" "lambert7SG.gn" -na;
connectAttr "groupId22.msg" "lambert7SG.gn" -na;
connectAttr "groupId23.msg" "lambert7SG.gn" -na;
connectAttr "groupId24.msg" "lambert7SG.gn" -na;
connectAttr "groupId25.msg" "lambert7SG.gn" -na;
connectAttr "groupId26.msg" "lambert7SG.gn" -na;
connectAttr "lambert7SG.msg" "materialInfo6.sg";
connectAttr "desk.msg" "materialInfo6.m";
connectAttr "file6.msg" "materialInfo6.t" -na;
connectAttr "place2dTexture7.c" "file6.c";
connectAttr "place2dTexture7.tf" "file6.tf";
connectAttr "place2dTexture7.rf" "file6.rf";
connectAttr "place2dTexture7.mu" "file6.mu";
connectAttr "place2dTexture7.mv" "file6.mv";
connectAttr "place2dTexture7.s" "file6.s";
connectAttr "place2dTexture7.wu" "file6.wu";
connectAttr "place2dTexture7.wv" "file6.wv";
connectAttr "place2dTexture7.re" "file6.re";
connectAttr "place2dTexture7.of" "file6.of";
connectAttr "place2dTexture7.r" "file6.ro";
connectAttr "place2dTexture7.n" "file6.n";
connectAttr "place2dTexture7.vt1" "file6.vt1";
connectAttr "place2dTexture7.vt2" "file6.vt2";
connectAttr "place2dTexture7.vt3" "file6.vt3";
connectAttr "place2dTexture7.vc1" "file6.vc1";
connectAttr "place2dTexture7.o" "file6.uv";
connectAttr "place2dTexture7.ofs" "file6.fs";
connectAttr "deleteComponent15.og" "polyTweakUV6.ip";
connectAttr "polyCube6.out" "polyTweakUV7.ip";
connectAttr "polyTweakUV7.out" "polyMapCut1.ip";
connectAttr "polyMapCut1.out" "polyTweakUV8.ip";
connectAttr "polyTweakUV8.out" "polyMapCut2.ip";
connectAttr "polyMapCut2.out" "polyTweakUV9.ip";
connectAttr "polyTweakUV9.out" "polyMapCut3.ip";
connectAttr "polyMapCut3.out" "polyTweakUV10.ip";
connectAttr "polyTweakUV10.out" "polyMapCut4.ip";
connectAttr "polyMapCut4.out" "polyTweakUV11.ip";
connectAttr "pCubeShape7.o" "polyUnite3.ip[0]";
connectAttr "pCubeShape8.o" "polyUnite3.ip[1]";
connectAttr "pCylinderShape1.o" "polyUnite3.ip[2]";
connectAttr "pCylinderShape2.o" "polyUnite3.ip[3]";
connectAttr "pCylinderShape3.o" "polyUnite3.ip[4]";
connectAttr "pCylinderShape4.o" "polyUnite3.ip[5]";
connectAttr "pCubeShape7.wm" "polyUnite3.im[0]";
connectAttr "pCubeShape8.wm" "polyUnite3.im[1]";
connectAttr "pCylinderShape1.wm" "polyUnite3.im[2]";
connectAttr "pCylinderShape2.wm" "polyUnite3.im[3]";
connectAttr "pCylinderShape3.wm" "polyUnite3.im[4]";
connectAttr "pCylinderShape4.wm" "polyUnite3.im[5]";
connectAttr "polyTweakUV11.out" "groupParts9.ig";
connectAttr "groupId14.id" "groupParts9.gi";
connectAttr "polyTweakUV6.out" "groupParts10.ig";
connectAttr "groupId18.id" "groupParts10.gi";
connectAttr "polyUnite3.out" "groupParts11.ig";
connectAttr "groupId26.id" "groupParts11.gi";
connectAttr "groupParts11.og" "polyMapCut5.ip";
connectAttr "polyMapCut5.out" "polyTweakUV12.ip";
connectAttr "polyTweakUV12.out" "polyTriangulate6.ip";
connectAttr "polyTweak10.out" "polyTriangulate7.ip";
connectAttr "groupParts8.og" "polyTweak10.ip";
connectAttr "pz_fence:pz_fence.pa" ":renderPartition.st" -na;
connectAttr "cube_0_5:lambert2SG.pa" ":renderPartition.st" -na;
connectAttr "lambert2SG.pa" ":renderPartition.st" -na;
connectAttr "lambert3SG.pa" ":renderPartition.st" -na;
connectAttr "lambert4SG.pa" ":renderPartition.st" -na;
connectAttr "lambert5SG.pa" ":renderPartition.st" -na;
connectAttr "lambert6SG.pa" ":renderPartition.st" -na;
connectAttr "lambert7SG.pa" ":renderPartition.st" -na;
connectAttr "pz_fence:pz_fence1.msg" ":defaultShaderList1.s" -na;
connectAttr "cube_0_5:lambert2SG1.msg" ":defaultShaderList1.s" -na;
connectAttr "lambert2.msg" ":defaultShaderList1.s" -na;
connectAttr "white_wood.msg" ":defaultShaderList1.s" -na;
connectAttr "wood.msg" ":defaultShaderList1.s" -na;
connectAttr "lambert5.msg" ":defaultShaderList1.s" -na;
connectAttr "lambert6.msg" ":defaultShaderList1.s" -na;
connectAttr "desk.msg" ":defaultShaderList1.s" -na;
connectAttr "pz_fence:pz_fence1P2D.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "cube_0_5:lambert2SG1P2D.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture2.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture3.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture4.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture5.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture6.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture7.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "pz_fence:pz_fence1F.msg" ":defaultTextureList1.tx" -na;
connectAttr "cube_0_5:lambert2SG1F.msg" ":defaultTextureList1.tx" -na;
connectAttr "file1.msg" ":defaultTextureList1.tx" -na;
connectAttr "file2.msg" ":defaultTextureList1.tx" -na;
connectAttr "file3.msg" ":defaultTextureList1.tx" -na;
connectAttr "file4.msg" ":defaultTextureList1.tx" -na;
connectAttr "blackboard.msg" ":defaultTextureList1.tx" -na;
connectAttr "file5.msg" ":defaultTextureList1.tx" -na;
connectAttr "file6.msg" ":defaultTextureList1.tx" -na;
dataStructure -fmt "raw" -as "name=externalContentTable:string=node:string=key:string=upath:uint32=upathcrc:string=rpath:string=roles";
applyMetadata -fmt "raw" -v "channel\nname externalContentTable\nstream\nname v1.0\nindexType numeric\nstructure externalContentTable\n0\n\"pz_fence:pz_fence1F\" \"fileTextureName\" \"pz-fence.png\" 1984701315 \"C:/Users/Dad/projects/github/music-theory/Assets/obj/pz-fence.png\" \"sourceImages\"\n1\n\"cube_0_5:lambert2SG1F\" \"fileTextureName\" \"plat_wood.png\" 688141114 \"\" \"sourceImages\"\n2\n\"file1\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/wallpaper1.png\" 4017085073 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/wallpaper1.png\" \"sourceImages\"\n3\n\"file2\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/white-wood.png\" 4232265713 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/white-wood.png\" \"sourceImages\"\n4\n\"file3\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/wood1.png\" 1800306167 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/wood1.png\" \"sourceImages\"\n5\n\"file4\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/wood2.png\" 753796903 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/wood2.png\" \"sourceImages\"\n6\n\"blackboard\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/blackboard.png\" 14752819 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/blackboard.png\" \"sourceImages\"\n7\n\"file5\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/floor1.png\" 1448379758 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/floor1.png\" \"sourceImages\"\n8\n\"file6\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/desk1.png\" 1055611876 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/desk1.png\" \"sourceImages\"\nendStream\nendChannel\nendAssociations\n" 
		-scn;
// End of blackboard-wide.ma
