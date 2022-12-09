//Maya ASCII 2015 scene
//Name: picture-frame3.ma
//Last modified: Fri, Jul 28, 2017 11:08:07 PM
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
	setAttr ".t" -type "double3" 43.514838887707491 9.3618527739565991 48.751599422765473 ;
	setAttr ".r" -type "double3" -2.1383527363572501 -681.39999999991517 0 ;
createNode camera -s -n "perspShape" -p "persp";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999993;
	setAttr ".coi" 61.229339998636327;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".tp" -type "double3" 3.6113298984862467 61.171016544573263 65.953903135049742 ;
	setAttr ".hc" -type "string" "viewSet -p %camera";
createNode transform -s -n "top";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 11.684782913580495 100.82229875850864 -0.14267557647490284 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 27.757437712801043;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
createNode transform -s -n "front";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -1.5519578004983972 2.1058958690264546 100.14907218025749 ;
createNode camera -s -n "frontShape" -p "front";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 45.383514274168618;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
createNode transform -s -n "side";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 100.1 -1.7185460366135104 2.2245016176879204 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 49.741196069224188;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
createNode transform -n "pCube1";
	setAttr ".t" -type "double3" 0 5.93343044784155 0 ;
createNode transform -n "transform28" -p "pCube1";
	setAttr ".v" no;
createNode mesh -n "pCubeShape1" -p "transform28";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.36453795433044434 0.15624409914016724 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "pCylinder1";
	setAttr ".t" -type "double3" 2.7059602210314595 3.5128703083879 3.0934706775535474 ;
	setAttr ".r" -type "double3" 90 0 0 ;
	setAttr ".s" -type "double3" 0.54968368763934716 0.10823069456682666 0.65159827125284098 ;
	setAttr ".rp" -type "double3" 0 -7.6353304041940954e-014 -1.5987211554602254e-014 ;
	setAttr ".rpt" -type "double3" 0 8.9675980337442832e-014 -5.2215581642880684e-014 ;
	setAttr ".spt" -type "double3" 0 -7.638334409421077e-014 -1.5987211554602254e-014 ;
createNode transform -n "transform27" -p "pCylinder1";
	setAttr ".v" no;
createNode mesh -n "pCylinderShape1" -p "transform27";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.099662844287881691 0.091053310474787041 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "pCube2";
	setAttr ".t" -type "double3" 2.6459281617788157 3.3178599443624859 0 ;
createNode transform -n "transform26" -p "pCube2";
	setAttr ".v" no;
createNode mesh -n "pCubeShape2" -p "transform26";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.49999992549419403 0.5 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "pCylinder2";
	setAttr ".t" -type "double3" 2.7059602210314595 3.5128703083878996 -3.1121648130642559 ;
	setAttr ".r" -type "double3" 90 0 0 ;
	setAttr ".s" -type "double3" 0.54968368763934716 0.10823069456682666 0.65159827125284098 ;
	setAttr ".rp" -type "double3" 0 -7.6353304041940954e-014 -1.5987211554602254e-014 ;
	setAttr ".rpt" -type "double3" 0 8.9675980337442832e-014 -5.2215581642880684e-014 ;
	setAttr ".spt" -type "double3" 0 -7.638334409421077e-014 -1.5987211554602254e-014 ;
createNode mesh -n "polySurfaceShape10" -p "pCylinder2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.50268697738647461 0.82648634910583496 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 72 ".uvst[0].uvsp[0:71]" -type "float2" 0.65549755 0.14093488
		 0.65324765 0.1257672 0.64952189 0.11089309 0.64435613 0.096455783 0.63780016 0.082594335
		 0.62991709 0.069442227 0.62078285 0.05712612 0.61048543 0.045764633 0.59912395 0.035467185
		 0.58680785 0.026332945 0.57365578 0.018449858 0.55979431 0.011893883 0.54535699 0.0067281425
		 0.53048289 0.0030023456 0.51531523 0.00075243413 0.50000006 4.4703484e-008 0.65625
		 0.15625 0.375 0.3125 0.37890625 0.3125 0.3828125 0.3125 0.38671875 0.3125 0.390625
		 0.3125 0.39453125 0.3125 0.3984375 0.3125 0.40234375 0.3125 0.40625 0.3125 0.41015625
		 0.3125 0.4140625 0.3125 0.41796875 0.3125 0.421875 0.3125 0.42578125 0.3125 0.4296875
		 0.3125 0.43359375 0.3125 0.62109375 0.3125 0.625 0.3125 0.375 0.68843985 0.37890625
		 0.68843985 0.3828125 0.68843985 0.38671875 0.68843985 0.390625 0.68843985 0.39453125
		 0.68843985 0.3984375 0.68843985 0.40234375 0.68843985 0.40625 0.68843985 0.41015625
		 0.68843985 0.4140625 0.68843985 0.41796875 0.68843985 0.421875 0.68843985 0.42578125
		 0.68843985 0.4296875 0.68843985 0.43359375 0.68843985 0.62109375 0.68843985 0.625
		 0.68843985 0.5 1 0.51531518 0.99924761 0.53048283 0.99699771 0.54535699 0.99327195
		 0.55979425 0.98810613 0.57365572 0.98155022 0.58680785 0.97366714 0.59912395 0.96453285
		 0.61048543 0.95423543 0.62078285 0.94287395 0.62991714 0.93055785 0.63780022 0.91740572
		 0.64435619 0.90354431 0.64952195 0.88910699 0.65324771 0.87423289 0.65549761 0.85906518
		 0.65625 0.84375 0.5 0.15000001 0.5 0.83749998;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 4 ".pt";
	setAttr ".pt[15]" -type "float3" -2.6652386 -1.7763568e-015 0 ;
	setAttr ".pt[32]" -type "float3" -2.6652386 -1.7763568e-015 0 ;
	setAttr ".pt[34]" -type "float3" -2.6652386 -1.7763568e-015 -6.8415176e-032 ;
	setAttr ".pt[35]" -type "float3" -2.6652386 -1.7763568e-015 -6.8415176e-032 ;
	setAttr -s 36 ".vt[0:35]"  2.569942 -3.5 -0.25311637 2.53275728 -3.5 -0.50379592
		 2.47118092 -3.5 -0.7496236 2.38580537 -3.5 -0.98823208 2.27745342 -3.5 -1.2173233
		 2.1471684 -3.5 -1.43469107 1.99620485 -3.5 -1.63824213 1.8260169 -3.5 -1.82601583
		 1.63824332 -3.5 -1.99620402 1.43469262 -3.5 -2.14716768 1.21732485 -3.5 -2.27745295
		 0.98823369 -3.5 -2.38580513 0.74962527 -3.5 -2.47118044 0.50379753 -3.5 -2.53275728
		 0.25311795 -3.5 -2.56994224 7.3112818e-007 -3.5 -2.5823772 2.58237791 -3.5 0 2.569942 3.5 -0.25311637
		 2.53275728 3.5 -0.50379592 2.47118092 3.5 -0.7496236 2.38580537 3.5 -0.98823208 2.27745342 3.5 -1.2173233
		 2.1471684 3.5 -1.43469107 1.99620485 3.5 -1.63824213 1.8260169 3.5 -1.82601583 1.63824332 3.5 -1.99620402
		 1.43469262 3.5 -2.14716768 1.21732485 3.5 -2.27745295 0.98823369 3.5 -2.38580513
		 0.74962527 3.5 -2.47118044 0.50379753 3.5 -2.53275728 0.25311795 3.5 -2.56994224
		 7.3112818e-007 3.5 -2.5823772 2.58237791 3.5 0 0 -3.5 0 0 3.5 0;
	setAttr -s 83 ".ed[0:82]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 6 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 12 0 12 13 0 13 14 0 14 15 0 16 0 0 17 18 0 18 19 0
		 19 20 0 20 21 0 21 22 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0 27 28 0 28 29 0 29 30 0
		 30 31 0 31 32 0 33 17 0 0 17 1 1 18 1 2 19 1 3 20 1 4 21 1 5 22 1 6 23 1 7 24 1 8 25 1
		 9 26 1 10 27 1 11 28 1 12 29 1 13 30 1 14 31 1 15 32 1 16 33 1 34 0 1 34 1 1 34 2 1
		 34 3 1 34 4 1 34 5 1 34 6 1 34 7 1 34 8 1 34 9 1 34 10 1 34 11 1 34 12 1 34 13 1
		 34 14 1 34 15 1 34 16 1 17 35 1 18 35 1 19 35 1 20 35 1 21 35 1 22 35 1 23 35 1 24 35 1
		 25 35 1 26 35 1 27 35 1 28 35 1 29 35 1 30 35 1 31 35 1 32 35 1 33 35 1;
	setAttr -s 48 -ch 160 ".fc[0:47]" -type "polyFaces" 
		f 4 0 33 -17 -33
		mu 0 4 17 18 36 35
		f 4 1 34 -18 -34
		mu 0 4 18 19 37 36
		f 4 2 35 -19 -35
		mu 0 4 19 20 38 37
		f 4 3 36 -20 -36
		mu 0 4 20 21 39 38
		f 4 4 37 -21 -37
		mu 0 4 21 22 40 39
		f 4 5 38 -22 -38
		mu 0 4 22 23 41 40
		f 4 6 39 -23 -39
		mu 0 4 23 24 42 41
		f 4 7 40 -24 -40
		mu 0 4 24 25 43 42
		f 4 8 41 -25 -41
		mu 0 4 25 26 44 43
		f 4 9 42 -26 -42
		mu 0 4 26 27 45 44
		f 4 10 43 -27 -43
		mu 0 4 27 28 46 45
		f 4 11 44 -28 -44
		mu 0 4 28 29 47 46
		f 4 12 45 -29 -45
		mu 0 4 29 30 48 47
		f 4 13 46 -30 -46
		mu 0 4 30 31 49 48
		f 4 14 47 -31 -47
		mu 0 4 31 32 50 49
		f 4 15 32 -32 -49
		mu 0 4 33 34 52 51
		f 3 -1 -50 50
		mu 0 3 1 0 70
		f 3 -2 -51 51
		mu 0 3 2 1 70
		f 3 -3 -52 52
		mu 0 3 3 2 70
		f 3 -4 -53 53
		mu 0 3 4 3 70
		f 3 -5 -54 54
		mu 0 3 5 4 70
		f 3 -6 -55 55
		mu 0 3 6 5 70
		f 3 -7 -56 56
		mu 0 3 7 6 70
		f 3 -8 -57 57
		mu 0 3 8 7 70
		f 3 -9 -58 58
		mu 0 3 9 8 70
		f 3 -10 -59 59
		mu 0 3 10 9 70
		f 3 -11 -60 60
		mu 0 3 11 10 70
		f 3 -12 -61 61
		mu 0 3 12 11 70
		f 3 -13 -62 62
		mu 0 3 13 12 70
		f 3 -14 -63 63
		mu 0 3 14 13 70
		f 3 -15 -64 64
		mu 0 3 15 14 70
		f 3 -16 -66 49
		mu 0 3 0 16 70
		f 3 16 67 -67
		mu 0 3 68 67 71
		f 3 17 68 -68
		mu 0 3 67 66 71
		f 3 18 69 -69
		mu 0 3 66 65 71
		f 3 19 70 -70
		mu 0 3 65 64 71
		f 3 20 71 -71
		mu 0 3 64 63 71
		f 3 21 72 -72
		mu 0 3 63 62 71
		f 3 22 73 -73
		mu 0 3 62 61 71
		f 3 23 74 -74
		mu 0 3 61 60 71
		f 3 24 75 -75
		mu 0 3 60 59 71
		f 3 25 76 -76
		mu 0 3 59 58 71
		f 3 26 77 -77
		mu 0 3 58 57 71
		f 3 27 78 -78
		mu 0 3 57 56 71
		f 3 28 79 -79
		mu 0 3 56 55 71
		f 3 29 80 -80
		mu 0 3 55 54 71
		f 3 30 81 -81
		mu 0 3 54 53 71
		f 3 31 66 -83
		mu 0 3 69 68 71;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "transform25" -p "pCylinder2";
	setAttr ".v" no;
createNode mesh -n "pCylinderShape2" -p "transform25";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.34503086639893799 0.10396743136095499 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "pTorus1";
	setAttr ".t" -type "double3" 0 2.4468085106382982 -7.4255319148936429 ;
createNode transform -n "polySurface1" -p "pTorus1";
	setAttr ".t" -type "double3" 0 -0.10453908036929693 8.8817841970012523e-016 ;
createNode transform -n "polySurface5" -p "polySurface1";
	setAttr ".t" -type "double3" 1.5012295461640917 10.038873020067388 10.200927883833822 ;
createNode transform -n "transform24" -p "polySurface5";
	setAttr ".v" no;
createNode mesh -n "polySurfaceShape5" -p "transform24";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.19651845779364285 0.49999994039535522 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "polySurface6" -p "polySurface1";
	setAttr ".t" -type "double3" 1.5111019251531204 9.9963198285780273 4.3404255319148861 ;
createNode transform -n "transform23" -p "polySurface6";
	setAttr ".v" no;
createNode mesh -n "polySurfaceShape6" -p "transform23";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "transform3" -p "polySurface1";
	setAttr ".v" no;
createNode mesh -n "polySurfaceShape1" -p "transform3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.50000005960464478 0.75 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "polySurface2" -p "pTorus1";
	setAttr ".t" -type "double3" 0 0.62431118207865799 1.7763568394002505e-015 ;
createNode transform -n "polySurface3" -p "polySurface2";
	setAttr ".t" -type "double3" 1.5012295461640917 3.0325627518513576 10.18601602910511 ;
createNode transform -n "transform22" -p "polySurface3";
	setAttr ".v" no;
createNode mesh -n "polySurfaceShape3" -p "transform22";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "polySurface4" -p "polySurface2";
	setAttr ".t" -type "double3" 1.501229546164093 3.0688901593110431 4.3737645863283312 ;
createNode transform -n "transform21" -p "polySurface4";
	setAttr ".v" no;
createNode mesh -n "polySurfaceShape4" -p "transform21";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "transform2" -p "polySurface2";
	setAttr ".v" no;
createNode mesh -n "polySurfaceShape2" -p "transform2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.32500002533197403 0.75 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "transform1" -p "pTorus1";
	setAttr ".v" no;
createNode mesh -n "pTorusShape1" -p "transform1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.45000005513429642 0.75 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 33 ".pt";
	setAttr ".pt[4]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[5]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[6]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[7]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[8]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[9]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[10]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[11]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[12]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[13]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[14]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[24]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[25]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[26]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[27]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[28]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[29]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[30]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[31]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[32]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[33]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[34]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[44]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[45]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[46]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[47]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[48]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[49]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[50]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[51]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[52]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[53]" -type "float3" 0 8.9406967e-008 0 ;
	setAttr ".pt[54]" -type "float3" 0 8.9406967e-008 0 ;
createNode transform -n "pCylinder3";
	setAttr ".t" -type "double3" 1.5012463226046449 6.0714979482368534 -0.14187592375491853 ;
	setAttr ".r" -type "double3" 90 0 0 ;
	setAttr ".s" -type "double3" 0.023903923140400418 1 0.020731640579670606 ;
	setAttr ".rp" -type "double3" 0 0 3.694822225952521e-013 ;
	setAttr ".rpt" -type "double3" 0 -3.7481129311345285e-013 -3.6532851463944456e-013 ;
	setAttr ".spt" -type "double3" 0 0 3.694822225952521e-013 ;
createNode transform -n "transform20" -p "pCylinder3";
	setAttr ".v" no;
createNode mesh -n "pCylinderShape3" -p "transform20";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.5 0.76249998807907104 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "pCylinder4";
	setAttr ".t" -type "double3" 1.5012463226046453 12.3639933449401 -0.14187592375492208 ;
	setAttr ".r" -type "double3" 90 0 0 ;
	setAttr ".s" -type "double3" 0.023903923140400418 1 0.020731640579670606 ;
	setAttr ".rp" -type "double3" 0 0 3.694822225952521e-013 ;
	setAttr ".rpt" -type "double3" 0 -3.7481129311345285e-013 -3.6532851463944456e-013 ;
	setAttr ".spt" -type "double3" 0 0 3.694822225952521e-013 ;
createNode mesh -n "polySurfaceShape7" -p "pCylinder4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.5 0.76249998807907104 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.5 0 0.5 0.3125
		 0.65625 0.15625 0.5 0.3125 0.5625 0.3125 0.625 0.3125 0.5 0.68843985 0.5625 0.68843985
		 0.625 0.68843985 0.5 0.6875 0.5 1 0.65625 0.84375 0.5 0.15000001 0.5 0.83749998;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".vt[0:7]"  4.9517479e-007 -1.47936177 -3.77609324 -1.6505828e-007 -1.47936177 3.77609324
		 3.77609324 -1.47936177 0 4.9517479e-007 1.47936177 -3.77609324 -1.6505828e-007 1.47936177 3.77609324
		 3.77609324 1.47936177 0 0 -1.47936177 0 0 1.47936177 0;
	setAttr -s 11 ".ed[0:10]"  1 2 0 2 0 0 4 5 0 5 3 0 0 3 0 1 4 0 2 5 0
		 6 0 0 6 1 0 3 7 0 4 7 0;
	setAttr -s 4 -ch 16 ".fc[0:3]" -type "polyFaces" 
		f 4 0 6 -3 -6
		mu 0 4 3 4 7 6
		f 4 1 4 -4 -7
		mu 0 4 4 5 8 7
		f 4 -1 -9 7 -2
		mu 0 4 2 1 12 0
		f 4 -11 2 3 9
		mu 0 4 13 9 11 10;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "transform19" -p "pCylinder4";
	setAttr ".v" no;
createNode mesh -n "pCylinderShape4" -p "transform19";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.5 0.76249998807907104 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "pCylinder5";
	setAttr ".t" -type "double3" 1.5012463226046453 9.2424608903626631 2.7040237662868511 ;
	setAttr ".s" -type "double3" 0.023903923140400418 1.1372790934155366 0.020731640579670606 ;
	setAttr ".rp" -type "double3" 0 -1.6653345369377348e-015 5.6848022209749982e-014 ;
	setAttr ".spt" -type "double3" 0 -1.6653345369377348e-015 5.6843418860808015e-014 ;
createNode mesh -n "polySurfaceShape8" -p "pCylinder5";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.5 0.76249998807907104 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.5 0 0.5 0.3125
		 0.65625 0.15625 0.5 0.3125 0.5625 0.3125 0.625 0.3125 0.5 0.68843985 0.5625 0.68843985
		 0.625 0.68843985 0.5 0.6875 0.5 1 0.65625 0.84375 0.5 0.15000001 0.5 0.83749998;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".vt[0:7]"  4.9517479e-007 -1.47936177 -3.77609324 -1.6505828e-007 -1.47936177 3.77609324
		 3.77609324 -1.47936177 0 4.9517479e-007 1.47936177 -3.77609324 -1.6505828e-007 1.47936177 3.77609324
		 3.77609324 1.47936177 0 0 -1.47936177 0 0 1.47936177 0;
	setAttr -s 11 ".ed[0:10]"  1 2 0 2 0 0 4 5 0 5 3 0 0 3 0 1 4 0 2 5 0
		 6 0 0 6 1 0 3 7 0 4 7 0;
	setAttr -s 4 -ch 16 ".fc[0:3]" -type "polyFaces" 
		f 4 0 6 -3 -6
		mu 0 4 3 4 7 6
		f 4 1 4 -4 -7
		mu 0 4 4 5 8 7
		f 4 -1 -9 7 -2
		mu 0 4 2 1 12 0
		f 4 -11 2 3 9
		mu 0 4 13 9 11 10;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "transform18" -p "pCylinder5";
	setAttr ".v" no;
createNode mesh -n "pCylinderShape5" -p "transform18";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.5 0.76249998807907104 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "pCylinder6";
	setAttr ".t" -type "double3" 1.5012463226046455 9.2237667548519546 -3.015778916926537 ;
	setAttr ".s" -type "double3" 0.023903923140400418 1.1544029511168992 0.020731640579670606 ;
	setAttr ".rp" -type "double3" 0 -6.6613381477509392e-016 -2.1316282072803006e-014 ;
	setAttr ".spt" -type "double3" 0 -6.6613381477509392e-016 -2.1316282072803006e-014 ;
createNode mesh -n "polySurfaceShape9" -p "pCylinder6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.5 0.76249998807907104 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.5 0 0.5 0.3125
		 0.65625 0.15625 0.5 0.3125 0.5625 0.3125 0.625 0.3125 0.5 0.68843985 0.5625 0.68843985
		 0.625 0.68843985 0.5 0.6875 0.5 1 0.65625 0.84375 0.5 0.15000001 0.5 0.83749998;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".vt[0:7]"  4.9517479e-007 -1.47936177 -3.77609324 -1.6505828e-007 -1.47936177 3.77609324
		 3.77609324 -1.47936177 0 4.9517479e-007 1.47936177 -3.77609324 -1.6505828e-007 1.47936177 3.77609324
		 3.77609324 1.47936177 0 0 -1.47936177 0 0 1.47936177 0;
	setAttr -s 11 ".ed[0:10]"  1 2 0 2 0 0 4 5 0 5 3 0 0 3 0 1 4 0 2 5 0
		 6 0 0 6 1 0 3 7 0 4 7 0;
	setAttr -s 4 -ch 16 ".fc[0:3]" -type "polyFaces" 
		f 4 0 6 -3 -6
		mu 0 4 3 4 7 6
		f 4 1 4 -4 -7
		mu 0 4 4 5 8 7
		f 4 -1 -9 7 -2
		mu 0 4 2 1 12 0
		f 4 -11 2 3 9
		mu 0 4 13 9 11 10;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "transform17" -p "pCylinder6";
	setAttr ".v" no;
createNode mesh -n "pCylinderShape6" -p "transform17";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.5 0.76249998807907104 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "pCube3";
	setAttr ".t" -type "double3" 2.4171397349926491 3.8078193324582799 2.3537700852486836 ;
	setAttr ".s" -type "double3" 0.52819803346594751 0.3373329592676822 0.14886637952439635 ;
	setAttr ".rp" -type "double3" 0 -3.5527136788005009e-015 1.6007870914869827e-014 ;
	setAttr ".spt" -type "double3" 0 -3.5527136788005009e-015 1.5987211554602254e-014 ;
createNode transform -n "transform16" -p "pCube3";
	setAttr ".v" no;
createNode mesh -n "pCubeShape3" -p "transform16";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.73891095438858212 0.1836044033673353 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "pCube4";
	setAttr ".t" -type "double3" 2.4171397349926491 3.8078193324582812 1.6824489618266587 ;
	setAttr ".s" -type "double3" 0.52819803346594751 0.3373329592676822 0.14886637952439635 ;
	setAttr ".rp" -type "double3" 0 -3.5527136788005009e-015 1.6007870914869827e-014 ;
	setAttr ".spt" -type "double3" 0 -3.5527136788005009e-015 1.5987211554602254e-014 ;
createNode transform -n "transform15" -p "pCube4";
	setAttr ".v" no;
createNode mesh -n "pCubeShape4" -p "transform15";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:5]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.70114845 0.032554299
		 0.7766735 0.032554299 0.70114845 0.10807934 0.7766735 0.10807934 0.70114845 0.18360439
		 0.7766735 0.18360439 0.70114845 0.25912943 0.7766735 0.25912943 0.70114845 0.33465448
		 0.7766735 0.33465448 0.85219854 0.032554299 0.85219854 0.10807934 0.6256234 0.032554299
		 0.6256234 0.10807934;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".vt[0:7]"  -2.76526713 -1.048175097 2.11163425 2.76526713 -1.048175097 2.11163425
		 -2.76526713 1.048175097 2.11163425 2.76526713 1.048175097 2.11163425 -2.76526713 1.048175097 -2.11163425
		 2.76526713 1.048175097 -2.11163425 -2.76526713 -1.048175097 -2.11163425 2.76526713 -1.048175097 -2.11163425;
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
createNode transform -n "pCube5";
	setAttr ".t" -type "double3" 2.4171397349926491 3.8078193324582812 1.0208170833705479 ;
	setAttr ".s" -type "double3" 0.52819803346594751 0.3373329592676822 0.14886637952439635 ;
	setAttr ".rp" -type "double3" 0 -3.5527136788005009e-015 1.6007870914869827e-014 ;
	setAttr ".spt" -type "double3" 0 -3.5527136788005009e-015 1.5987211554602254e-014 ;
createNode transform -n "transform14" -p "pCube5";
	setAttr ".v" no;
createNode mesh -n "pCubeShape5" -p "transform14";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:5]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.70114845 0.032554299
		 0.7766735 0.032554299 0.70114845 0.10807934 0.7766735 0.10807934 0.70114845 0.18360439
		 0.7766735 0.18360439 0.70114845 0.25912943 0.7766735 0.25912943 0.70114845 0.33465448
		 0.7766735 0.33465448 0.85219854 0.032554299 0.85219854 0.10807934 0.6256234 0.032554299
		 0.6256234 0.10807934;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".vt[0:7]"  -2.76526713 -1.048175097 2.11163425 2.76526713 -1.048175097 2.11163425
		 -2.76526713 1.048175097 2.11163425 2.76526713 1.048175097 2.11163425 -2.76526713 1.048175097 -2.11163425
		 2.76526713 1.048175097 -2.11163425 -2.76526713 -1.048175097 -2.11163425 2.76526713 -1.048175097 -2.11163425;
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
createNode transform -n "pCube6";
	setAttr ".t" -type "double3" 2.4171397349926491 3.8078193324582812 0.3278073471570313 ;
	setAttr ".s" -type "double3" 0.52819803346594751 0.3373329592676822 0.14886637952439635 ;
	setAttr ".rp" -type "double3" 0 -3.5527136788005009e-015 1.6007870914869827e-014 ;
	setAttr ".spt" -type "double3" 0 -3.5527136788005009e-015 1.5987211554602254e-014 ;
createNode transform -n "transform13" -p "pCube6";
	setAttr ".v" no;
createNode mesh -n "pCubeShape6" -p "transform13";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:5]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.70114845 0.032554299
		 0.7766735 0.032554299 0.70114845 0.10807934 0.7766735 0.10807934 0.70114845 0.18360439
		 0.7766735 0.18360439 0.70114845 0.25912943 0.7766735 0.25912943 0.70114845 0.33465448
		 0.7766735 0.33465448 0.85219854 0.032554299 0.85219854 0.10807934 0.6256234 0.032554299
		 0.6256234 0.10807934;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".vt[0:7]"  -2.76526713 -1.048175097 2.11163425 2.76526713 -1.048175097 2.11163425
		 -2.76526713 1.048175097 2.11163425 2.76526713 1.048175097 2.11163425 -2.76526713 1.048175097 -2.11163425
		 2.76526713 1.048175097 -2.11163425 -2.76526713 -1.048175097 -2.11163425 2.76526713 -1.048175097 -2.11163425;
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
createNode transform -n "pCube7";
	setAttr ".t" -type "double3" 2.4171397349926491 3.8078193324582812 -0.32083685312661037 ;
	setAttr ".s" -type "double3" 0.52819803346594751 0.3373329592676822 0.14886637952439635 ;
	setAttr ".rp" -type "double3" 0 -3.5527136788005009e-015 1.6007870914869827e-014 ;
	setAttr ".spt" -type "double3" 0 -3.5527136788005009e-015 1.5987211554602254e-014 ;
createNode transform -n "transform12" -p "pCube7";
	setAttr ".v" no;
createNode mesh -n "pCubeShape7" -p "transform12";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:5]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.70114845 0.032554299
		 0.7766735 0.032554299 0.70114845 0.10807934 0.7766735 0.10807934 0.70114845 0.18360439
		 0.7766735 0.18360439 0.70114845 0.25912943 0.7766735 0.25912943 0.70114845 0.33465448
		 0.7766735 0.33465448 0.85219854 0.032554299 0.85219854 0.10807934 0.6256234 0.032554299
		 0.6256234 0.10807934;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".vt[0:7]"  -2.76526713 -1.048175097 2.11163425 2.76526713 -1.048175097 2.11163425
		 -2.76526713 1.048175097 2.11163425 2.76526713 1.048175097 2.11163425 -2.76526713 1.048175097 -2.11163425
		 2.76526713 1.048175097 -2.11163425 -2.76526713 -1.048175097 -2.11163425 2.76526713 -1.048175097 -2.11163425;
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
createNode transform -n "pCube8";
	setAttr ".t" -type "double3" 2.4171397349926491 3.8078193324582812 -1.0057306692776342 ;
	setAttr ".s" -type "double3" 0.52819803346594751 0.3373329592676822 0.14886637952439635 ;
	setAttr ".rp" -type "double3" 0 -3.5527136788005009e-015 1.6007870914869827e-014 ;
	setAttr ".spt" -type "double3" 0 -3.5527136788005009e-015 1.5987211554602254e-014 ;
createNode transform -n "transform11" -p "pCube8";
	setAttr ".v" no;
createNode mesh -n "pCubeShape8" -p "transform11";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:5]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.70114845 0.032554299
		 0.7766735 0.032554299 0.70114845 0.10807934 0.7766735 0.10807934 0.70114845 0.18360439
		 0.7766735 0.18360439 0.70114845 0.25912943 0.7766735 0.25912943 0.70114845 0.33465448
		 0.7766735 0.33465448 0.85219854 0.032554299 0.85219854 0.10807934 0.6256234 0.032554299
		 0.6256234 0.10807934;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".vt[0:7]"  -2.76526713 -1.048175097 2.11163425 2.76526713 -1.048175097 2.11163425
		 -2.76526713 1.048175097 2.11163425 2.76526713 1.048175097 2.11163425 -2.76526713 1.048175097 -2.11163425
		 2.76526713 1.048175097 -2.11163425 -2.76526713 -1.048175097 -2.11163425 2.76526713 -1.048175097 -2.11163425;
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
createNode transform -n "pCube9";
	setAttr ".t" -type "double3" 2.4171397349926491 3.8078193324582812 -1.70523647477553 ;
	setAttr ".s" -type "double3" 0.52819803346594751 0.3373329592676822 0.14886637952439635 ;
	setAttr ".rp" -type "double3" 0 -3.5527136788005009e-015 1.6007870914869827e-014 ;
	setAttr ".spt" -type "double3" 0 -3.5527136788005009e-015 1.5987211554602254e-014 ;
createNode transform -n "transform10" -p "pCube9";
	setAttr ".v" no;
createNode mesh -n "pCubeShape9" -p "transform10";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:5]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.70114845 0.032554299
		 0.7766735 0.032554299 0.70114845 0.10807934 0.7766735 0.10807934 0.70114845 0.18360439
		 0.7766735 0.18360439 0.70114845 0.25912943 0.7766735 0.25912943 0.70114845 0.33465448
		 0.7766735 0.33465448 0.85219854 0.032554299 0.85219854 0.10807934 0.6256234 0.032554299
		 0.6256234 0.10807934;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".vt[0:7]"  -2.76526713 -1.048175097 2.11163425 2.76526713 -1.048175097 2.11163425
		 -2.76526713 1.048175097 2.11163425 2.76526713 1.048175097 2.11163425 -2.76526713 1.048175097 -2.11163425
		 2.76526713 1.048175097 -2.11163425 -2.76526713 -1.048175097 -2.11163425 2.76526713 -1.048175097 -2.11163425;
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
createNode transform -n "pCube10";
	setAttr ".t" -type "double3" 2.4171397349926491 3.8078193324582812 -2.4003987502660387 ;
	setAttr ".s" -type "double3" 0.52819803346594751 0.3373329592676822 0.14886637952439635 ;
	setAttr ".rp" -type "double3" 0 -3.5527136788005009e-015 1.6007870914869827e-014 ;
	setAttr ".spt" -type "double3" 0 -3.5527136788005009e-015 1.5987211554602254e-014 ;
createNode transform -n "transform9" -p "pCube10";
	setAttr ".v" no;
createNode mesh -n "pCubeShape10" -p "transform9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:5]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.70114845 0.032554299
		 0.7766735 0.032554299 0.70114845 0.10807934 0.7766735 0.10807934 0.70114845 0.18360439
		 0.7766735 0.18360439 0.70114845 0.25912943 0.7766735 0.25912943 0.70114845 0.33465448
		 0.7766735 0.33465448 0.85219854 0.032554299 0.85219854 0.10807934 0.6256234 0.032554299
		 0.6256234 0.10807934;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".vt[0:7]"  -2.76526713 -1.048175097 2.11163425 2.76526713 -1.048175097 2.11163425
		 -2.76526713 1.048175097 2.11163425 2.76526713 1.048175097 2.11163425 -2.76526713 1.048175097 -2.11163425
		 2.76526713 1.048175097 -2.11163425 -2.76526713 -1.048175097 -2.11163425 2.76526713 -1.048175097 -2.11163425;
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
createNode transform -n "pCube11";
	setAttr ".t" -type "double3" 1.6585369204003784 4.3613943593229143 2.0585823285637881 ;
	setAttr ".s" -type "double3" 0.52819803346594751 0.30753965868203176 0.091055519590827458 ;
	setAttr ".rp" -type "double3" 0 8.8817841970012523e-015 -2.6650407187722205e-014 ;
	setAttr ".spt" -type "double3" 0 8.8817841970012523e-015 -2.6645352591003757e-014 ;
createNode transform -n "transform8" -p "pCube11";
	setAttr ".v" no;
createNode mesh -n "pCubeShape11" -p "transform8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:5]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.91325138056403388 0.88311849245636076 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.89595199 0.81392086
		 0.93055081 0.81392086 0.89595199 0.84851968 0.93055081 0.84851968 0.89595199 0.88311851
		 0.93055081 0.88311851 0.89595199 0.91771734 0.93055081 0.91771734 0.89595199 0.95231616
		 0.93055081 0.95231616 0.96514964 0.81392086 0.96514964 0.84851968 0.86135316 0.81392086
		 0.86135316 0.84851968;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 6 ".pt";
	setAttr ".pt[0]" -type "float3" 1.4362088 0 9.7699626e-015 ;
	setAttr ".pt[2]" -type "float3" 1.4362088 0 -0.33460841 ;
	setAttr ".pt[3]" -type "float3" -0.23289879 0 -0.33460841 ;
	setAttr ".pt[4]" -type "float3" 1.4362088 0 0.33460841 ;
	setAttr ".pt[5]" -type "float3" -0.23289879 0 0.33460841 ;
	setAttr ".pt[6]" -type "float3" 1.4362088 0 9.7699626e-015 ;
	setAttr -s 8 ".vt[0:7]"  -2.76526713 -1.048175097 2.11163425 2.76526713 -1.048175097 2.11163425
		 -2.76526713 1.048175097 2.11163425 2.76526713 1.048175097 2.11163425 -2.76526713 1.048175097 -2.11163425
		 2.76526713 1.048175097 -2.11163425 -2.76526713 -1.048175097 -2.11163425 2.76526713 -1.048175097 -2.11163425;
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
createNode transform -n "pCube12";
	setAttr ".t" -type "double3" 1.6585369204003784 4.3613943593229143 1.390066196810289 ;
	setAttr ".s" -type "double3" 0.52819803346594751 0.30753965868203176 0.091055519590827458 ;
	setAttr ".rp" -type "double3" 0 8.8817841970012523e-015 -2.6650407187722205e-014 ;
	setAttr ".spt" -type "double3" 0 8.8817841970012523e-015 -2.6645352591003757e-014 ;
createNode transform -n "transform7" -p "pCube12";
	setAttr ".v" no;
createNode mesh -n "pCubeShape12" -p "transform7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:5]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.85513790517221666 0.88311847755519934 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.83891547 0.81822884
		 0.8713603 0.81822884 0.83891547 0.85067368 0.8713603 0.85067368 0.83891547 0.88311851
		 0.8713603 0.88311851 0.83891547 0.91556334 0.8713603 0.91556334 0.83891547 0.94800806
		 0.8713603 0.94800806 0.90380514 0.81822884 0.90380514 0.85067368 0.80647075 0.81822884
		 0.80647075 0.85067368;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 6 ".pt";
	setAttr ".pt[0]" -type "float3" 1.4362088 0 9.7699626e-015 ;
	setAttr ".pt[2]" -type "float3" 1.4362088 0 -0.33460841 ;
	setAttr ".pt[3]" -type "float3" -0.23289879 0 -0.33460841 ;
	setAttr ".pt[4]" -type "float3" 1.4362088 0 0.33460841 ;
	setAttr ".pt[5]" -type "float3" -0.23289879 0 0.33460841 ;
	setAttr ".pt[6]" -type "float3" 1.4362088 0 9.7699626e-015 ;
	setAttr -s 8 ".vt[0:7]"  -2.76526713 -1.048175097 2.11163425 2.76526713 -1.048175097 2.11163425
		 -2.76526713 1.048175097 2.11163425 2.76526713 1.048175097 2.11163425 -2.76526713 1.048175097 -2.11163425
		 2.76526713 1.048175097 -2.11163425 -2.76526713 -1.048175097 -2.11163425 2.76526713 -1.048175097 -2.11163425;
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
createNode transform -n "pCube13";
	setAttr ".t" -type "double3" 1.6585369204003784 4.3613943593229143 0.013099093566631748 ;
	setAttr ".s" -type "double3" 0.52819803346594751 0.30753965868203176 0.091055519590827458 ;
	setAttr ".rp" -type "double3" 0 8.8817841970012523e-015 -2.6650407187722205e-014 ;
	setAttr ".spt" -type "double3" 0 8.8817841970012523e-015 -2.6645352591003757e-014 ;
createNode transform -n "transform6" -p "pCube13";
	setAttr ".v" no;
createNode mesh -n "pCubeShape13" -p "transform6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:5]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.88527083323950551 0.87235672256769381 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.86366338 0.785927
		 0.90687823 0.785927 0.86366338 0.82914186 0.90687823 0.82914186 0.86366338 0.87235671
		 0.90687823 0.87235671 0.86366338 0.91557157 0.90687823 0.91557157 0.86366338 0.95878643
		 0.90687823 0.95878643 0.95009315 0.785927 0.95009315 0.82914186 0.82044852 0.785927
		 0.82044852 0.82914186;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 6 ".pt";
	setAttr ".pt[0]" -type "float3" 1.4362088 0 9.7699626e-015 ;
	setAttr ".pt[2]" -type "float3" 1.4362088 0 -0.33460841 ;
	setAttr ".pt[3]" -type "float3" -0.23289879 0 -0.33460841 ;
	setAttr ".pt[4]" -type "float3" 1.4362088 0 0.33460841 ;
	setAttr ".pt[5]" -type "float3" -0.23289879 0 0.33460841 ;
	setAttr ".pt[6]" -type "float3" 1.4362088 0 9.7699626e-015 ;
	setAttr -s 8 ".vt[0:7]"  -2.76526713 -1.048175097 2.11163425 2.76526713 -1.048175097 2.11163425
		 -2.76526713 1.048175097 2.11163425 2.76526713 1.048175097 2.11163425 -2.76526713 1.048175097 -2.11163425
		 2.76526713 1.048175097 -2.11163425 -2.76526713 -1.048175097 -2.11163425 2.76526713 -1.048175097 -2.11163425;
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
createNode transform -n "pCube14";
	setAttr ".t" -type "double3" 1.6585369204003784 4.3613943593229143 -0.64205657679259875 ;
	setAttr ".s" -type "double3" 0.52819803346594751 0.30753965868203176 0.091055519590827458 ;
	setAttr ".rp" -type "double3" 0 8.8817841970012523e-015 -2.6650407187722205e-014 ;
	setAttr ".spt" -type "double3" 0 8.8817841970012523e-015 -2.6645352591003757e-014 ;
createNode transform -n "transform5" -p "pCube14";
	setAttr ".v" no;
createNode mesh -n "pCubeShape14" -p "transform5";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:5]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.89818493888213136 0.87235672256769381 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.87873149 0.79454303
		 0.9176383 0.79454303 0.87873149 0.83344984 0.9176383 0.83344984 0.87873149 0.87235671
		 0.9176383 0.87235671 0.87873149 0.91126353 0.9176383 0.91126353 0.87873149 0.9501704
		 0.9176383 0.9501704 0.95654523 0.79454303 0.95654523 0.83344984 0.83982468 0.79454303
		 0.83982468 0.83344984;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 6 ".pt";
	setAttr ".pt[0]" -type "float3" 1.4362088 0 9.7699626e-015 ;
	setAttr ".pt[2]" -type "float3" 1.4362088 0 -0.33460841 ;
	setAttr ".pt[3]" -type "float3" -0.23289879 0 -0.33460841 ;
	setAttr ".pt[4]" -type "float3" 1.4362088 0 0.33460841 ;
	setAttr ".pt[5]" -type "float3" -0.23289879 0 0.33460841 ;
	setAttr ".pt[6]" -type "float3" 1.4362088 0 9.7699626e-015 ;
	setAttr -s 8 ".vt[0:7]"  -2.76526713 -1.048175097 2.11163425 2.76526713 -1.048175097 2.11163425
		 -2.76526713 1.048175097 2.11163425 2.76526713 1.048175097 2.11163425 -2.76526713 1.048175097 -2.11163425
		 2.76526713 1.048175097 -2.11163425 -2.76526713 -1.048175097 -2.11163425 2.76526713 -1.048175097 -2.11163425;
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
createNode transform -n "pCube15";
	setAttr ".t" -type "double3" 1.6585369204003784 4.3613943593229143 -1.2838026790402621 ;
	setAttr ".s" -type "double3" 0.52819803346594751 0.30753965868203176 0.091055519590827458 ;
	setAttr ".rp" -type "double3" 0 8.8817841970012523e-015 -2.6650407187722205e-014 ;
	setAttr ".spt" -type "double3" 0 8.8817841970012523e-015 -2.6645352591003757e-014 ;
createNode transform -n "transform4" -p "pCube15";
	setAttr ".v" no;
createNode mesh -n "pCubeShape15" -p "transform4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:5]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.25 0.5 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.87873149 0.79454303
		 0.9176383 0.79454303 0.87873149 0.83344984 0.9176383 0.83344984 0.87873149 0.87235671
		 0.9176383 0.87235671 0.87873149 0.91126353 0.9176383 0.91126353 0.87873149 0.9501704
		 0.9176383 0.9501704 0.95654523 0.79454303 0.95654523 0.83344984 0.83982468 0.79454303
		 0.83982468 0.83344984;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 6 ".pt";
	setAttr ".pt[0]" -type "float3" 1.4362088 0 9.7699626e-015 ;
	setAttr ".pt[2]" -type "float3" 1.4362088 0 -0.33460841 ;
	setAttr ".pt[3]" -type "float3" -0.23289879 0 -0.33460841 ;
	setAttr ".pt[4]" -type "float3" 1.4362088 0 0.33460841 ;
	setAttr ".pt[5]" -type "float3" -0.23289879 0 0.33460841 ;
	setAttr ".pt[6]" -type "float3" 1.4362088 0 9.7699626e-015 ;
	setAttr -s 8 ".vt[0:7]"  -2.76526713 -1.048175097 2.11163425 2.76526713 -1.048175097 2.11163425
		 -2.76526713 1.048175097 2.11163425 2.76526713 1.048175097 2.11163425 -2.76526713 1.048175097 -2.11163425
		 2.76526713 1.048175097 -2.11163425 -2.76526713 -1.048175097 -2.11163425 2.76526713 -1.048175097 -2.11163425;
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
createNode transform -n "corridor1:pCube3";
	setAttr ".t" -type "double3" 0.76396559587260526 45.376168307992828 58.020323832319406 ;
	setAttr ".s" -type "double3" 1 1.0000000000000004 1 ;
createNode transform -n "corridor1:transform2" -p "corridor1:pCube3";
createNode mesh -n "corridor1:pCubeShape3" -p "corridor1:transform2";
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
createNode transform -n "corridor1:pCube4";
	setAttr ".t" -type "double3" 0.76396559587260526 69.420071768319673 58.020323832319406 ;
	setAttr ".s" -type "double3" 1 1.0000000000000004 1 ;
createNode mesh -n "corridor1:polySurfaceShape1" -p "corridor1:pCube4";
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
createNode transform -n "corridor1:transform1" -p "corridor1:pCube4";
createNode mesh -n "corridor1:pCubeShape4" -p "corridor1:transform1";
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
createNode transform -n "corridor1:window_frame1";
	setAttr ".t" -type "double3" 431.64305679431499 0 0 ;
createNode transform -n "corridor1:polySurface1" -p "corridor1:window_frame1";
	setAttr ".t" -type "double3" 1.8465552077534539 -2.1273015654206375 -115.19057078437376 ;
	setAttr ".rp" -type "double3" -1.5676768063777899 37.85066362836573 55.179079296443199 ;
	setAttr ".sp" -type "double3" -1.5676768063777899 37.85066362836573 55.179079296443199 ;
createNode transform -n "corridor1:transform6" -p "corridor1:polySurface1";
createNode mesh -n "corridor1:polySurfaceShape2" -p "corridor1:transform6";
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
createNode transform -n "corridor1:polySurface2" -p "corridor1:window_frame1";
	setAttr ".t" -type "double3" 0 0 -131.01785264469927 ;
	setAttr ".s" -type "double3" 1 1 1.2486904136388408 ;
createNode transform -n "corridor1:transform5" -p "corridor1:polySurface2";
createNode mesh -n "corridor1:polySurfaceShape3" -p "corridor1:transform5";
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
createNode transform -n "corridor1:transform3" -p "corridor1:window_frame1";
createNode mesh -n "corridor1:window_frame1Shape" -p "corridor1:transform3";
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
createNode transform -n "corridor1:pCube5";
	setAttr ".t" -type "double3" 403.33241877964525 64.568470637198658 -58.877627432537551 ;
createNode transform -n "corridor1:transform4" -p "corridor1:pCube5";
createNode mesh -n "corridor1:pCubeShape5" -p "corridor1:transform4";
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
createNode transform -n "corridor1:pCube7";
	setAttr ".t" -type "double3" -122.73679106243407 32.842935136945201 121.95922510387692 ;
createNode transform -n "corridor1:transform12" -p "corridor1:pCube7";
createNode mesh -n "corridor1:pCubeShape7" -p "corridor1:transform12";
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
createNode transform -n "corridor1:pCube8";
	setAttr ".t" -type "double3" -122.73679106243407 7.8471795801870812 121.95922510387692 ;
	setAttr ".s" -type "double3" 0.81555620425524034 4.3612924960166328 0.79903990761831523 ;
createNode transform -n "corridor1:transform11" -p "corridor1:pCube8";
createNode mesh -n "corridor1:pCubeShape8" -p "corridor1:transform11";
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
createNode transform -n "corridor1:pCylinder1";
	setAttr ".t" -type "double3" -214.87060344334282 41.676197356733567 31.838748924432196 ;
	setAttr ".rp" -type "double3" 110.25223844624442 -29.326043151447109 102.05839551589557 ;
	setAttr ".sp" -type "double3" 110.25223844624442 -29.326043151447109 102.05839551589557 ;
createNode transform -n "corridor1:transform10" -p "corridor1:pCylinder1";
createNode mesh -n "corridor1:pCylinderShape1" -p "corridor1:transform10";
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
createNode transform -n "corridor1:pCylinder2";
	setAttr ".t" -type "double3" -214.02905852745869 41.676197356733567 8.5067256565830718 ;
	setAttr ".r" -type "double3" 0 92.532652790026731 0 ;
	setAttr ".rp" -type "double3" 110.25223844624442 -29.326043151447109 102.05839551589557 ;
	setAttr ".sp" -type "double3" 110.25223844624442 -29.326043151447109 102.05839551589557 ;
createNode transform -n "corridor1:transform9" -p "corridor1:pCylinder2";
createNode mesh -n "corridor1:pCylinderShape2" -p "corridor1:transform9";
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
createNode transform -n "corridor1:pCylinder3";
	setAttr ".t" -type "double3" -252.22910332188749 41.676197356733567 8.1902859208535403 ;
	setAttr ".r" -type "double3" 0 181.96845630878249 0 ;
	setAttr ".rp" -type "double3" 110.25223844624442 -29.326043151447109 102.05839551589557 ;
	setAttr ".sp" -type "double3" 110.25223844624442 -29.326043151447109 102.05839551589557 ;
createNode transform -n "corridor1:transform8" -p "corridor1:pCylinder3";
createNode mesh -n "corridor1:pCylinderShape3" -p "corridor1:transform8";
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
createNode transform -n "corridor1:pCylinder4";
	setAttr ".t" -type "double3" -251.96159881835132 41.676197356733567 31.101321568577319 ;
	setAttr ".r" -type "double3" 0 273.51828896125727 0 ;
	setAttr ".rp" -type "double3" 110.25223844624442 -29.326043151447109 102.05839551589557 ;
	setAttr ".sp" -type "double3" 110.25223844624442 -29.326043151447109 102.05839551589557 ;
createNode transform -n "corridor1:transform7" -p "corridor1:pCylinder4";
createNode mesh -n "corridor1:pCylinderShape4" -p "corridor1:transform7";
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
createNode transform -n "corridor1:left";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -347.68232400733405 61.171016544573263 153.99010241051892 ;
	setAttr ".r" -type "double3" 0 -89.999999999999986 0 ;
createNode camera -n "corridor1:leftShape" -p "corridor1:left";
	setAttr -k off ".v";
	setAttr ".rnd" no;
	setAttr ".coi" 308.56522706313001;
	setAttr ".ow" 554.54460046370536;
	setAttr ".imn" -type "string" "left1";
	setAttr ".den" -type "string" "left1_depth";
	setAttr ".man" -type "string" "left1_mask";
	setAttr ".hc" -type "string" "viewSet -ls %camera";
	setAttr ".o" yes;
createNode transform -n "corridor1:pCube10";
	setAttr ".t" -type "double3" 0.70273672668440668 58.076476094454073 42.306032658796838 ;
	setAttr ".s" -type "double3" 0.18255422909586339 1 1.1151411804345381 ;
	setAttr ".rp" -type "double3" 0 -17.099405499625895 23.151825593087128 ;
	setAttr ".sp" -type "double3" 0 -17.099405499625895 23.002240100663453 ;
	setAttr ".spt" -type "double3" 0 0 0.14958549242366903 ;
createNode transform -n "corridor1:transform14" -p "corridor1:pCube10";
	setAttr ".v" no;
createNode mesh -n "corridor1:pCubeShape10" -p "corridor1:transform14";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" -0.50485715270042419 1.9466979668941349 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "corridor1:pCube11";
	setAttr ".t" -type "double3" -0.85217509506102118 62.825313515271141 65.576768113219586 ;
	setAttr ".s" -type "double3" 1 0.70262150769898757 1.4500442818010963 ;
createNode transform -n "corridor1:transform13" -p "corridor1:pCube11";
	setAttr ".v" no;
createNode mesh -n "corridor1:pCubeShape11" -p "corridor1:transform13";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "door1:pCube12";
	setAttr ".t" -type "double3" 1.041795023252305 -58.526080202809581 21.306399334995223 ;
	setAttr ".s" -type "double3" 0.53146142269335306 0.69849556595089912 0.53146142269335306 ;
	setAttr ".rp" -type "double3" 0.70273661613464355 58.725679397583008 -21.25268392264843 ;
	setAttr ".sp" -type "double3" 0.70273661613464355 58.725679397583008 -21.25268392264843 ;
createNode mesh -n "door1:pCube12Shape" -p "door1:pCube12";
	setAttr -k off ".v";
	setAttr -s 4 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" -5.9604644775390625e-008 0.5 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr -s 5 ".pt";
	setAttr ".pt[1]" -type "float3" 6.5336051 -4.3817911 8.943222 ;
	setAttr ".pt[9]" -type "float3" 6.5336051 -4.3817911 -10.41475 ;
	setAttr ".pt[10]" -type "float3" 5.7433662 5.2424383 8.943222 ;
	setAttr ".pt[16]" -type "float3" 5.7433662 5.2424383 -10.41475 ;
createNode mesh -n "door1:polySurfaceShape1" -p "door1:pCube12";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 2 ".iog[0].og";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:27]";
	setAttr ".iog[0].og[1].gcl" -type "componentList" 1 "f[28:39]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" -0.33437502384185791 0.5 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 40 ".uvst[0].uvsp[0:39]" -type "float2" -1.0043510199 0.0020000001
		 -0.0053630001 0.0020000001 -1.0043510199 0.97434902 -1.83511996 0.81061602 -1.83511996
		 0.87856299 -2.0033390522 0.97434902 -1.18522203 0.878564 -1.18522203 0.61632001 -1.63549995
		 0.61632001 -1.0043510199 3.89139605 -0.0053630001 2.91904712 -0.0053630001 3.89139605
		 -0.0053630001 0.97434902 -1.0043510199 1.94669795 -2.0033390522 0.0020000001 -1.83511996
		 0.61632001 -1.0043510199 2.91904712 0.17550801 0.79830098 0.17550801 0.61459899 0.99362397
		 0.0020000001 0.36424199 0.61459899 -0.0053630001 1.94669795 0.17550801 0.87610799
		 0.99362397 0.97434902 0.82540601 0.87610799 0.82540601 0.61459899 0.49849001 0 0.49849001
		 0.25 0.51647198 0 0.51647198 0.17561699 0.51647198 0.25 0.49849001 0.75 0.49849001
		 0.82678902 0.51647198 0.75 0.51647198 0.92739803 0.49849001 1 0.51647198 1 0.49849001
		 0.5 0.51647198 0.5 0.49849001 0.56477499;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr -s 14 ".pt";
	setAttr ".pt[0]" -type "float3" 1.1920929e-007 37.433426 0 ;
	setAttr ".pt[1]" -type "float3" 1.1920929e-007 37.433426 0 ;
	setAttr ".pt[4]" -type "float3" 0 0 -3.8628082 ;
	setAttr ".pt[5]" -type "float3" 0 5.0925364 -3.8628073 ;
	setAttr ".pt[6]" -type "float3" 0 5.0925364 4.5049362 ;
	setAttr ".pt[7]" -type "float3" 0 0 4.5049362 ;
	setAttr ".pt[9]" -type "float3" 1.1920929e-007 37.433426 0 ;
	setAttr ".pt[11]" -type "float3" 1.1920929e-007 37.433426 0 ;
	setAttr ".pt[12]" -type "float3" 0 0 -3.8628082 ;
	setAttr ".pt[14]" -type "float3" 0 0 4.5049362 ;
	setAttr ".pt[15]" -type "float3" 0 0 4.5049362 ;
	setAttr ".pt[17]" -type "float3" 0 5.0925364 4.5049362 ;
	setAttr ".pt[18]" -type "float3" 0 5.0925364 -3.8628073 ;
	setAttr ".pt[19]" -type "float3" 0 0 -3.8628082 ;
	setAttr -s 20 ".vt[0:19]"  -1.16593504 0.52511197 -0.26314601 2.57140803 0.52511197 -0.26314601
		 -1.16593504 79.26456451 -0.26314601 -1.16593504 79.26459503 -42.24222183 -1.16593504 65.58061981 -35.17340088
		 -1.16593504 71.25931549 -35.17340088 -1.16593504 71.25931549 -7.86363316 -1.16593504 40.36016464 -7.86363316
		 -1.16593504 40.36016464 -26.78507233 2.57140803 0.52511197 -42.24222183 2.57140803 79.49282074 -0.26314601
		 -1.16593504 0.52511197 -42.24222183 -1.16593504 40.36016464 -35.17340088 2.57140803 40.36016464 -15.79457092
		 2.57140803 40.36016464 -7.86363316 2.57140803 64.7383194 -7.86363316 2.57140803 79.49282074 -42.24222183
		 2.57140803 71.25931549 -7.86363316 2.57140803 71.25931549 -35.17340088 2.57140803 40.36016464 -35.17340088;
	setAttr -s 60 ".ed[0:59]"  0 1 0 1 2 0 2 0 0 4 5 0 5 3 0 3 4 0 5 2 0
		 2 3 0 5 6 0 6 2 0 6 0 0 6 7 0 7 0 0 7 8 0 8 0 0 0 9 0 9 1 0 1 10 0 10 2 0 10 3 0
		 3 11 0 11 4 0 11 12 0 12 4 0 8 11 0 11 0 0 8 12 0 11 9 0 1 15 0 15 10 0 1 14 0 14 15 0
		 9 13 0 13 1 0 13 14 0 10 16 0 16 3 0 3 9 0 15 17 0 17 10 0 17 16 0 17 18 0 18 16 0
		 18 9 0 9 16 0 18 19 0 19 9 0 19 13 0 6 14 0 14 7 0 6 15 0 6 17 0 8 19 0 19 12 0 8 13 0
		 7 13 0 5 17 0 5 18 0 4 18 0 4 19 0;
	setAttr -s 120 ".n[0:119]" -type "float3"  0 0 1 0 0 1 -2e-006 0 1 -1
		 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0
		 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 0 -1 0 0 -1 0 0 -1 0 -2e-006 0 1 0 0 1
		 -2e-006 0 1 -0.060961001 0.99813998 1e-006 -0.060959999 0.99813998 1e-006 -0.060952999
		 0.99814099 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0
		 0 -1 0 0 -1 0 0 0 -1 0 0 -1 0 0 -1 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1
		 0 0 1 0 0 1 0 0 1 0 0 1 0 0 -0.060952999 0.99814099 0 -0.060959999 0.99813998 1e-006
		 -0.060952999 0.99814099 0 0 0 -1 0 0 -1 0 0 -1 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0
		 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 0 0 -1
		 0 0 -1 0 0 -1 0 0 -1 1e-006 0 -1 0 0 -1 0 0 -1 1e-006 0 -1 3.0000001e-006 0 -1 1e-006
		 0 -1 0 0 -1 3.0000001e-006 0 -1 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0
		 1 0 0 1 0 0 1 0 0 1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 0 1 0 0 1 0 0 1
		 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1;
	setAttr -s 40 -ch 120 ".fc[0:39]" -type "polyFaces" 
		f 3 0 1 2
		mu 0 3 0 1 2
		f 3 3 4 5
		mu 0 3 3 4 5
		f 3 -5 6 7
		mu 0 3 5 4 2
		f 3 8 9 -7
		mu 0 3 4 6 2
		f 3 -10 10 -3
		mu 0 3 2 6 0
		f 3 11 12 -11
		mu 0 3 6 7 0
		f 3 13 14 -13
		mu 0 3 7 8 0
		f 3 15 16 -1
		mu 0 3 9 10 11
		f 3 -2 17 18
		mu 0 3 2 1 12
		f 3 -19 19 -8
		mu 0 3 2 12 13
		f 3 20 21 -6
		mu 0 3 5 14 3
		f 3 -22 22 23
		mu 0 3 3 14 15
		f 3 -15 24 25
		mu 0 3 0 8 14
		f 3 -25 26 -23
		mu 0 3 14 8 15
		f 3 27 -16 -26
		mu 0 3 16 10 9
		f 3 -18 28 29
		mu 0 3 12 1 17
		f 3 -29 30 31
		mu 0 3 17 1 18
		f 3 32 33 -17
		mu 0 3 19 20 1
		f 3 -34 34 -31
		mu 0 3 1 20 18
		f 3 -20 35 36
		mu 0 3 13 12 21
		f 3 37 -28 -21
		mu 0 3 13 10 16
		f 3 38 39 -30
		mu 0 3 17 22 12
		f 3 -40 40 -36
		mu 0 3 12 22 23
		f 3 41 42 -41
		mu 0 3 22 24 23
		f 3 -43 43 44
		mu 0 3 23 24 19
		f 3 45 46 -44
		mu 0 3 24 25 19
		f 3 47 -33 -47
		mu 0 3 25 20 19
		f 3 -45 -38 -37
		mu 0 3 21 10 13
		f 3 -12 48 49
		mu 0 3 26 27 28
		f 3 -49 50 -32
		mu 0 3 28 27 29
		f 3 51 -39 -51
		mu 0 3 27 30 29
		f 3 -27 52 53
		mu 0 3 31 32 33
		f 3 -53 54 -48
		mu 0 3 33 32 34
		f 3 -14 55 -55
		mu 0 3 32 35 34
		f 3 -50 -35 -56
		mu 0 3 35 36 34
		f 3 -9 56 -52
		mu 0 3 27 37 30
		f 3 -57 57 -42
		mu 0 3 30 37 38
		f 3 -4 58 -58
		mu 0 3 37 39 38
		f 3 -59 59 -46
		mu 0 3 38 39 33
		f 3 -24 -54 -60
		mu 0 3 39 31 33;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "door1:pCube13";
	setAttr ".t" -type "double3" 1.041795023252305 -58.526080202809581 21.306399334995223 ;
	setAttr ".s" -type "double3" 0.53146142269335306 0.74728387187426537 0.60066714106770802 ;
	setAttr ".rp" -type "double3" 0.62545102834701538 58.658742904663086 -21.345752954483036 ;
	setAttr ".sp" -type "double3" 0.62545102834701538 58.658742904663086 -21.345752954483032 ;
	setAttr ".spt" -type "double3" 0 1.7763568394002505e-015 -2.886579864025407e-015 ;
createNode mesh -n "door1:pCube13Shape" -p "door1:pCube13";
	setAttr -k off ".v";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:1]";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.50071489810943604 0.50104029476642609 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 4 ".uvst[0].uvsp[0:3]" -type "float2" 0.0055044889 0.0058299005
		 0.99592531 0.0058299005 0.0055044889 0.99625069 0.99592531 0.99625069;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr -s 4 ".vt[0:3]"  0.62545103 40.71600723 -4.27751207 0.62545103 76.60147858 -4.27751207
		 0.62545103 76.60147858 -38.41399384 0.62545103 40.71600723 -38.41399384;
	setAttr -s 5 ".ed[0:4]"  0 3 0 3 1 0 1 0 0 3 2 0 2 1 0;
	setAttr -s 6 ".n[0:5]" -type "float3"  1e+020 1e+020 1e+020 1e+020
		 1e+020 1e+020 1e+020 1e+020 1e+020 1 0 0 1 0 0 1 0 0;
	setAttr -s 2 -ch 6 ".fc[0:1]" -type "polyFaces" 
		f 3 0 1 2
		mu 0 3 0 1 2
		f 3 -2 3 4
		mu 0 3 2 1 3;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode lightLinker -s -n "lightLinker1";
	setAttr -s 16 ".lnk";
	setAttr -s 16 ".slnk";
createNode displayLayerManager -n "layerManager";
createNode displayLayer -n "defaultLayer";
createNode renderLayerManager -n "renderLayerManager";
createNode renderLayer -n "defaultRenderLayer";
	setAttr ".g" yes;
createNode polyCube -n "polyCube1";
	setAttr ".w" 3;
	setAttr ".h" 11.8668608956831;
	setAttr ".d" 7;
	setAttr ".cuv" 4;
createNode polyCylinder -n "polyCylinder1";
	setAttr ".r" 2.5823780125975198;
	setAttr ".h" 7;
	setAttr ".sa" 64;
	setAttr ".sc" 1;
	setAttr ".cuv" 3;
createNode deleteComponent -n "deleteComponent1";
	setAttr ".dc" -type "componentList" 3 "f[15:46]" "f[79:110]" "f[143:174]";
createNode deleteComponent -n "deleteComponent2";
	setAttr ".dc" -type "componentList" 3 "f[15:30]" "f[47:62]" "f[79:94]";
createNode script -n "uiConfigurationScriptNode";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"top\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"smoothShaded\" \n"
		+ "                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 0\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 1\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"vp2Renderer\" \n                -objectFilterShowInHUD 1\n"
		+ "                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n"
		+ "                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 1\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n"
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
		+ "        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-defaultImage \"vacantCell.xP:/\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 1\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 1\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        setFocus `paneLayout -q -p1 $gMainPane`;\n        sceneUIReplacement -deleteRemaining;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 120 -ast 1 -aet 200 ";
	setAttr ".st" 6;
createNode polyCube -n "polyCube2";
	setAttr ".w" 3;
	setAttr ".h" 0.5;
	setAttr ".d" 7;
	setAttr ".cuv" 4;
createNode polySplitRing -n "polySplitRing1";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 2 "e[4:5]" "e[8:9]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 5.93343044784155 0 1;
	setAttr ".wt" 0.92803388833999634;
	setAttr ".dr" no;
	setAttr ".re" 5;
	setAttr ".sma" 29.999999999999996;
	setAttr ".p[0]"  0 0 1;
	setAttr ".fq" yes;
createNode polyTorus -n "polyTorus1";
	setAttr ".ax" -type "double3" 1.0000000000000002 0 2.2204460492503131e-016 ;
	setAttr ".r" 1.5206441178898418;
	setAttr ".sr" 0.08;
	setAttr ".sh" 4;
createNode deleteComponent -n "deleteComponent3";
	setAttr ".dc" -type "componentList" 1 "f[40:79]";
createNode polyChipOff -n "polyChipOff1";
	setAttr ".ics" -type "componentList" 2 "f[4:13]" "f[24:33]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 2.4468085106382982 -7.4255319148936429 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0 2.4468086 -7.4255319 ;
	setAttr ".rs" 52086;
	setAttr ".dup" no;
createNode polySeparate -n "polySeparate1";
	setAttr ".ic" 2;
	setAttr -s 2 ".out";
createNode groupId -n "groupId1";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts1";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:39]";
createNode groupId -n "groupId2";
	setAttr ".ihi" 0;
createNode groupId -n "groupId3";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts2";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:19]";
createNode groupId -n "groupId4";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts3";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:19]";
createNode polyChipOff -n "polyChipOff2";
	setAttr ".ics" -type "componentList" 2 "f[0:4]" "f[10:14]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 3.0711196927169562 -7.4255319148936412 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0 3.0711198 -7.4255319 ;
	setAttr ".rs" 60875;
	setAttr ".dup" no;
createNode polySeparate -n "polySeparate2";
	setAttr ".ic" 2;
	setAttr -s 2 ".out";
createNode polyChipOff -n "polyChipOff3";
	setAttr ".ics" -type "componentList" 3 "f[0:3]" "f[9:13]" "f[19]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 2.3422694302690013 -7.4255319148936421 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0 2.3422694 -7.4255319 ;
	setAttr ".rs" 42676;
	setAttr ".dup" no;
createNode polySeparate -n "polySeparate3";
	setAttr ".ic" 2;
	setAttr -s 2 ".out";
createNode polySoftEdge -n "polySoftEdge1";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "e[0:19]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 5.93343044784155 0 1;
	setAttr ".a" 0;
createNode polyTweak -n "polyTweak1";
	setAttr ".uopa" yes;
	setAttr -s 4 ".tk[2:5]" -type "float3"  -0.34790772 0 0.36443162 0.34790772
		 0 0.36443162 -0.34790772 0 -0.36443162 0.34790772 0 -0.36443162;
createNode polyCylinder -n "polyCylinder2";
	setAttr ".r" 3.7760931889467462;
	setAttr ".h" 2.9587235145190185;
	setAttr ".sa" 4;
	setAttr ".sc" 1;
	setAttr ".cuv" 3;
createNode deleteComponent -n "deleteComponent4";
	setAttr ".dc" -type "componentList" 3 "f[0:1]" "f[4:5]" "f[8:9]";
createNode deleteComponent -n "deleteComponent5";
	setAttr ".dc" -type "componentList" 1 "e[12]";
createNode deleteComponent -n "deleteComponent6";
	setAttr ".dc" -type "componentList" 1 "e[9]";
createNode polyCube -n "polyCube3";
	setAttr ".w" 5.5305343333537849;
	setAttr ".h" 2.0963502129191269;
	setAttr ".d" 4.22326866377351;
	setAttr ".cuv" 4;
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
createNode shadingEngine -n "corridor1:pz_fence:pz_fence";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "corridor1:pz_fence:materialInfo1";
createNode lambert -n "corridor1:pz_fence:pz_fence1";
	setAttr ".ambc" -type "float3" 0.40000001 0.40000001 0.40000001 ;
createNode file -n "corridor1:pz_fence:pz_fence1F";
	setAttr ".ftn" -type "string" "pz-fence.png";
createNode place2dTexture -n "corridor1:pz_fence:pz_fence1P2D";
createNode shadingEngine -n "corridor1:cube_0_5:lambert2SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "corridor1:cube_0_5:materialInfo1";
createNode lambert -n "corridor1:cube_0_5:lambert2SG1";
createNode file -n "corridor1:cube_0_5:lambert2SG1F";
	setAttr ".ftn" -type "string" "plat_wood.png";
createNode place2dTexture -n "corridor1:cube_0_5:lambert2SG1P2D";
createNode polyCube -n "corridor1:polyCube3";
	setAttr ".ax" -type "double3" 0 1.0000000000000002 2.2204460492503131e-016 ;
	setAttr ".w" 14.436339730031193;
	setAttr ".h" 24.430728773899034;
	setAttr ".d" 79.955112350942301;
	setAttr ".cuv" 4;
createNode polySplitRing -n "corridor1:polySplitRing10";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 2 "e[6:7]" "e[10:11]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1.0000000000000004 0 0 0 0 1 0 352.98321714906137 36.259267847276355 157.75470952471883 1;
	setAttr ".wt" 0.091514147818088531;
	setAttr ".re" 7;
	setAttr ".sma" 29.999999999999996;
	setAttr ".p[0]"  0 0 1;
	setAttr ".fq" yes;
createNode polySplitRing -n "corridor1:polySplitRing11";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "e[10:13]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1.0000000000000004 0 0 0 0 1 0 352.98321714906137 36.259267847276355 157.75470952471883 1;
	setAttr ".wt" 0.86847269535064697;
	setAttr ".dr" no;
	setAttr ".re" 12;
	setAttr ".sma" 29.999999999999996;
	setAttr ".p[0]"  0 0 1;
	setAttr ".fq" yes;
createNode polyExtrudeFace -n "corridor1:polyExtrudeFace7";
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
createNode polyExtrudeFace -n "corridor1:polyExtrudeFace8";
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
createNode lambert -n "corridor1:lambert2";
createNode shadingEngine -n "corridor1:lambert2SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "corridor1:materialInfo1";
createNode file -n "corridor1:file1";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//ground2.png";
createNode place2dTexture -n "corridor1:place2dTexture1";
createNode lambert -n "corridor1:white_wood";
createNode shadingEngine -n "corridor1:lambert3SG";
	setAttr ".ihi" 0;
	setAttr -s 4 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 4 ".gn";
createNode materialInfo -n "corridor1:materialInfo2";
createNode file -n "corridor1:file2";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//white-wood.png";
createNode place2dTexture -n "corridor1:place2dTexture2";
createNode polyTriangulate -n "corridor1:polyTriangulate3";
	setAttr ".ics" -type "componentList" 1 "f[*]";
createNode polyTweak -n "corridor1:polyTweak7";
	setAttr ".uopa" yes;
	setAttr -s 8 ".tk[0:7]" -type "float3"  0 5.4836273e-006 0 -4.24304199
		 5.4836273e-006 0 0 -16.26499557 0 -4.24304199 -16.26499557 0 0 -16.26499557 0 -4.24304199
		 -16.26499557 0 0 5.4836273e-006 0 -4.24304199 5.4836273e-006 0;
createNode polyTriangulate -n "corridor1:polyTriangulate4";
	setAttr ".ics" -type "componentList" 1 "f[*]";
createNode polyTweak -n "corridor1:polyTweak8";
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
createNode groupId -n "corridor1:groupId1";
	setAttr ".ihi" 0;
createNode groupParts -n "corridor1:groupParts1";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:11]";
createNode groupId -n "corridor1:groupId2";
	setAttr ".ihi" 0;
createNode groupId -n "corridor1:groupId3";
	setAttr ".ihi" 0;
createNode groupParts -n "corridor1:groupParts2";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:43]";
createNode groupId -n "corridor1:groupId4";
	setAttr ".ihi" 0;
createNode lambert -n "corridor1:wood";
createNode shadingEngine -n "corridor1:lambert4SG";
	setAttr ".ihi" 0;
	setAttr -s 4 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 4 ".gn";
createNode materialInfo -n "corridor1:materialInfo3";
createNode file -n "corridor1:file3";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//wood1.png";
createNode place2dTexture -n "corridor1:place2dTexture3";
createNode file -n "corridor1:file4";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//wood2.png";
createNode place2dTexture -n "corridor1:place2dTexture4";
createNode polySeparate -n "corridor1:polySeparate1";
	setAttr ".ic" 2;
	setAttr -s 2 ".out";
createNode groupId -n "corridor1:groupId6";
	setAttr ".ihi" 0;
createNode groupId -n "corridor1:groupId7";
	setAttr ".ihi" 0;
createNode groupId -n "corridor1:groupId8";
	setAttr ".ihi" 0;
createNode groupParts -n "corridor1:groupParts4";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:11]";
createNode groupId -n "corridor1:groupId9";
	setAttr ".ihi" 0;
createNode groupParts -n "corridor1:groupParts5";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:43]";
createNode polyCube -n "corridor1:polyCube4";
	setAttr ".w" 52.377931334530274;
	setAttr ".h" 52.721339558123091;
	setAttr ".d" 63.965261523959782;
	setAttr ".cuv" 4;
createNode deleteComponent -n "corridor1:deleteComponent11";
	setAttr ".dc" -type "componentList" 2 "f[0:3]" "f[5]";
createNode lambert -n "corridor1:lambert5";
createNode shadingEngine -n "corridor1:lambert5SG";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 2 ".gn";
createNode materialInfo -n "corridor1:materialInfo4";
createNode file -n "corridor1:blackboard";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//blackboard.png";
createNode place2dTexture -n "corridor1:place2dTexture5";
createNode polyTweakUV -n "corridor1:polyTweakUV3";
	setAttr ".uopa" yes;
	setAttr -s 4 ".uvtk[0:3]" -type "float2" -0.62306297 0.0013405979
		 -0.62306297 0.74447477 0.12007117 0.0013405979 0.12007117 0.74447477;
createNode polyTweakUV -n "corridor1:polyTweakUV4";
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
createNode lambert -n "corridor1:lambert6";
createNode shadingEngine -n "corridor1:lambert6SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "corridor1:materialInfo5";
createNode file -n "corridor1:file5";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//floor1.png";
createNode place2dTexture -n "corridor1:place2dTexture6";
createNode groupId -n "corridor1:groupId10";
	setAttr ".ihi" 0;
createNode groupParts -n "corridor1:groupParts6";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0]";
createNode groupId -n "corridor1:groupId11";
	setAttr ".ihi" 0;
createNode polyCube -n "corridor1:polyCube6";
	setAttr ".w" 48.351192852466269;
	setAttr ".h" 2.3175894449114622;
	setAttr ".d" 37.553502228555544;
	setAttr ".cuv" 4;
createNode polyCylinder -n "corridor1:polyCylinder1";
	setAttr ".r" 18.261126462257909;
	setAttr ".h" 83.352394713467135;
	setAttr ".sa" 3;
	setAttr ".sc" 1;
	setAttr ".cuv" 3;
createNode deleteComponent -n "corridor1:deleteComponent13";
	setAttr ".dc" -type "componentList" 0;
createNode deleteComponent -n "corridor1:deleteComponent14";
	setAttr ".dc" -type "componentList" 1 "e[12:14]";
createNode deleteComponent -n "corridor1:deleteComponent15";
	setAttr ".dc" -type "componentList" 1 "e[9:11]";
createNode lambert -n "corridor1:desk";
createNode shadingEngine -n "corridor1:lambert7SG";
	setAttr ".ihi" 0;
	setAttr -s 12 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 12 ".gn";
createNode materialInfo -n "corridor1:materialInfo6";
createNode file -n "corridor1:file6";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//desk1.png";
createNode place2dTexture -n "corridor1:place2dTexture7";
createNode polyTweakUV -n "corridor1:polyTweakUV6";
	setAttr ".uopa" yes;
	setAttr -s 14 ".uvtk[0:13]" -type "float2" 0.45413852 0.0045094565 0.45413852
		 -0.23319337 0.25421929 -0.11434197 0.49412239 -0.25157976 0.42304003 -0.25157976
		 0.35195759 -0.25157976 0.28087518 -0.25157976 0.49412239 -0.5817759 0.42304003 -0.5817759
		 0.35195759 -0.5817759 0.28087518 -0.5817759 0.45413852 -0.59933674 0.45413852 -0.83703959
		 0.25421929 -0.71818817;
createNode polyTweakUV -n "corridor1:polyTweakUV7";
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
createNode polyMapCut -n "corridor1:polyMapCut1";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 3 "e[0]" "e[3]" "e[8:11]";
createNode polyTweakUV -n "corridor1:polyTweakUV8";
	setAttr ".uopa" yes;
	setAttr -s 6 ".uvtk";
	setAttr ".uvtk[6]" -type "float2" 0.0070157135 0.042094216 ;
	setAttr ".uvtk[7]" -type "float2" 0.0070156837 0.042094216 ;
	setAttr ".uvtk[8]" -type "float2" 0.0070157135 0.042094216 ;
	setAttr ".uvtk[9]" -type "float2" 0.0070156837 0.042094216 ;
	setAttr ".uvtk[14]" -type "float2" 0.0070156837 0.042094216 ;
	setAttr ".uvtk[15]" -type "float2" 0.0070157135 0.042094216 ;
createNode polyMapCut -n "corridor1:polyMapCut2";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 3 "e[0]" "e[3]" "e[10:11]";
createNode polyTweakUV -n "corridor1:polyTweakUV9";
	setAttr ".uopa" yes;
	setAttr -s 4 ".uvtk";
	setAttr ".uvtk[6]" -type "float2" 0.53699183 -0.61163986 ;
	setAttr ".uvtk[8]" -type "float2" 0.5880996 -0.65827382 ;
	setAttr ".uvtk[9]" -type "float2" 0.34698686 -0.79617792 ;
	setAttr ".uvtk[14]" -type "float2" 0.29587904 -0.74954402 ;
createNode polyMapCut -n "corridor1:polyMapCut3";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 2 "e[2:3]" "e[8:9]";
createNode polyTweakUV -n "corridor1:polyTweakUV10";
	setAttr ".uopa" yes;
	setAttr -s 4 ".uvtk";
	setAttr ".uvtk[4]" -type "float2" -0.32694626 0.037740588 ;
	setAttr ".uvtk[7]" -type "float2" 0.30356055 -0.037740529 ;
	setAttr ".uvtk[15]" -type "float2" -0.32110286 -0.037740529 ;
	setAttr ".uvtk[16]" -type "float2" 0.27434403 0.032142699 ;
createNode polyMapCut -n "corridor1:polyMapCut4";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 2 "e[1:2]" "e[6:7]";
createNode polyTweakUV -n "corridor1:polyTweakUV11";
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
createNode groupId -n "corridor1:groupId14";
	setAttr ".ihi" 0;
createNode groupParts -n "corridor1:groupParts9";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:5]";
createNode groupId -n "corridor1:groupId15";
	setAttr ".ihi" 0;
createNode groupId -n "corridor1:groupId16";
	setAttr ".ihi" 0;
createNode groupId -n "corridor1:groupId17";
	setAttr ".ihi" 0;
createNode groupId -n "corridor1:groupId18";
	setAttr ".ihi" 0;
createNode groupParts -n "corridor1:groupParts10";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:4]";
createNode groupId -n "corridor1:groupId19";
	setAttr ".ihi" 0;
createNode groupId -n "corridor1:groupId20";
	setAttr ".ihi" 0;
createNode groupId -n "corridor1:groupId21";
	setAttr ".ihi" 0;
createNode groupId -n "corridor1:groupId22";
	setAttr ".ihi" 0;
createNode groupId -n "corridor1:groupId23";
	setAttr ".ihi" 0;
createNode groupId -n "corridor1:groupId24";
	setAttr ".ihi" 0;
createNode groupId -n "corridor1:groupId25";
	setAttr ".ihi" 0;
createNode file -n "corridor1:file7";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//darkness.png";
createNode place2dTexture -n "corridor1:place2dTexture8";
createNode lambert -n "corridor1:door_red";
createNode shadingEngine -n "corridor1:lambert8SG";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 2 ".gn";
createNode materialInfo -n "corridor1:materialInfo7";
createNode file -n "corridor1:file8";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//door-red.png";
createNode place2dTexture -n "corridor1:place2dTexture9";
createNode polyTweakUV -n "corridor1:polyTweakUV12";
	setAttr ".uopa" yes;
	setAttr -s 14 ".uvtk[0:13]" -type "float2" -1.37935078 0.0019998881
		 -0.63036335 0.0019998881 -1.37935078 0.72434902 -0.63036335 0.72434902 -1.37935078
		 1.44669783 -0.63036335 1.44669783 -1.37935078 2.16904712 -0.63036335 2.16904712 -1.37935078
		 2.89139605 -0.63036335 2.89139605 0.11862427 0.0019998881 0.11862427 0.72434902 -2.12833858
		 0.0019998881 -2.12833858 0.72434902;
createNode polyCube -n "corridor1:polyCube8";
	setAttr ".w" 20.47250628605417;
	setAttr ".h" 115.10273231033884;
	setAttr ".d" 84.339152203941993;
	setAttr ".cuv" 4;
createNode polyTriangulate -n "corridor1:polyTriangulate6";
	setAttr ".ics" -type "componentList" 1 "f[*]";
createNode polyTweak -n "corridor1:polyTweak9";
	setAttr ".uopa" yes;
	setAttr -s 6 ".tk[2:7]" -type "float3"  1.7763568e-015 -33.83877945
		 0 0 -33.61052704 0 1.7763568e-015 -33.83874893 46.69451523 0 -33.61052704 46.69451523
		 0 0 46.69451523 0 0 46.69451523;
createNode polyCube -n "corridor1:polyCube9";
	setAttr ".w" 51.959334544592764;
	setAttr ".h" 31.193170399534246;
	setAttr ".d" 18.833748192061357;
	setAttr ".cuv" 4;
createNode groupId -n "corridor1:groupId26";
	setAttr ".ihi" 0;
createNode groupParts -n "corridor1:groupParts11";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:11]";
createNode groupId -n "corridor1:groupId27";
	setAttr ".ihi" 0;
createNode groupId -n "corridor1:groupId28";
	setAttr ".ihi" 0;
createNode groupParts -n "corridor1:groupParts12";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:5]";
createNode groupId -n "corridor1:groupId29";
	setAttr ".ihi" 0;
createNode lambert -n "corridor1:lambert9";
createNode shadingEngine -n "corridor1:lambert9SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "corridor1:materialInfo8";
createNode file -n "corridor1:file9";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//window.png";
createNode place2dTexture -n "corridor1:place2dTexture10";
createNode polyPlanarProj -n "polyPlanarProj1";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:9]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 1.5012295461640917 12.381142450336389 2.7753959689401801 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 0 6.8496980667114258 0 ;
	setAttr ".ps" -type "double2" 3.6958155632019043 13.699396133422852 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode polyPlanarProj -n "polyPlanarProj2";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:9]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 1.5111019251531204 12.338589258847028 -3.0851063829787559 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 0 6.8496980667114258 0 ;
	setAttr ".ps" -type "double2" 3.6958155632019043 13.699396133422852 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode polyPlanarProj -n "polyPlanarProj3";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:9]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 1.5012295461640917 6.1036824445683138 2.7604841142114687 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 0 6.8496980667114258 0 ;
	setAttr ".ps" -type "double2" 3.6958155632019043 13.699396133422852 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode polyPlanarProj -n "polyPlanarProj4";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:9]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 1.501229546164093 6.1400098520279993 -3.05176732856531 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 0 6.8496980667114258 0 ;
	setAttr ".ps" -type "double2" 3.6958155632019043 13.699396133422852 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode polyPlanarProj -n "polyPlanarProj5";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:3]";
	setAttr ".ix" -type "matrix" 0.023903923140400418 0 0 0 0 2.2204460492503131e-016 1 0
		 0 -0.020731640579670606 4.6033489419607067e-018 0 1.5012463226046449 6.0714979482364786 -0.14187592375491437 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 0 6.8496980667114258 0 ;
	setAttr ".ps" -type "double2" 3.6958155632019043 13.699396133422852 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode polyPlanarProj -n "polyPlanarProj6";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:3]";
	setAttr ".ix" -type "matrix" 0.023903923140400418 0 0 0 0 2.2204460492503131e-016 1 0
		 0 -0.020731640579670606 4.6033489419607067e-018 0 1.5012463226046453 12.363993344939725 -0.14187592375491792 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 0 6.8496980667114258 0 ;
	setAttr ".ps" -type "double2" 3.6958155632019043 13.699396133422852 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode polyPlanarProj -n "polyPlanarProj7";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:3]";
	setAttr ".ix" -type "matrix" 0.023903923140400418 0 0 0 0 1.1372790934155366 0 0
		 0 0 0.020731640579670606 0 1.5012463226046453 9.2424608903626613 2.704023766286908 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 0 6.8496980667114258 0 ;
	setAttr ".ps" -type "double2" 3.6958155632019043 13.699396133422852 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode polyPlanarProj -n "polyPlanarProj8";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:3]";
	setAttr ".ix" -type "matrix" 0.023903923140400418 0 0 0 0 1.1544029511168992 0 0
		 0 0 0.020731640579670606 0 1.5012463226046455 9.2237667548519546 -3.0157789169265583 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 0 6.8496980667114258 0 ;
	setAttr ".ps" -type "double2" 3.6958155632019043 13.699396133422852 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode polyPlanarProj -n "polyPlanarProj9";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:9]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 5.93343044784155 0 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 0 6.8496980667114258 0 ;
	setAttr ".ps" -type "double2" 3.6958155632019043 13.699396133422852 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode polyTweak -n "polyTweak2";
	setAttr ".uopa" yes;
	setAttr -s 8 ".tk";
	setAttr ".tk[2]" -type "float3" 0 1.8325353 0 ;
	setAttr ".tk[3]" -type "float3" 0 1.8325353 0 ;
	setAttr ".tk[4]" -type "float3" 0 1.8325353 0 ;
	setAttr ".tk[5]" -type "float3" 0 1.8325353 0 ;
	setAttr ".tk[8]" -type "float3" 0 1.8325353 0 ;
	setAttr ".tk[9]" -type "float3" 0 1.8325353 0 ;
	setAttr ".tk[10]" -type "float3" 0 1.8325353 0 ;
	setAttr ".tk[11]" -type "float3" 0 1.8325353 0 ;
createNode polyPlanarProj -n "polyPlanarProj10";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:9]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 1.5012295461640917 12.381142450336389 2.7753959689401801 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 0 6.8496980667114258 0 ;
	setAttr ".ro" -type "double3" 0 90 0 ;
	setAttr ".ps" -type "double2" 7.7288632392883301 13.699396133422852 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode polyPlanarProj -n "polyPlanarProj11";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:9]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 1.5111019251531204 12.338589258847028 -3.0851063829787559 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 0 6.8496980667114258 0 ;
	setAttr ".ro" -type "double3" 0 90 0 ;
	setAttr ".ps" -type "double2" 7.7288632392883301 13.699396133422852 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode polyPlanarProj -n "polyPlanarProj12";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:9]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 1.5012295461640917 6.1036824445683138 2.7604841142114687 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 0 6.8496980667114258 0 ;
	setAttr ".ro" -type "double3" 0 90 0 ;
	setAttr ".ps" -type "double2" 7.7288632392883301 13.699396133422852 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode polyPlanarProj -n "polyPlanarProj13";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:9]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 1.501229546164093 6.1400098520279993 -3.05176732856531 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 0 6.8496980667114258 0 ;
	setAttr ".ro" -type "double3" 0 90 0 ;
	setAttr ".ps" -type "double2" 7.7288632392883301 13.699396133422852 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode polyPlanarProj -n "polyPlanarProj14";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:3]";
	setAttr ".ix" -type "matrix" 0.023903923140400418 0 0 0 0 2.2204460492503131e-016 1 0
		 0 -0.020731640579670606 4.6033489419607067e-018 0 1.5012463226046449 6.0714979482364786 -0.14187592375491437 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 0 6.8496980667114258 0 ;
	setAttr ".ro" -type "double3" 0 90 0 ;
	setAttr ".ps" -type "double2" 7.7288632392883301 13.699396133422852 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode polyPlanarProj -n "polyPlanarProj15";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:3]";
	setAttr ".ix" -type "matrix" 0.023903923140400418 0 0 0 0 2.2204460492503131e-016 1 0
		 0 -0.020731640579670606 4.6033489419607067e-018 0 1.5012463226046453 12.363993344939725 -0.14187592375491792 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 0 6.8496980667114258 0 ;
	setAttr ".ro" -type "double3" 0 90 0 ;
	setAttr ".ps" -type "double2" 7.7288632392883301 13.699396133422852 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode polyPlanarProj -n "polyPlanarProj16";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:3]";
	setAttr ".ix" -type "matrix" 0.023903923140400418 0 0 0 0 1.1372790934155366 0 0
		 0 0 0.020731640579670606 0 1.5012463226046453 9.2424608903626613 2.704023766286908 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 0 6.8496980667114258 0 ;
	setAttr ".ro" -type "double3" 0 90 0 ;
	setAttr ".ps" -type "double2" 7.7288632392883301 13.699396133422852 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode polyPlanarProj -n "polyPlanarProj17";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:3]";
	setAttr ".ix" -type "matrix" 0.023903923140400418 0 0 0 0 1.1544029511168992 0 0
		 0 0 0.020731640579670606 0 1.5012463226046455 9.2237667548519546 -3.0157789169265583 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 0 6.8496980667114258 0 ;
	setAttr ".ro" -type "double3" 0 90 0 ;
	setAttr ".ps" -type "double2" 7.7288632392883301 13.699396133422852 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode polyPlanarProj -n "polyPlanarProj18";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:9]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 5.93343044784155 0 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 0 6.8496980667114258 0 ;
	setAttr ".ro" -type "double3" 0 90 0 ;
	setAttr ".ps" -type "double2" 7.7288632392883301 13.699396133422852 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode polyTweakUV -n "polyTweakUV1";
	setAttr ".uopa" yes;
	setAttr -s 18 ".uvtk[0:17]" -type "float2" -0.14653239 -0.099588752
		 -0.11384988 -0.094920993 -0.11586171 -0.093185425 -0.15035912 -0.098112345 -0.17246935
		 -0.10685873 -0.17773643 -0.10578614 -0.18912199 -0.11601949 -0.19531369 -0.11545563
		 -0.19486004 -0.12617433 -0.20137042 -0.12617433 -0.077621043 -0.093312621 -0.077621043
		 -0.091487885 -0.11787349 -0.091449976 -0.1541858 -0.096635997 -0.1830034 -0.10471356
		 -0.20150545 -0.11489165 -0.20788074 -0.12617433 -0.077621043 -0.089663029;
createNode polyTweakUV -n "polyTweakUV2";
	setAttr ".uopa" yes;
	setAttr -s 18 ".uvtk[0:17]" -type "float2" -0.44304517 -0.11504877 -0.43730709
		 -0.12520367 -0.43079671 -0.12520367 -0.4368535 -0.11448497 -0.45969769 -0.10588807
		 -0.45443079 -0.10481548 -0.48563471 -0.09861809 -0.48180804 -0.097141743 -0.51831734
		 -0.093950391 -0.51630545 -0.092214942 -0.554546 -0.092342019 -0.554546 -0.090517163
		 -0.42428634 -0.12520367 -0.43066171 -0.11392105 -0.44916382 -0.1037429 -0.4779813
		 -0.095665336 -0.51429355 -0.090479314 -0.554546 -0.088692307;
createNode polyTweakUV -n "polyTweakUV3";
	setAttr ".uopa" yes;
	setAttr -s 18 ".uvtk[0:17]" -type "float2" -0.19033548 0.0068620741
		 -0.19607353 0.017016888 -0.20258391 0.017016888 -0.19652721 0.0062981844 -0.17368287
		 -0.0022987723 -0.17894986 -0.0033713579 -0.14774588 -0.0095687509 -0.15157261 -0.011044979
		 -0.11506343 -0.014236331 -0.11707518 -0.015971959 -0.078834534 -0.015844703 -0.078834534
		 -0.017669559 -0.20909429 0.017016888 -0.20271894 0.0057342649 -0.18421692 -0.0044439435
		 -0.15539932 -0.012521386 -0.11908704 -0.017707407 -0.078834534 -0.019494414;
createNode polyTweakUV -n "polyTweakUV4";
	setAttr ".uopa" yes;
	setAttr -s 18 ".uvtk[0:17]" -type "float2" -0.51560414 -0.015065014
		 -0.55183291 -0.016673386 -0.55183291 -0.018498242 -0.51359236 -0.016800582 -0.48292163
		 -0.010397434 -0.47909489 -0.011873662 -0.45698455 -0.0031273365 -0.45171764 -0.0041999221
		 -0.44033208 0.0060334206 -0.43414035 0.0054695308 -0.43459395 0.016188264 -0.42808363
		 0.016188264 -0.55183291 -0.020323098 -0.51158047 -0.018536091 -0.47526827 -0.01335001
		 -0.44645062 -0.0052726269 -0.42794856 0.0049056411 -0.42157325 0.016188264;
createNode polyTweakUV -n "polyTweakUV5";
	setAttr ".uopa" yes;
	setAttr -s 8 ".uvtk[0:7]" -type "float2" -0.43541709 0.019536734 -0.43541709
		 0.017751008 -0.19463751 0.017751008 -0.19463751 0.019536734 -0.43541709 0.015965313
		 -0.19463751 0.015965313 -0.43541709 0.017751008 -0.19463751 0.017751008;
createNode polyTweakUV -n "polyTweakUV6";
	setAttr ".uopa" yes;
	setAttr -s 8 ".uvtk[0:7]" -type "float2" -0.43541709 -0.12399733 -0.43541709
		 -0.12578309 -0.19463751 -0.12578309 -0.19463751 -0.12399733 -0.43541709 -0.12756878
		 -0.19463751 -0.12756878 -0.43541709 -0.12578309 -0.19463751 -0.12578309;
createNode polyTweakUV -n "polyTweakUV7";
	setAttr ".uopa" yes;
	setAttr -s 8 ".uvtk[0:7]" -type "float2" -0.077058464 -0.016202569
		 -0.083429247 -0.016202569 -0.083429247 -0.092957079 -0.077058464 -0.092957079 -0.08979997
		 -0.016202569 -0.08979997 -0.092957079 -0.083429247 -0.016202569 -0.083429247 -0.092957079;
createNode polyTweakUV -n "polyTweakUV8";
	setAttr ".uopa" yes;
	setAttr -s 8 ".uvtk[0:7]" -type "float2" -0.54253352 -0.01519829 -0.5489043
		 -0.01519829 -0.5489043 -0.093108416 -0.54253352 -0.093108416 -0.55527496 -0.01519829
		 -0.55527496 -0.093108416 -0.5489043 -0.01519829 -0.5489043 -0.093108416;
createNode polyTweakUV -n "polyTweakUV9";
	setAttr ".uopa" yes;
	setAttr -s 12 ".uvtk[0:11]" -type "float2" -0.018653065 0.15624413 -0.018653065
		 0.15624413 -0.018653065 -0.13676387 -0.018653065 -0.13676387 0.01100421 -0.1562441
		 0.01100421 -0.1562441 -0.61796713 -0.1562441 -0.61796713 -0.1562441 -0.58831 -0.13676387
		 -0.58831 -0.13676387 -0.58831 0.15624413 -0.58831 0.15624413;
createNode lambert -n "piano";
createNode shadingEngine -n "lambert2SG";
	setAttr ".ihi" 0;
	setAttr -s 50 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 50 ".gn";
createNode materialInfo -n "materialInfo1";
createNode file -n "file1";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//piano1.png";
createNode place2dTexture -n "place2dTexture1";
createNode polyAutoProj -n "polyAutoProj1";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:5]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 2.6459281617788157 3.3178599443624859 0 1;
	setAttr ".s" -type "double3" 7 7 7 ;
	setAttr ".ps" 0.20000000298023224;
	setAttr ".dl" yes;
createNode polyPlanarProj -n "polyPlanarProj19";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:5]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 2.6459281617788157 3.3178599443624859 0 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 2.6459282636642456 3.3178598880767822 0 ;
	setAttr ".ro" -type "double3" 0 90 0 ;
	setAttr ".ps" -type "double2" 7 0.5 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode polyTweakUV -n "polyTweakUV10";
	setAttr ".uopa" yes;
	setAttr -s 8 ".uvtk[0:7]" -type "float2" 0.13805932 0.58790076 0.13805932
		 0.58790076 0.13805932 -0.21339171 0.13805932 -0.21339171 -0.66323304 -0.21339171
		 -0.66323304 -0.21339171 -0.66323304 0.58790076 -0.66323304 0.58790076;
createNode polyPlanarProj -n "polyPlanarProj20";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:47]";
	setAttr ".ix" -type "matrix" 0.54968368763934716 0 0 0 0 2.4032041815852758e-017 0.10823069456682666 0
		 0 -0.65159827125284098 1.4468388071017046e-016 0 2.7059602210314595 3.5128703083879129 -3.1121648130643242 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 2.6831865906715393 4.3542065620422363 -3.1121647357940674 ;
	setAttr ".ps" -type "double2" 2.8845289945602417 1.6826725006103516 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode polyTweakUV -n "polyTweakUV11";
	setAttr ".uopa" yes;
	setAttr -s 36 ".uvtk[0:35]" -type "float2" -0.58372992 -0.049681902
		 -0.5776245 -0.13332072 -0.5776245 -0.13332072 -0.58372992 -0.049681902 -0.5675143
		 -0.21534061 -0.5675143 -0.21534061 -0.55349654 -0.29495174 -0.55349654 -0.29495174
		 -0.53570628 -0.3713876 -0.53570628 -0.3713876 -0.51431477 -0.44391185 -0.51431477
		 -0.44391185 -0.48952806 -0.51182622 -0.48952806 -0.51182622 -0.46158499 -0.5744766
		 -0.46158499 -0.5744766 -0.43075442 -0.6312595 -0.43075442 -0.6312595 -0.3973335 -0.68162829
		 -0.3973335 -0.68162829 -0.36164403 -0.72509754 -0.36164403 -0.72509754 -0.32402956
		 -0.76124907 -0.32402956 -0.76124907 -0.2848525 -0.78973418 -0.2848525 -0.78973418
		 -0.24449016 -0.81027955 -0.24449016 -0.81027955 -0.20333119 -0.82268602 -0.20333119
		 -0.82268602 0.27583298 -0.82683492 0.27583298 -0.82683492 -0.58577162 0.034769803
		 -0.58577162 0.034769803 0.2758331 0.034769803 0.2758331 0.034769803;
createNode polyPlanarProj -n "polyPlanarProj21";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:47]";
	setAttr ".ix" -type "matrix" 0.54968368763934716 0 0 0 0 2.4032041815852758e-017 0.10823069456682666 0
		 0 -0.65159827125284098 1.4468388071017046e-016 0 2.7059602210314595 3.5128703083879134 3.093470677553479 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 2.6831865906715393 4.3542065620422363 3.093470573425293 ;
	setAttr ".ps" -type "double2" 2.8845289945602417 1.6826725006103516 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode polyTweak -n "polyTweak3";
	setAttr ".uopa" yes;
	setAttr -s 4 ".tk";
	setAttr ".tk[15]" -type "float3" -2.6652386 -1.7763568e-015 0 ;
	setAttr ".tk[32]" -type "float3" -2.6652386 -1.7763568e-015 0 ;
	setAttr ".tk[34]" -type "float3" -2.6652386 -1.7763568e-015 -6.8415176e-032 ;
	setAttr ".tk[35]" -type "float3" -2.6652386 -1.7763568e-015 -6.8415176e-032 ;
createNode polyTweakUV -n "polyTweakUV12";
	setAttr ".uopa" yes;
	setAttr -s 36 ".uvtk[0:35]" -type "float2" -0.82481033 -0.06605953 -0.81876594
		 -0.14886194 -0.81876594 -0.14886194 -0.82481033 -0.06605953 -0.80875689 -0.23006165
		 -0.80875689 -0.23006165 -0.79487932 -0.30887666 -0.79487932 -0.30887666 -0.77726692
		 -0.38454819 -0.77726692 -0.38454819 -0.75608933 -0.45634717 -0.75608933 -0.45634717
		 -0.73155051 -0.5235824 -0.73155051 -0.5235824 -0.70388681 -0.58560628 -0.70388681
		 -0.58560628 -0.67336458 -0.64182132 -0.67336458 -0.64182132 -0.64027786 -0.69168645
		 -0.64027786 -0.69168645 -0.60494524 -0.734721 -0.60494524 -0.734721 -0.567707 -0.77051103
		 -0.567707 -0.77051103 -0.52892172 -0.7987113 -0.52892172 -0.7987113 -0.48896295 -0.81905121
		 -0.48896295 -0.81905121 -0.4482156 -0.83133358 -0.4482156 -0.83133358 0.026156962
		 -0.83544105 0.026156962 -0.83544105 -0.82683164 0.017547667 -0.82683164 0.017547667
		 0.026157081 0.017547667 0.026157081 0.017547667;
createNode polyTweakUV -n "polyTweakUV13";
	setAttr ".uopa" yes;
	setAttr -s 14 ".uvtk[0:13]" -type "float2" 0.32614845 0.032554299 0.1516735
		 0.032554299 0.32614845 -0.14192066 0.1516735 -0.14192066 0.32614845 -0.31639561 0.1516735
		 -0.31639561 0.32614845 -0.49087057 0.1516735 -0.49087057 0.32614845 -0.66534555 0.1516735
		 -0.66534555 -0.022801474 0.032554299 -0.022801474 -0.14192066 0.5006234 0.032554299
		 0.5006234 -0.14192066;
createNode groupId -n "groupId5";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts4";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:9]";
createNode groupId -n "groupId6";
	setAttr ".ihi" 0;
createNode groupId -n "groupId7";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts5";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:47]";
createNode groupId -n "groupId8";
	setAttr ".ihi" 0;
createNode groupId -n "groupId9";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts6";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:5]";
createNode groupId -n "groupId10";
	setAttr ".ihi" 0;
createNode groupId -n "groupId11";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts7";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:47]";
createNode groupId -n "groupId12";
	setAttr ".ihi" 0;
createNode groupId -n "groupId13";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts8";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:9]";
createNode groupId -n "groupId14";
	setAttr ".ihi" 0;
createNode groupId -n "groupId15";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts9";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:9]";
createNode groupId -n "groupId16";
	setAttr ".ihi" 0;
createNode groupId -n "groupId17";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts10";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:9]";
createNode groupId -n "groupId18";
	setAttr ".ihi" 0;
createNode groupId -n "groupId19";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts11";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:9]";
createNode groupId -n "groupId20";
	setAttr ".ihi" 0;
createNode groupId -n "groupId21";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts12";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:3]";
createNode groupId -n "groupId22";
	setAttr ".ihi" 0;
createNode groupId -n "groupId23";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts13";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:3]";
createNode groupId -n "groupId24";
	setAttr ".ihi" 0;
createNode groupId -n "groupId25";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts14";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:3]";
createNode groupId -n "groupId26";
	setAttr ".ihi" 0;
createNode groupId -n "groupId27";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts15";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:3]";
createNode groupId -n "groupId28";
	setAttr ".ihi" 0;
createNode groupId -n "groupId29";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts16";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:5]";
createNode groupId -n "groupId30";
	setAttr ".ihi" 0;
createNode groupId -n "groupId31";
	setAttr ".ihi" 0;
createNode groupId -n "groupId32";
	setAttr ".ihi" 0;
createNode groupId -n "groupId33";
	setAttr ".ihi" 0;
createNode groupId -n "groupId34";
	setAttr ".ihi" 0;
createNode groupId -n "groupId35";
	setAttr ".ihi" 0;
createNode groupId -n "groupId36";
	setAttr ".ihi" 0;
createNode groupId -n "groupId37";
	setAttr ".ihi" 0;
createNode groupId -n "groupId38";
	setAttr ".ihi" 0;
createNode groupId -n "groupId39";
	setAttr ".ihi" 0;
createNode groupId -n "groupId40";
	setAttr ".ihi" 0;
createNode groupId -n "groupId41";
	setAttr ".ihi" 0;
createNode groupId -n "groupId42";
	setAttr ".ihi" 0;
createNode groupId -n "groupId43";
	setAttr ".ihi" 0;
createNode groupId -n "groupId44";
	setAttr ".ihi" 0;
createNode groupId -n "groupId45";
	setAttr ".ihi" 0;
createNode groupId -n "groupId46";
	setAttr ".ihi" 0;
createNode groupId -n "groupId47";
	setAttr ".ihi" 0;
createNode groupId -n "groupId48";
	setAttr ".ihi" 0;
createNode groupId -n "groupId49";
	setAttr ".ihi" 0;
createNode groupId -n "groupId50";
	setAttr ".ihi" 0;
createNode groupId -n "groupId51";
	setAttr ".ihi" 0;
createNode groupId -n "groupId52";
	setAttr ".ihi" 0;
createNode groupId -n "groupId53";
	setAttr ".ihi" 0;
createNode groupId -n "groupId54";
	setAttr ".ihi" 0;
createNode shadingEngine -n "door1:lambert8SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "door1:materialInfo1";
createNode lambert -n "door1:lambert8SG1";
createNode file -n "door1:lambert8SG1F";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//gold-frame.png";
createNode place2dTexture -n "door1:lambert8SG1P2D";
createNode shadingEngine -n "door1:initialShadingGroup";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "door1:materialInfo2";
createNode lambert -n "door1:initialShadingGroup1";
createNode file -n "door1:initialShadingGroup1F";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/obj/darkness.png";
createNode place2dTexture -n "door1:initialShadingGroup1P2D";
createNode shadingEngine -n "door1:lambert9SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "door1:materialInfo3";
createNode groupId -n "door1:groupId3";
	setAttr ".ihi" 0;
createNode lambert -n "door1:lambert9SG1";
createNode file -n "door1:lambert9SG1F";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//flower-border-blurred.png";
createNode place2dTexture -n "door1:lambert9SG1P2D";
createNode polyPlanarProj -n "polyPlanarProj22";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:39]";
	setAttr ".ix" -type "matrix" 0.53146142269335306 0 0 0 0 0.69849556595089912 0 0
		 0 0 0.53146142269335306 0 1.3710542375973183 -40.820027471892374 11.348697045929679 1;
	setAttr ".ws" yes;
	setAttr ".pc" -type "double3" 1.744531661272049 0.1996002197265625 0.053715229034423828 ;
	setAttr ".ro" -type "double3" 0 90 0 ;
	setAttr ".ps" -type "double2" 22.310259819030762 29.011512756347656 ;
	setAttr ".cam" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
createNode groupId -n "door1:groupId4";
	setAttr ".ihi" 0;
createNode groupParts -n "door1:groupParts1";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:27]";
createNode groupId -n "door1:groupId5";
	setAttr ".ihi" 0;
createNode groupParts -n "door1:groupParts2";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[28:39]";
createNode polyTweakUV -n "polyTweakUV14";
	setAttr ".uopa" yes;
	setAttr -s 13 ".uvtk";
	setAttr ".uvtk[3]" -type "float2" -0.12183142 -0.083171666 ;
	setAttr ".uvtk[4]" -type "float2" -0.12183142 -0.15854651 ;
	setAttr ".uvtk[6]" -type "float2" 0.1218316 -0.15854651 ;
	setAttr ".uvtk[7]" -type "float2" 0.1218316 0.17354649 ;
	setAttr ".uvtk[8]" -type "float2" -0.038161457 0.17354649 ;
	setAttr ".uvtk[12]" -type "float2" -0.12183142 0.17354649 ;
	setAttr ".uvtk[13]" -type "float2" 0.1218316 -0.077277482 ;
	setAttr ".uvtk[14]" -type "float2" 0.1218316 0.17354649 ;
	setAttr ".uvtk[15]" -type "float2" 0.036899418 0.17354649 ;
	setAttr ".uvtk[17]" -type "float2" 0.1218316 -0.15854651 ;
	setAttr ".uvtk[18]" -type "float2" -0.12183142 -0.15854651 ;
	setAttr ".uvtk[19]" -type "float2" -0.12183142 0.17354649 ;
select -ne :time1;
	setAttr ".o" 1;
	setAttr ".unw" 1;
select -ne :renderPartition;
	setAttr -s 16 ".st";
select -ne :renderGlobalsList1;
select -ne :defaultShaderList1;
	setAttr -s 16 ".s";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
	setAttr -s 16 ".u";
select -ne :defaultRenderingList1;
select -ne :defaultTextureList1;
	setAttr -s 16 ".tx";
select -ne :lambert1;
select -ne :initialShadingGroup;
	setAttr -s 6 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 6 ".gn";
select -ne :initialParticleSE;
	setAttr ".ro" yes;
select -ne :initialMaterialInfo;
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
connectAttr "groupId5.id" "pCubeShape1.iog.og[0].gid";
connectAttr "lambert2SG.mwc" "pCubeShape1.iog.og[0].gco";
connectAttr "groupParts4.og" "pCubeShape1.i";
connectAttr "polyTweakUV9.uvtk[0]" "pCubeShape1.uvst[0].uvtw";
connectAttr "groupId6.id" "pCubeShape1.ciog.cog[0].cgid";
connectAttr "groupId7.id" "pCylinderShape1.iog.og[0].gid";
connectAttr "lambert2SG.mwc" "pCylinderShape1.iog.og[0].gco";
connectAttr "groupParts5.og" "pCylinderShape1.i";
connectAttr "polyTweakUV12.uvtk[0]" "pCylinderShape1.uvst[0].uvtw";
connectAttr "groupId8.id" "pCylinderShape1.ciog.cog[0].cgid";
connectAttr "groupId9.id" "pCubeShape2.iog.og[0].gid";
connectAttr "lambert2SG.mwc" "pCubeShape2.iog.og[0].gco";
connectAttr "groupParts6.og" "pCubeShape2.i";
connectAttr "polyTweakUV10.uvtk[0]" "pCubeShape2.uvst[0].uvtw";
connectAttr "groupId10.id" "pCubeShape2.ciog.cog[0].cgid";
connectAttr "groupId11.id" "pCylinderShape2.iog.og[0].gid";
connectAttr "lambert2SG.mwc" "pCylinderShape2.iog.og[0].gco";
connectAttr "groupParts7.og" "pCylinderShape2.i";
connectAttr "polyTweakUV11.uvtk[0]" "pCylinderShape2.uvst[0].uvtw";
connectAttr "groupId12.id" "pCylinderShape2.ciog.cog[0].cgid";
connectAttr "groupParts8.og" "polySurfaceShape5.i";
connectAttr "groupId13.id" "polySurfaceShape5.iog.og[1].gid";
connectAttr "lambert2SG.mwc" "polySurfaceShape5.iog.og[1].gco";
connectAttr "polyTweakUV1.uvtk[0]" "polySurfaceShape5.uvst[0].uvtw";
connectAttr "groupId14.id" "polySurfaceShape5.ciog.cog[0].cgid";
connectAttr "groupParts9.og" "polySurfaceShape6.i";
connectAttr "groupId15.id" "polySurfaceShape6.iog.og[1].gid";
connectAttr "lambert2SG.mwc" "polySurfaceShape6.iog.og[1].gco";
connectAttr "polyTweakUV2.uvtk[0]" "polySurfaceShape6.uvst[0].uvtw";
connectAttr "groupId16.id" "polySurfaceShape6.ciog.cog[0].cgid";
connectAttr "polyChipOff3.out" "polySurfaceShape1.i";
connectAttr "groupId3.id" "polySurfaceShape1.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "polySurfaceShape1.iog.og[0].gco";
connectAttr "groupParts10.og" "polySurfaceShape3.i";
connectAttr "groupId17.id" "polySurfaceShape3.iog.og[1].gid";
connectAttr "lambert2SG.mwc" "polySurfaceShape3.iog.og[1].gco";
connectAttr "polyTweakUV3.uvtk[0]" "polySurfaceShape3.uvst[0].uvtw";
connectAttr "groupId18.id" "polySurfaceShape3.ciog.cog[0].cgid";
connectAttr "groupParts11.og" "polySurfaceShape4.i";
connectAttr "groupId19.id" "polySurfaceShape4.iog.og[1].gid";
connectAttr "lambert2SG.mwc" "polySurfaceShape4.iog.og[1].gco";
connectAttr "polyTweakUV4.uvtk[0]" "polySurfaceShape4.uvst[0].uvtw";
connectAttr "groupId20.id" "polySurfaceShape4.ciog.cog[0].cgid";
connectAttr "polyChipOff2.out" "polySurfaceShape2.i";
connectAttr "groupId4.id" "polySurfaceShape2.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "polySurfaceShape2.iog.og[0].gco";
connectAttr "groupId1.id" "pTorusShape1.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "pTorusShape1.iog.og[0].gco";
connectAttr "groupParts1.og" "pTorusShape1.i";
connectAttr "groupId2.id" "pTorusShape1.ciog.cog[0].cgid";
connectAttr "groupId21.id" "pCylinderShape3.iog.og[0].gid";
connectAttr "lambert2SG.mwc" "pCylinderShape3.iog.og[0].gco";
connectAttr "groupParts12.og" "pCylinderShape3.i";
connectAttr "polyTweakUV5.uvtk[0]" "pCylinderShape3.uvst[0].uvtw";
connectAttr "groupId22.id" "pCylinderShape3.ciog.cog[0].cgid";
connectAttr "groupId23.id" "pCylinderShape4.iog.og[0].gid";
connectAttr "lambert2SG.mwc" "pCylinderShape4.iog.og[0].gco";
connectAttr "groupParts13.og" "pCylinderShape4.i";
connectAttr "polyTweakUV6.uvtk[0]" "pCylinderShape4.uvst[0].uvtw";
connectAttr "groupId24.id" "pCylinderShape4.ciog.cog[0].cgid";
connectAttr "groupId25.id" "pCylinderShape5.iog.og[0].gid";
connectAttr "lambert2SG.mwc" "pCylinderShape5.iog.og[0].gco";
connectAttr "groupParts14.og" "pCylinderShape5.i";
connectAttr "polyTweakUV7.uvtk[0]" "pCylinderShape5.uvst[0].uvtw";
connectAttr "groupId26.id" "pCylinderShape5.ciog.cog[0].cgid";
connectAttr "groupId27.id" "pCylinderShape6.iog.og[0].gid";
connectAttr "lambert2SG.mwc" "pCylinderShape6.iog.og[0].gco";
connectAttr "groupParts15.og" "pCylinderShape6.i";
connectAttr "polyTweakUV8.uvtk[0]" "pCylinderShape6.uvst[0].uvtw";
connectAttr "groupId28.id" "pCylinderShape6.ciog.cog[0].cgid";
connectAttr "groupId29.id" "pCubeShape3.iog.og[0].gid";
connectAttr "lambert2SG.mwc" "pCubeShape3.iog.og[0].gco";
connectAttr "groupParts16.og" "pCubeShape3.i";
connectAttr "polyTweakUV13.uvtk[0]" "pCubeShape3.uvst[0].uvtw";
connectAttr "groupId30.id" "pCubeShape3.ciog.cog[0].cgid";
connectAttr "groupId31.id" "pCubeShape4.iog.og[0].gid";
connectAttr "lambert2SG.mwc" "pCubeShape4.iog.og[0].gco";
connectAttr "groupId32.id" "pCubeShape4.ciog.cog[0].cgid";
connectAttr "groupId33.id" "pCubeShape5.iog.og[0].gid";
connectAttr "lambert2SG.mwc" "pCubeShape5.iog.og[0].gco";
connectAttr "groupId34.id" "pCubeShape5.ciog.cog[0].cgid";
connectAttr "groupId35.id" "pCubeShape6.iog.og[0].gid";
connectAttr "lambert2SG.mwc" "pCubeShape6.iog.og[0].gco";
connectAttr "groupId36.id" "pCubeShape6.ciog.cog[0].cgid";
connectAttr "groupId37.id" "pCubeShape7.iog.og[0].gid";
connectAttr "lambert2SG.mwc" "pCubeShape7.iog.og[0].gco";
connectAttr "groupId38.id" "pCubeShape7.ciog.cog[0].cgid";
connectAttr "groupId39.id" "pCubeShape8.iog.og[0].gid";
connectAttr "lambert2SG.mwc" "pCubeShape8.iog.og[0].gco";
connectAttr "groupId40.id" "pCubeShape8.ciog.cog[0].cgid";
connectAttr "groupId41.id" "pCubeShape9.iog.og[0].gid";
connectAttr "lambert2SG.mwc" "pCubeShape9.iog.og[0].gco";
connectAttr "groupId42.id" "pCubeShape9.ciog.cog[0].cgid";
connectAttr "groupId43.id" "pCubeShape10.iog.og[0].gid";
connectAttr "lambert2SG.mwc" "pCubeShape10.iog.og[0].gco";
connectAttr "groupId44.id" "pCubeShape10.ciog.cog[0].cgid";
connectAttr "groupId45.id" "pCubeShape11.iog.og[0].gid";
connectAttr "lambert2SG.mwc" "pCubeShape11.iog.og[0].gco";
connectAttr "groupId46.id" "pCubeShape11.ciog.cog[0].cgid";
connectAttr "groupId47.id" "pCubeShape12.iog.og[0].gid";
connectAttr "lambert2SG.mwc" "pCubeShape12.iog.og[0].gco";
connectAttr "groupId48.id" "pCubeShape12.ciog.cog[0].cgid";
connectAttr "groupId49.id" "pCubeShape13.iog.og[0].gid";
connectAttr "lambert2SG.mwc" "pCubeShape13.iog.og[0].gco";
connectAttr "groupId50.id" "pCubeShape13.ciog.cog[0].cgid";
connectAttr "groupId51.id" "pCubeShape14.iog.og[0].gid";
connectAttr "lambert2SG.mwc" "pCubeShape14.iog.og[0].gco";
connectAttr "groupId52.id" "pCubeShape14.ciog.cog[0].cgid";
connectAttr "groupId53.id" "pCubeShape15.iog.og[0].gid";
connectAttr "lambert2SG.mwc" "pCubeShape15.iog.og[0].gco";
connectAttr "groupId54.id" "pCubeShape15.ciog.cog[0].cgid";
connectAttr "corridor1:groupId1.id" "corridor1:pCubeShape3.iog.og[0].gid";
connectAttr "corridor1:lambert3SG.mwc" "corridor1:pCubeShape3.iog.og[0].gco";
connectAttr "corridor1:groupParts1.og" "corridor1:pCubeShape3.i";
connectAttr "corridor1:groupId2.id" "corridor1:pCubeShape3.ciog.cog[0].cgid";
connectAttr "corridor1:groupId3.id" "corridor1:pCubeShape4.iog.og[0].gid";
connectAttr "corridor1:lambert3SG.mwc" "corridor1:pCubeShape4.iog.og[0].gco";
connectAttr "corridor1:groupParts2.og" "corridor1:pCubeShape4.i";
connectAttr "corridor1:groupId4.id" "corridor1:pCubeShape4.ciog.cog[0].cgid";
connectAttr "corridor1:groupParts4.og" "corridor1:polySurfaceShape2.i";
connectAttr "corridor1:groupId8.id" "corridor1:polySurfaceShape2.iog.og[0].gid";
connectAttr "corridor1:lambert4SG.mwc" "corridor1:polySurfaceShape2.iog.og[0].gco"
		;
connectAttr "corridor1:polyTweakUV4.out" "corridor1:polySurfaceShape3.i";
connectAttr "corridor1:groupId9.id" "corridor1:polySurfaceShape3.iog.og[0].gid";
connectAttr "corridor1:lambert4SG.mwc" "corridor1:polySurfaceShape3.iog.og[0].gco"
		;
connectAttr "corridor1:polyTweakUV4.uvtk[0]" "corridor1:polySurfaceShape3.uvst[0].uvtw"
		;
connectAttr "corridor1:groupId6.id" "corridor1:window_frame1Shape.iog.og[1].gid"
		;
connectAttr "corridor1:lambert4SG.mwc" "corridor1:window_frame1Shape.iog.og[1].gco"
		;
connectAttr "corridor1:groupId7.id" "corridor1:window_frame1Shape.ciog.cog[0].cgid"
		;
connectAttr "corridor1:groupId10.id" "corridor1:pCubeShape5.iog.og[0].gid";
connectAttr "corridor1:lambert5SG.mwc" "corridor1:pCubeShape5.iog.og[0].gco";
connectAttr "corridor1:groupParts6.og" "corridor1:pCubeShape5.i";
connectAttr "corridor1:polyTweakUV3.uvtk[0]" "corridor1:pCubeShape5.uvst[0].uvtw"
		;
connectAttr "corridor1:groupId11.id" "corridor1:pCubeShape5.ciog.cog[0].cgid";
connectAttr "corridor1:groupId14.id" "corridor1:pCubeShape7.iog.og[1].gid";
connectAttr "corridor1:lambert7SG.mwc" "corridor1:pCubeShape7.iog.og[1].gco";
connectAttr "corridor1:groupParts9.og" "corridor1:pCubeShape7.i";
connectAttr "corridor1:polyTweakUV11.uvtk[0]" "corridor1:pCubeShape7.uvst[0].uvtw"
		;
connectAttr "corridor1:groupId15.id" "corridor1:pCubeShape7.ciog.cog[1].cgid";
connectAttr "corridor1:groupId16.id" "corridor1:pCubeShape8.iog.og[1].gid";
connectAttr "corridor1:lambert7SG.mwc" "corridor1:pCubeShape8.iog.og[1].gco";
connectAttr "corridor1:groupId17.id" "corridor1:pCubeShape8.ciog.cog[1].cgid";
connectAttr "corridor1:groupId18.id" "corridor1:pCylinderShape1.iog.og[1].gid";
connectAttr "corridor1:lambert7SG.mwc" "corridor1:pCylinderShape1.iog.og[1].gco"
		;
connectAttr "corridor1:groupParts10.og" "corridor1:pCylinderShape1.i";
connectAttr "corridor1:polyTweakUV6.uvtk[0]" "corridor1:pCylinderShape1.uvst[0].uvtw"
		;
connectAttr "corridor1:groupId19.id" "corridor1:pCylinderShape1.ciog.cog[1].cgid"
		;
connectAttr "corridor1:groupId20.id" "corridor1:pCylinderShape2.iog.og[1].gid";
connectAttr "corridor1:lambert7SG.mwc" "corridor1:pCylinderShape2.iog.og[1].gco"
		;
connectAttr "corridor1:groupId21.id" "corridor1:pCylinderShape2.ciog.cog[1].cgid"
		;
connectAttr "corridor1:groupId22.id" "corridor1:pCylinderShape3.iog.og[1].gid";
connectAttr "corridor1:lambert7SG.mwc" "corridor1:pCylinderShape3.iog.og[1].gco"
		;
connectAttr "corridor1:groupId23.id" "corridor1:pCylinderShape3.ciog.cog[1].cgid"
		;
connectAttr "corridor1:groupId24.id" "corridor1:pCylinderShape4.iog.og[1].gid";
connectAttr "corridor1:lambert7SG.mwc" "corridor1:pCylinderShape4.iog.og[1].gco"
		;
connectAttr "corridor1:groupId25.id" "corridor1:pCylinderShape4.ciog.cog[1].cgid"
		;
connectAttr "corridor1:groupId26.id" "corridor1:pCubeShape10.iog.og[0].gid";
connectAttr "corridor1:lambert8SG.mwc" "corridor1:pCubeShape10.iog.og[0].gco";
connectAttr "corridor1:groupParts11.og" "corridor1:pCubeShape10.i";
connectAttr "corridor1:polyTweakUV12.uvtk[0]" "corridor1:pCubeShape10.uvst[0].uvtw"
		;
connectAttr "corridor1:groupId27.id" "corridor1:pCubeShape10.ciog.cog[0].cgid";
connectAttr "corridor1:groupId28.id" "corridor1:pCubeShape11.iog.og[0].gid";
connectAttr ":initialShadingGroup.mwc" "corridor1:pCubeShape11.iog.og[0].gco";
connectAttr "corridor1:groupParts12.og" "corridor1:pCubeShape11.i";
connectAttr "corridor1:groupId29.id" "corridor1:pCubeShape11.ciog.cog[0].cgid";
connectAttr "door1:groupId4.id" "door1:pCube12Shape.iog.og[0].gid";
connectAttr "door1:lambert8SG.mwc" "door1:pCube12Shape.iog.og[0].gco";
connectAttr "door1:groupId5.id" "door1:pCube12Shape.iog.og[1].gid";
connectAttr "door1:initialShadingGroup.mwc" "door1:pCube12Shape.iog.og[1].gco";
connectAttr "polyTweakUV14.out" "door1:pCube12Shape.i";
connectAttr "polyTweakUV14.uvtk[0]" "door1:pCube12Shape.uvst[0].uvtw";
connectAttr "door1:groupId3.id" "door1:pCube13Shape.iog.og[0].gid";
connectAttr "door1:lambert9SG.mwc" "door1:pCube13Shape.iog.og[0].gco";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "corridor1:pz_fence:pz_fence.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "corridor1:cube_0_5:lambert2SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "corridor1:lambert2SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "corridor1:lambert3SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "corridor1:lambert4SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "corridor1:lambert5SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "corridor1:lambert6SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "corridor1:lambert7SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "corridor1:lambert8SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "corridor1:lambert9SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "lambert2SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "door1:lambert8SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "door1:initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "door1:lambert9SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "corridor1:pz_fence:pz_fence.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "corridor1:cube_0_5:lambert2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "corridor1:lambert2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "corridor1:lambert3SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "corridor1:lambert4SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "corridor1:lambert5SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "corridor1:lambert6SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "corridor1:lambert7SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "corridor1:lambert8SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "corridor1:lambert9SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "lambert2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "door1:lambert8SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "door1:initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "door1:lambert9SG.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr "polyCylinder1.out" "deleteComponent1.ig";
connectAttr "deleteComponent1.og" "deleteComponent2.ig";
connectAttr "polyCube1.out" "polySplitRing1.ip";
connectAttr "pCubeShape1.wm" "polySplitRing1.mp";
connectAttr "polyTorus1.out" "deleteComponent3.ig";
connectAttr "deleteComponent3.og" "polyChipOff1.ip";
connectAttr "pTorusShape1.wm" "polyChipOff1.mp";
connectAttr "pTorusShape1.o" "polySeparate1.ip";
connectAttr "polyChipOff1.out" "groupParts1.ig";
connectAttr "groupId1.id" "groupParts1.gi";
connectAttr "polySeparate1.out[0]" "groupParts2.ig";
connectAttr "groupId3.id" "groupParts2.gi";
connectAttr "polySeparate1.out[1]" "groupParts3.ig";
connectAttr "groupId4.id" "groupParts3.gi";
connectAttr "groupParts3.og" "polyChipOff2.ip";
connectAttr "polySurfaceShape2.wm" "polyChipOff2.mp";
connectAttr "polySurfaceShape2.o" "polySeparate2.ip";
connectAttr "groupParts2.og" "polyChipOff3.ip";
connectAttr "polySurfaceShape1.wm" "polyChipOff3.mp";
connectAttr "polySurfaceShape1.o" "polySeparate3.ip";
connectAttr "polyTweak1.out" "polySoftEdge1.ip";
connectAttr "pCubeShape1.wm" "polySoftEdge1.mp";
connectAttr "polySplitRing1.out" "polyTweak1.ip";
connectAttr "polyCylinder2.out" "deleteComponent4.ig";
connectAttr "deleteComponent4.og" "deleteComponent5.ig";
connectAttr "deleteComponent5.og" "deleteComponent6.ig";
connectAttr ":mentalrayGlobals.msg" ":mentalrayItemsList.glb";
connectAttr ":miDefaultOptions.msg" ":mentalrayItemsList.opt" -na;
connectAttr ":miDefaultFramebuffer.msg" ":mentalrayItemsList.fb" -na;
connectAttr ":miDefaultOptions.msg" ":mentalrayGlobals.opt";
connectAttr ":miDefaultFramebuffer.msg" ":mentalrayGlobals.fb";
connectAttr "corridor1:pz_fence:pz_fence1.oc" "corridor1:pz_fence:pz_fence.ss";
connectAttr "corridor1:pz_fence:pz_fence.msg" "corridor1:pz_fence:materialInfo1.sg"
		;
connectAttr "corridor1:pz_fence:pz_fence1.msg" "corridor1:pz_fence:materialInfo1.m"
		;
connectAttr "corridor1:pz_fence:pz_fence1F.msg" "corridor1:pz_fence:materialInfo1.t"
		 -na;
connectAttr "corridor1:pz_fence:pz_fence1F.oc" "corridor1:pz_fence:pz_fence1.c";
connectAttr "corridor1:pz_fence:pz_fence1P2D.c" "corridor1:pz_fence:pz_fence1F.c"
		;
connectAttr "corridor1:pz_fence:pz_fence1P2D.tf" "corridor1:pz_fence:pz_fence1F.tf"
		;
connectAttr "corridor1:pz_fence:pz_fence1P2D.rf" "corridor1:pz_fence:pz_fence1F.rf"
		;
connectAttr "corridor1:pz_fence:pz_fence1P2D.s" "corridor1:pz_fence:pz_fence1F.s"
		;
connectAttr "corridor1:pz_fence:pz_fence1P2D.wu" "corridor1:pz_fence:pz_fence1F.wu"
		;
connectAttr "corridor1:pz_fence:pz_fence1P2D.wv" "corridor1:pz_fence:pz_fence1F.wv"
		;
connectAttr "corridor1:pz_fence:pz_fence1P2D.re" "corridor1:pz_fence:pz_fence1F.re"
		;
connectAttr "corridor1:pz_fence:pz_fence1P2D.of" "corridor1:pz_fence:pz_fence1F.of"
		;
connectAttr "corridor1:pz_fence:pz_fence1P2D.r" "corridor1:pz_fence:pz_fence1F.ro"
		;
connectAttr "corridor1:pz_fence:pz_fence1P2D.o" "corridor1:pz_fence:pz_fence1F.uv"
		;
connectAttr "corridor1:pz_fence:pz_fence1P2D.ofs" "corridor1:pz_fence:pz_fence1F.fs"
		;
connectAttr "corridor1:cube_0_5:lambert2SG1.oc" "corridor1:cube_0_5:lambert2SG.ss"
		;
connectAttr "corridor1:cube_0_5:lambert2SG.msg" "corridor1:cube_0_5:materialInfo1.sg"
		;
connectAttr "corridor1:cube_0_5:lambert2SG1.msg" "corridor1:cube_0_5:materialInfo1.m"
		;
connectAttr "corridor1:cube_0_5:lambert2SG1F.msg" "corridor1:cube_0_5:materialInfo1.t"
		 -na;
connectAttr "corridor1:cube_0_5:lambert2SG1F.oc" "corridor1:cube_0_5:lambert2SG1.c"
		;
connectAttr "corridor1:cube_0_5:lambert2SG1P2D.c" "corridor1:cube_0_5:lambert2SG1F.c"
		;
connectAttr "corridor1:cube_0_5:lambert2SG1P2D.tf" "corridor1:cube_0_5:lambert2SG1F.tf"
		;
connectAttr "corridor1:cube_0_5:lambert2SG1P2D.rf" "corridor1:cube_0_5:lambert2SG1F.rf"
		;
connectAttr "corridor1:cube_0_5:lambert2SG1P2D.s" "corridor1:cube_0_5:lambert2SG1F.s"
		;
connectAttr "corridor1:cube_0_5:lambert2SG1P2D.wu" "corridor1:cube_0_5:lambert2SG1F.wu"
		;
connectAttr "corridor1:cube_0_5:lambert2SG1P2D.wv" "corridor1:cube_0_5:lambert2SG1F.wv"
		;
connectAttr "corridor1:cube_0_5:lambert2SG1P2D.re" "corridor1:cube_0_5:lambert2SG1F.re"
		;
connectAttr "corridor1:cube_0_5:lambert2SG1P2D.of" "corridor1:cube_0_5:lambert2SG1F.of"
		;
connectAttr "corridor1:cube_0_5:lambert2SG1P2D.r" "corridor1:cube_0_5:lambert2SG1F.ro"
		;
connectAttr "corridor1:cube_0_5:lambert2SG1P2D.o" "corridor1:cube_0_5:lambert2SG1F.uv"
		;
connectAttr "corridor1:cube_0_5:lambert2SG1P2D.ofs" "corridor1:cube_0_5:lambert2SG1F.fs"
		;
connectAttr "corridor1:polySurfaceShape1.o" "corridor1:polySplitRing10.ip";
connectAttr "corridor1:pCubeShape4.wm" "corridor1:polySplitRing10.mp";
connectAttr "corridor1:polySplitRing10.out" "corridor1:polySplitRing11.ip";
connectAttr "corridor1:pCubeShape4.wm" "corridor1:polySplitRing11.mp";
connectAttr "corridor1:polySplitRing11.out" "corridor1:polyExtrudeFace7.ip";
connectAttr "corridor1:pCubeShape4.wm" "corridor1:polyExtrudeFace7.mp";
connectAttr "corridor1:polyExtrudeFace7.out" "corridor1:polyExtrudeFace8.ip";
connectAttr "corridor1:pCubeShape4.wm" "corridor1:polyExtrudeFace8.mp";
connectAttr "corridor1:file1.oc" "corridor1:lambert2.c";
connectAttr "corridor1:lambert2.oc" "corridor1:lambert2SG.ss";
connectAttr "corridor1:lambert2SG.msg" "corridor1:materialInfo1.sg";
connectAttr "corridor1:lambert2.msg" "corridor1:materialInfo1.m";
connectAttr "corridor1:file1.msg" "corridor1:materialInfo1.t" -na;
connectAttr "corridor1:place2dTexture1.c" "corridor1:file1.c";
connectAttr "corridor1:place2dTexture1.tf" "corridor1:file1.tf";
connectAttr "corridor1:place2dTexture1.rf" "corridor1:file1.rf";
connectAttr "corridor1:place2dTexture1.mu" "corridor1:file1.mu";
connectAttr "corridor1:place2dTexture1.mv" "corridor1:file1.mv";
connectAttr "corridor1:place2dTexture1.s" "corridor1:file1.s";
connectAttr "corridor1:place2dTexture1.wu" "corridor1:file1.wu";
connectAttr "corridor1:place2dTexture1.wv" "corridor1:file1.wv";
connectAttr "corridor1:place2dTexture1.re" "corridor1:file1.re";
connectAttr "corridor1:place2dTexture1.of" "corridor1:file1.of";
connectAttr "corridor1:place2dTexture1.r" "corridor1:file1.ro";
connectAttr "corridor1:place2dTexture1.n" "corridor1:file1.n";
connectAttr "corridor1:place2dTexture1.vt1" "corridor1:file1.vt1";
connectAttr "corridor1:place2dTexture1.vt2" "corridor1:file1.vt2";
connectAttr "corridor1:place2dTexture1.vt3" "corridor1:file1.vt3";
connectAttr "corridor1:place2dTexture1.vc1" "corridor1:file1.vc1";
connectAttr "corridor1:place2dTexture1.o" "corridor1:file1.uv";
connectAttr "corridor1:place2dTexture1.ofs" "corridor1:file1.fs";
connectAttr "corridor1:file2.oc" "corridor1:white_wood.c";
connectAttr "corridor1:white_wood.oc" "corridor1:lambert3SG.ss";
connectAttr "corridor1:pCubeShape3.iog.og[0]" "corridor1:lambert3SG.dsm" -na;
connectAttr "corridor1:pCubeShape3.ciog.cog[0]" "corridor1:lambert3SG.dsm" -na;
connectAttr "corridor1:pCubeShape4.iog.og[0]" "corridor1:lambert3SG.dsm" -na;
connectAttr "corridor1:pCubeShape4.ciog.cog[0]" "corridor1:lambert3SG.dsm" -na;
connectAttr "corridor1:groupId1.msg" "corridor1:lambert3SG.gn" -na;
connectAttr "corridor1:groupId2.msg" "corridor1:lambert3SG.gn" -na;
connectAttr "corridor1:groupId3.msg" "corridor1:lambert3SG.gn" -na;
connectAttr "corridor1:groupId4.msg" "corridor1:lambert3SG.gn" -na;
connectAttr "corridor1:lambert3SG.msg" "corridor1:materialInfo2.sg";
connectAttr "corridor1:white_wood.msg" "corridor1:materialInfo2.m";
connectAttr "corridor1:file2.msg" "corridor1:materialInfo2.t" -na;
connectAttr "corridor1:place2dTexture2.c" "corridor1:file2.c";
connectAttr "corridor1:place2dTexture2.tf" "corridor1:file2.tf";
connectAttr "corridor1:place2dTexture2.rf" "corridor1:file2.rf";
connectAttr "corridor1:place2dTexture2.mu" "corridor1:file2.mu";
connectAttr "corridor1:place2dTexture2.mv" "corridor1:file2.mv";
connectAttr "corridor1:place2dTexture2.s" "corridor1:file2.s";
connectAttr "corridor1:place2dTexture2.wu" "corridor1:file2.wu";
connectAttr "corridor1:place2dTexture2.wv" "corridor1:file2.wv";
connectAttr "corridor1:place2dTexture2.re" "corridor1:file2.re";
connectAttr "corridor1:place2dTexture2.of" "corridor1:file2.of";
connectAttr "corridor1:place2dTexture2.r" "corridor1:file2.ro";
connectAttr "corridor1:place2dTexture2.n" "corridor1:file2.n";
connectAttr "corridor1:place2dTexture2.vt1" "corridor1:file2.vt1";
connectAttr "corridor1:place2dTexture2.vt2" "corridor1:file2.vt2";
connectAttr "corridor1:place2dTexture2.vt3" "corridor1:file2.vt3";
connectAttr "corridor1:place2dTexture2.vc1" "corridor1:file2.vc1";
connectAttr "corridor1:place2dTexture2.o" "corridor1:file2.uv";
connectAttr "corridor1:place2dTexture2.ofs" "corridor1:file2.fs";
connectAttr "corridor1:polyTweak7.out" "corridor1:polyTriangulate3.ip";
connectAttr "corridor1:polyCube3.out" "corridor1:polyTweak7.ip";
connectAttr "corridor1:polyTweak8.out" "corridor1:polyTriangulate4.ip";
connectAttr "corridor1:polyExtrudeFace8.out" "corridor1:polyTweak8.ip";
connectAttr "corridor1:polyTriangulate3.out" "corridor1:groupParts1.ig";
connectAttr "corridor1:groupId1.id" "corridor1:groupParts1.gi";
connectAttr "corridor1:polyTriangulate4.out" "corridor1:groupParts2.ig";
connectAttr "corridor1:groupId3.id" "corridor1:groupParts2.gi";
connectAttr "corridor1:file4.oc" "corridor1:wood.c";
connectAttr "corridor1:wood.oc" "corridor1:lambert4SG.ss";
connectAttr "corridor1:window_frame1Shape.iog.og[1]" "corridor1:lambert4SG.dsm" 
		-na;
connectAttr "corridor1:window_frame1Shape.ciog.cog[0]" "corridor1:lambert4SG.dsm"
		 -na;
connectAttr "corridor1:polySurfaceShape2.iog.og[0]" "corridor1:lambert4SG.dsm" -na
		;
connectAttr "corridor1:polySurfaceShape3.iog.og[0]" "corridor1:lambert4SG.dsm" -na
		;
connectAttr "corridor1:groupId6.msg" "corridor1:lambert4SG.gn" -na;
connectAttr "corridor1:groupId7.msg" "corridor1:lambert4SG.gn" -na;
connectAttr "corridor1:groupId8.msg" "corridor1:lambert4SG.gn" -na;
connectAttr "corridor1:groupId9.msg" "corridor1:lambert4SG.gn" -na;
connectAttr "corridor1:lambert4SG.msg" "corridor1:materialInfo3.sg";
connectAttr "corridor1:wood.msg" "corridor1:materialInfo3.m";
connectAttr "corridor1:file4.msg" "corridor1:materialInfo3.t" -na;
connectAttr "corridor1:place2dTexture3.c" "corridor1:file3.c";
connectAttr "corridor1:place2dTexture3.tf" "corridor1:file3.tf";
connectAttr "corridor1:place2dTexture3.rf" "corridor1:file3.rf";
connectAttr "corridor1:place2dTexture3.mu" "corridor1:file3.mu";
connectAttr "corridor1:place2dTexture3.mv" "corridor1:file3.mv";
connectAttr "corridor1:place2dTexture3.s" "corridor1:file3.s";
connectAttr "corridor1:place2dTexture3.wu" "corridor1:file3.wu";
connectAttr "corridor1:place2dTexture3.wv" "corridor1:file3.wv";
connectAttr "corridor1:place2dTexture3.re" "corridor1:file3.re";
connectAttr "corridor1:place2dTexture3.of" "corridor1:file3.of";
connectAttr "corridor1:place2dTexture3.r" "corridor1:file3.ro";
connectAttr "corridor1:place2dTexture3.n" "corridor1:file3.n";
connectAttr "corridor1:place2dTexture3.vt1" "corridor1:file3.vt1";
connectAttr "corridor1:place2dTexture3.vt2" "corridor1:file3.vt2";
connectAttr "corridor1:place2dTexture3.vt3" "corridor1:file3.vt3";
connectAttr "corridor1:place2dTexture3.vc1" "corridor1:file3.vc1";
connectAttr "corridor1:place2dTexture3.o" "corridor1:file3.uv";
connectAttr "corridor1:place2dTexture3.ofs" "corridor1:file3.fs";
connectAttr "corridor1:place2dTexture4.c" "corridor1:file4.c";
connectAttr "corridor1:place2dTexture4.tf" "corridor1:file4.tf";
connectAttr "corridor1:place2dTexture4.rf" "corridor1:file4.rf";
connectAttr "corridor1:place2dTexture4.mu" "corridor1:file4.mu";
connectAttr "corridor1:place2dTexture4.mv" "corridor1:file4.mv";
connectAttr "corridor1:place2dTexture4.s" "corridor1:file4.s";
connectAttr "corridor1:place2dTexture4.wu" "corridor1:file4.wu";
connectAttr "corridor1:place2dTexture4.wv" "corridor1:file4.wv";
connectAttr "corridor1:place2dTexture4.re" "corridor1:file4.re";
connectAttr "corridor1:place2dTexture4.of" "corridor1:file4.of";
connectAttr "corridor1:place2dTexture4.r" "corridor1:file4.ro";
connectAttr "corridor1:place2dTexture4.n" "corridor1:file4.n";
connectAttr "corridor1:place2dTexture4.vt1" "corridor1:file4.vt1";
connectAttr "corridor1:place2dTexture4.vt2" "corridor1:file4.vt2";
connectAttr "corridor1:place2dTexture4.vt3" "corridor1:file4.vt3";
connectAttr "corridor1:place2dTexture4.vc1" "corridor1:file4.vc1";
connectAttr "corridor1:place2dTexture4.o" "corridor1:file4.uv";
connectAttr "corridor1:place2dTexture4.ofs" "corridor1:file4.fs";
connectAttr "corridor1:window_frame1Shape.o" "corridor1:polySeparate1.ip";
connectAttr "corridor1:polySeparate1.out[0]" "corridor1:groupParts4.ig";
connectAttr "corridor1:groupId8.id" "corridor1:groupParts4.gi";
connectAttr "corridor1:polySeparate1.out[1]" "corridor1:groupParts5.ig";
connectAttr "corridor1:groupId9.id" "corridor1:groupParts5.gi";
connectAttr "corridor1:polyCube4.out" "corridor1:deleteComponent11.ig";
connectAttr "corridor1:blackboard.oc" "corridor1:lambert5.c";
connectAttr "corridor1:lambert5.oc" "corridor1:lambert5SG.ss";
connectAttr "corridor1:pCubeShape5.iog.og[0]" "corridor1:lambert5SG.dsm" -na;
connectAttr "corridor1:pCubeShape5.ciog.cog[0]" "corridor1:lambert5SG.dsm" -na;
connectAttr "corridor1:groupId10.msg" "corridor1:lambert5SG.gn" -na;
connectAttr "corridor1:groupId11.msg" "corridor1:lambert5SG.gn" -na;
connectAttr "corridor1:lambert5SG.msg" "corridor1:materialInfo4.sg";
connectAttr "corridor1:lambert5.msg" "corridor1:materialInfo4.m";
connectAttr "corridor1:blackboard.msg" "corridor1:materialInfo4.t" -na;
connectAttr "corridor1:place2dTexture5.c" "corridor1:blackboard.c";
connectAttr "corridor1:place2dTexture5.tf" "corridor1:blackboard.tf";
connectAttr "corridor1:place2dTexture5.rf" "corridor1:blackboard.rf";
connectAttr "corridor1:place2dTexture5.mu" "corridor1:blackboard.mu";
connectAttr "corridor1:place2dTexture5.mv" "corridor1:blackboard.mv";
connectAttr "corridor1:place2dTexture5.s" "corridor1:blackboard.s";
connectAttr "corridor1:place2dTexture5.wu" "corridor1:blackboard.wu";
connectAttr "corridor1:place2dTexture5.wv" "corridor1:blackboard.wv";
connectAttr "corridor1:place2dTexture5.re" "corridor1:blackboard.re";
connectAttr "corridor1:place2dTexture5.of" "corridor1:blackboard.of";
connectAttr "corridor1:place2dTexture5.r" "corridor1:blackboard.ro";
connectAttr "corridor1:place2dTexture5.n" "corridor1:blackboard.n";
connectAttr "corridor1:place2dTexture5.vt1" "corridor1:blackboard.vt1";
connectAttr "corridor1:place2dTexture5.vt2" "corridor1:blackboard.vt2";
connectAttr "corridor1:place2dTexture5.vt3" "corridor1:blackboard.vt3";
connectAttr "corridor1:place2dTexture5.vc1" "corridor1:blackboard.vc1";
connectAttr "corridor1:place2dTexture5.o" "corridor1:blackboard.uv";
connectAttr "corridor1:place2dTexture5.ofs" "corridor1:blackboard.fs";
connectAttr "corridor1:deleteComponent11.og" "corridor1:polyTweakUV3.ip";
connectAttr "corridor1:groupParts5.og" "corridor1:polyTweakUV4.ip";
connectAttr "corridor1:file5.oc" "corridor1:lambert6.c";
connectAttr "corridor1:lambert6.oc" "corridor1:lambert6SG.ss";
connectAttr "corridor1:lambert6SG.msg" "corridor1:materialInfo5.sg";
connectAttr "corridor1:lambert6.msg" "corridor1:materialInfo5.m";
connectAttr "corridor1:file5.msg" "corridor1:materialInfo5.t" -na;
connectAttr "corridor1:place2dTexture6.c" "corridor1:file5.c";
connectAttr "corridor1:place2dTexture6.tf" "corridor1:file5.tf";
connectAttr "corridor1:place2dTexture6.rf" "corridor1:file5.rf";
connectAttr "corridor1:place2dTexture6.mu" "corridor1:file5.mu";
connectAttr "corridor1:place2dTexture6.mv" "corridor1:file5.mv";
connectAttr "corridor1:place2dTexture6.s" "corridor1:file5.s";
connectAttr "corridor1:place2dTexture6.wu" "corridor1:file5.wu";
connectAttr "corridor1:place2dTexture6.wv" "corridor1:file5.wv";
connectAttr "corridor1:place2dTexture6.re" "corridor1:file5.re";
connectAttr "corridor1:place2dTexture6.of" "corridor1:file5.of";
connectAttr "corridor1:place2dTexture6.r" "corridor1:file5.ro";
connectAttr "corridor1:place2dTexture6.n" "corridor1:file5.n";
connectAttr "corridor1:place2dTexture6.vt1" "corridor1:file5.vt1";
connectAttr "corridor1:place2dTexture6.vt2" "corridor1:file5.vt2";
connectAttr "corridor1:place2dTexture6.vt3" "corridor1:file5.vt3";
connectAttr "corridor1:place2dTexture6.vc1" "corridor1:file5.vc1";
connectAttr "corridor1:place2dTexture6.o" "corridor1:file5.uv";
connectAttr "corridor1:place2dTexture6.ofs" "corridor1:file5.fs";
connectAttr "corridor1:polyTweakUV3.out" "corridor1:groupParts6.ig";
connectAttr "corridor1:groupId10.id" "corridor1:groupParts6.gi";
connectAttr "corridor1:polyCylinder1.out" "corridor1:deleteComponent13.ig";
connectAttr "corridor1:deleteComponent13.og" "corridor1:deleteComponent14.ig";
connectAttr "corridor1:deleteComponent14.og" "corridor1:deleteComponent15.ig";
connectAttr "corridor1:file6.oc" "corridor1:desk.c";
connectAttr "corridor1:desk.oc" "corridor1:lambert7SG.ss";
connectAttr "corridor1:pCubeShape7.iog.og[1]" "corridor1:lambert7SG.dsm" -na;
connectAttr "corridor1:pCubeShape7.ciog.cog[1]" "corridor1:lambert7SG.dsm" -na;
connectAttr "corridor1:pCubeShape8.iog.og[1]" "corridor1:lambert7SG.dsm" -na;
connectAttr "corridor1:pCubeShape8.ciog.cog[1]" "corridor1:lambert7SG.dsm" -na;
connectAttr "corridor1:pCylinderShape1.iog.og[1]" "corridor1:lambert7SG.dsm" -na
		;
connectAttr "corridor1:pCylinderShape1.ciog.cog[1]" "corridor1:lambert7SG.dsm" -na
		;
connectAttr "corridor1:pCylinderShape2.iog.og[1]" "corridor1:lambert7SG.dsm" -na
		;
connectAttr "corridor1:pCylinderShape2.ciog.cog[1]" "corridor1:lambert7SG.dsm" -na
		;
connectAttr "corridor1:pCylinderShape3.iog.og[1]" "corridor1:lambert7SG.dsm" -na
		;
connectAttr "corridor1:pCylinderShape3.ciog.cog[1]" "corridor1:lambert7SG.dsm" -na
		;
connectAttr "corridor1:pCylinderShape4.iog.og[1]" "corridor1:lambert7SG.dsm" -na
		;
connectAttr "corridor1:pCylinderShape4.ciog.cog[1]" "corridor1:lambert7SG.dsm" -na
		;
connectAttr "corridor1:groupId14.msg" "corridor1:lambert7SG.gn" -na;
connectAttr "corridor1:groupId15.msg" "corridor1:lambert7SG.gn" -na;
connectAttr "corridor1:groupId16.msg" "corridor1:lambert7SG.gn" -na;
connectAttr "corridor1:groupId17.msg" "corridor1:lambert7SG.gn" -na;
connectAttr "corridor1:groupId18.msg" "corridor1:lambert7SG.gn" -na;
connectAttr "corridor1:groupId19.msg" "corridor1:lambert7SG.gn" -na;
connectAttr "corridor1:groupId20.msg" "corridor1:lambert7SG.gn" -na;
connectAttr "corridor1:groupId21.msg" "corridor1:lambert7SG.gn" -na;
connectAttr "corridor1:groupId22.msg" "corridor1:lambert7SG.gn" -na;
connectAttr "corridor1:groupId23.msg" "corridor1:lambert7SG.gn" -na;
connectAttr "corridor1:groupId24.msg" "corridor1:lambert7SG.gn" -na;
connectAttr "corridor1:groupId25.msg" "corridor1:lambert7SG.gn" -na;
connectAttr "corridor1:lambert7SG.msg" "corridor1:materialInfo6.sg";
connectAttr "corridor1:desk.msg" "corridor1:materialInfo6.m";
connectAttr "corridor1:file6.msg" "corridor1:materialInfo6.t" -na;
connectAttr "corridor1:place2dTexture7.c" "corridor1:file6.c";
connectAttr "corridor1:place2dTexture7.tf" "corridor1:file6.tf";
connectAttr "corridor1:place2dTexture7.rf" "corridor1:file6.rf";
connectAttr "corridor1:place2dTexture7.mu" "corridor1:file6.mu";
connectAttr "corridor1:place2dTexture7.mv" "corridor1:file6.mv";
connectAttr "corridor1:place2dTexture7.s" "corridor1:file6.s";
connectAttr "corridor1:place2dTexture7.wu" "corridor1:file6.wu";
connectAttr "corridor1:place2dTexture7.wv" "corridor1:file6.wv";
connectAttr "corridor1:place2dTexture7.re" "corridor1:file6.re";
connectAttr "corridor1:place2dTexture7.of" "corridor1:file6.of";
connectAttr "corridor1:place2dTexture7.r" "corridor1:file6.ro";
connectAttr "corridor1:place2dTexture7.n" "corridor1:file6.n";
connectAttr "corridor1:place2dTexture7.vt1" "corridor1:file6.vt1";
connectAttr "corridor1:place2dTexture7.vt2" "corridor1:file6.vt2";
connectAttr "corridor1:place2dTexture7.vt3" "corridor1:file6.vt3";
connectAttr "corridor1:place2dTexture7.vc1" "corridor1:file6.vc1";
connectAttr "corridor1:place2dTexture7.o" "corridor1:file6.uv";
connectAttr "corridor1:place2dTexture7.ofs" "corridor1:file6.fs";
connectAttr "corridor1:deleteComponent15.og" "corridor1:polyTweakUV6.ip";
connectAttr "corridor1:polyCube6.out" "corridor1:polyTweakUV7.ip";
connectAttr "corridor1:polyTweakUV7.out" "corridor1:polyMapCut1.ip";
connectAttr "corridor1:polyMapCut1.out" "corridor1:polyTweakUV8.ip";
connectAttr "corridor1:polyTweakUV8.out" "corridor1:polyMapCut2.ip";
connectAttr "corridor1:polyMapCut2.out" "corridor1:polyTweakUV9.ip";
connectAttr "corridor1:polyTweakUV9.out" "corridor1:polyMapCut3.ip";
connectAttr "corridor1:polyMapCut3.out" "corridor1:polyTweakUV10.ip";
connectAttr "corridor1:polyTweakUV10.out" "corridor1:polyMapCut4.ip";
connectAttr "corridor1:polyMapCut4.out" "corridor1:polyTweakUV11.ip";
connectAttr "corridor1:polyTweakUV11.out" "corridor1:groupParts9.ig";
connectAttr "corridor1:groupId14.id" "corridor1:groupParts9.gi";
connectAttr "corridor1:polyTweakUV6.out" "corridor1:groupParts10.ig";
connectAttr "corridor1:groupId18.id" "corridor1:groupParts10.gi";
connectAttr "corridor1:place2dTexture8.c" "corridor1:file7.c";
connectAttr "corridor1:place2dTexture8.tf" "corridor1:file7.tf";
connectAttr "corridor1:place2dTexture8.rf" "corridor1:file7.rf";
connectAttr "corridor1:place2dTexture8.mu" "corridor1:file7.mu";
connectAttr "corridor1:place2dTexture8.mv" "corridor1:file7.mv";
connectAttr "corridor1:place2dTexture8.s" "corridor1:file7.s";
connectAttr "corridor1:place2dTexture8.wu" "corridor1:file7.wu";
connectAttr "corridor1:place2dTexture8.wv" "corridor1:file7.wv";
connectAttr "corridor1:place2dTexture8.re" "corridor1:file7.re";
connectAttr "corridor1:place2dTexture8.of" "corridor1:file7.of";
connectAttr "corridor1:place2dTexture8.r" "corridor1:file7.ro";
connectAttr "corridor1:place2dTexture8.n" "corridor1:file7.n";
connectAttr "corridor1:place2dTexture8.vt1" "corridor1:file7.vt1";
connectAttr "corridor1:place2dTexture8.vt2" "corridor1:file7.vt2";
connectAttr "corridor1:place2dTexture8.vt3" "corridor1:file7.vt3";
connectAttr "corridor1:place2dTexture8.vc1" "corridor1:file7.vc1";
connectAttr "corridor1:place2dTexture8.o" "corridor1:file7.uv";
connectAttr "corridor1:place2dTexture8.ofs" "corridor1:file7.fs";
connectAttr "corridor1:file8.oc" "corridor1:door_red.c";
connectAttr "corridor1:door_red.oc" "corridor1:lambert8SG.ss";
connectAttr "corridor1:pCubeShape10.iog.og[0]" "corridor1:lambert8SG.dsm" -na;
connectAttr "corridor1:pCubeShape10.ciog.cog[0]" "corridor1:lambert8SG.dsm" -na;
connectAttr "corridor1:groupId26.msg" "corridor1:lambert8SG.gn" -na;
connectAttr "corridor1:groupId27.msg" "corridor1:lambert8SG.gn" -na;
connectAttr "corridor1:lambert8SG.msg" "corridor1:materialInfo7.sg";
connectAttr "corridor1:door_red.msg" "corridor1:materialInfo7.m";
connectAttr "corridor1:file8.msg" "corridor1:materialInfo7.t" -na;
connectAttr "corridor1:place2dTexture9.c" "corridor1:file8.c";
connectAttr "corridor1:place2dTexture9.tf" "corridor1:file8.tf";
connectAttr "corridor1:place2dTexture9.rf" "corridor1:file8.rf";
connectAttr "corridor1:place2dTexture9.mu" "corridor1:file8.mu";
connectAttr "corridor1:place2dTexture9.mv" "corridor1:file8.mv";
connectAttr "corridor1:place2dTexture9.s" "corridor1:file8.s";
connectAttr "corridor1:place2dTexture9.wu" "corridor1:file8.wu";
connectAttr "corridor1:place2dTexture9.wv" "corridor1:file8.wv";
connectAttr "corridor1:place2dTexture9.re" "corridor1:file8.re";
connectAttr "corridor1:place2dTexture9.of" "corridor1:file8.of";
connectAttr "corridor1:place2dTexture9.r" "corridor1:file8.ro";
connectAttr "corridor1:place2dTexture9.n" "corridor1:file8.n";
connectAttr "corridor1:place2dTexture9.vt1" "corridor1:file8.vt1";
connectAttr "corridor1:place2dTexture9.vt2" "corridor1:file8.vt2";
connectAttr "corridor1:place2dTexture9.vt3" "corridor1:file8.vt3";
connectAttr "corridor1:place2dTexture9.vc1" "corridor1:file8.vc1";
connectAttr "corridor1:place2dTexture9.o" "corridor1:file8.uv";
connectAttr "corridor1:place2dTexture9.ofs" "corridor1:file8.fs";
connectAttr "corridor1:polyCube8.out" "corridor1:polyTweakUV12.ip";
connectAttr "corridor1:polyTweak9.out" "corridor1:polyTriangulate6.ip";
connectAttr "corridor1:polyTweakUV12.out" "corridor1:polyTweak9.ip";
connectAttr "corridor1:polyTriangulate6.out" "corridor1:groupParts11.ig";
connectAttr "corridor1:groupId26.id" "corridor1:groupParts11.gi";
connectAttr "corridor1:polyCube9.out" "corridor1:groupParts12.ig";
connectAttr "corridor1:groupId28.id" "corridor1:groupParts12.gi";
connectAttr "corridor1:file9.oc" "corridor1:lambert9.c";
connectAttr "corridor1:lambert9.oc" "corridor1:lambert9SG.ss";
connectAttr "corridor1:lambert9SG.msg" "corridor1:materialInfo8.sg";
connectAttr "corridor1:lambert9.msg" "corridor1:materialInfo8.m";
connectAttr "corridor1:file9.msg" "corridor1:materialInfo8.t" -na;
connectAttr "corridor1:place2dTexture10.c" "corridor1:file9.c";
connectAttr "corridor1:place2dTexture10.tf" "corridor1:file9.tf";
connectAttr "corridor1:place2dTexture10.rf" "corridor1:file9.rf";
connectAttr "corridor1:place2dTexture10.mu" "corridor1:file9.mu";
connectAttr "corridor1:place2dTexture10.mv" "corridor1:file9.mv";
connectAttr "corridor1:place2dTexture10.s" "corridor1:file9.s";
connectAttr "corridor1:place2dTexture10.wu" "corridor1:file9.wu";
connectAttr "corridor1:place2dTexture10.wv" "corridor1:file9.wv";
connectAttr "corridor1:place2dTexture10.re" "corridor1:file9.re";
connectAttr "corridor1:place2dTexture10.of" "corridor1:file9.of";
connectAttr "corridor1:place2dTexture10.r" "corridor1:file9.ro";
connectAttr "corridor1:place2dTexture10.n" "corridor1:file9.n";
connectAttr "corridor1:place2dTexture10.vt1" "corridor1:file9.vt1";
connectAttr "corridor1:place2dTexture10.vt2" "corridor1:file9.vt2";
connectAttr "corridor1:place2dTexture10.vt3" "corridor1:file9.vt3";
connectAttr "corridor1:place2dTexture10.vc1" "corridor1:file9.vc1";
connectAttr "corridor1:place2dTexture10.o" "corridor1:file9.uv";
connectAttr "corridor1:place2dTexture10.ofs" "corridor1:file9.fs";
connectAttr "polySeparate3.out[0]" "polyPlanarProj1.ip";
connectAttr "polySurfaceShape5.wm" "polyPlanarProj1.mp";
connectAttr "polySeparate3.out[1]" "polyPlanarProj2.ip";
connectAttr "polySurfaceShape6.wm" "polyPlanarProj2.mp";
connectAttr "polySeparate2.out[0]" "polyPlanarProj3.ip";
connectAttr "polySurfaceShape3.wm" "polyPlanarProj3.mp";
connectAttr "polySeparate2.out[1]" "polyPlanarProj4.ip";
connectAttr "polySurfaceShape4.wm" "polyPlanarProj4.mp";
connectAttr "deleteComponent6.og" "polyPlanarProj5.ip";
connectAttr "pCylinderShape3.wm" "polyPlanarProj5.mp";
connectAttr "polySurfaceShape7.o" "polyPlanarProj6.ip";
connectAttr "pCylinderShape4.wm" "polyPlanarProj6.mp";
connectAttr "polySurfaceShape8.o" "polyPlanarProj7.ip";
connectAttr "pCylinderShape5.wm" "polyPlanarProj7.mp";
connectAttr "polySurfaceShape9.o" "polyPlanarProj8.ip";
connectAttr "pCylinderShape6.wm" "polyPlanarProj8.mp";
connectAttr "polyTweak2.out" "polyPlanarProj9.ip";
connectAttr "pCubeShape1.wm" "polyPlanarProj9.mp";
connectAttr "polySoftEdge1.out" "polyTweak2.ip";
connectAttr "polyPlanarProj1.out" "polyPlanarProj10.ip";
connectAttr "polySurfaceShape5.wm" "polyPlanarProj10.mp";
connectAttr "polyPlanarProj2.out" "polyPlanarProj11.ip";
connectAttr "polySurfaceShape6.wm" "polyPlanarProj11.mp";
connectAttr "polyPlanarProj3.out" "polyPlanarProj12.ip";
connectAttr "polySurfaceShape3.wm" "polyPlanarProj12.mp";
connectAttr "polyPlanarProj4.out" "polyPlanarProj13.ip";
connectAttr "polySurfaceShape4.wm" "polyPlanarProj13.mp";
connectAttr "polyPlanarProj5.out" "polyPlanarProj14.ip";
connectAttr "pCylinderShape3.wm" "polyPlanarProj14.mp";
connectAttr "polyPlanarProj6.out" "polyPlanarProj15.ip";
connectAttr "pCylinderShape4.wm" "polyPlanarProj15.mp";
connectAttr "polyPlanarProj7.out" "polyPlanarProj16.ip";
connectAttr "pCylinderShape5.wm" "polyPlanarProj16.mp";
connectAttr "polyPlanarProj8.out" "polyPlanarProj17.ip";
connectAttr "pCylinderShape6.wm" "polyPlanarProj17.mp";
connectAttr "polyPlanarProj9.out" "polyPlanarProj18.ip";
connectAttr "pCubeShape1.wm" "polyPlanarProj18.mp";
connectAttr "polyPlanarProj10.out" "polyTweakUV1.ip";
connectAttr "polyPlanarProj11.out" "polyTweakUV2.ip";
connectAttr "polyPlanarProj12.out" "polyTweakUV3.ip";
connectAttr "polyPlanarProj13.out" "polyTweakUV4.ip";
connectAttr "polyPlanarProj14.out" "polyTweakUV5.ip";
connectAttr "polyPlanarProj15.out" "polyTweakUV6.ip";
connectAttr "polyPlanarProj16.out" "polyTweakUV7.ip";
connectAttr "polyPlanarProj17.out" "polyTweakUV8.ip";
connectAttr "polyPlanarProj18.out" "polyTweakUV9.ip";
connectAttr "file1.oc" "piano.c";
connectAttr "piano.oc" "lambert2SG.ss";
connectAttr "pCubeShape1.iog.og[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape1.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "pCylinderShape1.iog.og[0]" "lambert2SG.dsm" -na;
connectAttr "pCylinderShape1.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape2.iog.og[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape2.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "pCylinderShape2.iog.og[0]" "lambert2SG.dsm" -na;
connectAttr "pCylinderShape2.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "polySurfaceShape5.iog.og[1]" "lambert2SG.dsm" -na;
connectAttr "polySurfaceShape5.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "polySurfaceShape6.iog.og[1]" "lambert2SG.dsm" -na;
connectAttr "polySurfaceShape6.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "polySurfaceShape3.iog.og[1]" "lambert2SG.dsm" -na;
connectAttr "polySurfaceShape3.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "polySurfaceShape4.iog.og[1]" "lambert2SG.dsm" -na;
connectAttr "polySurfaceShape4.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "pCylinderShape3.iog.og[0]" "lambert2SG.dsm" -na;
connectAttr "pCylinderShape3.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "pCylinderShape4.iog.og[0]" "lambert2SG.dsm" -na;
connectAttr "pCylinderShape4.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "pCylinderShape5.iog.og[0]" "lambert2SG.dsm" -na;
connectAttr "pCylinderShape5.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "pCylinderShape6.iog.og[0]" "lambert2SG.dsm" -na;
connectAttr "pCylinderShape6.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape3.iog.og[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape3.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape4.iog.og[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape4.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape5.iog.og[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape5.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape6.iog.og[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape6.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape7.iog.og[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape7.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape8.iog.og[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape8.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape9.iog.og[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape9.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape10.iog.og[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape10.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape11.iog.og[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape11.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape12.iog.og[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape12.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape13.iog.og[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape13.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape14.iog.og[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape14.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape15.iog.og[0]" "lambert2SG.dsm" -na;
connectAttr "pCubeShape15.ciog.cog[0]" "lambert2SG.dsm" -na;
connectAttr "groupId5.msg" "lambert2SG.gn" -na;
connectAttr "groupId6.msg" "lambert2SG.gn" -na;
connectAttr "groupId7.msg" "lambert2SG.gn" -na;
connectAttr "groupId8.msg" "lambert2SG.gn" -na;
connectAttr "groupId9.msg" "lambert2SG.gn" -na;
connectAttr "groupId10.msg" "lambert2SG.gn" -na;
connectAttr "groupId11.msg" "lambert2SG.gn" -na;
connectAttr "groupId12.msg" "lambert2SG.gn" -na;
connectAttr "groupId13.msg" "lambert2SG.gn" -na;
connectAttr "groupId14.msg" "lambert2SG.gn" -na;
connectAttr "groupId15.msg" "lambert2SG.gn" -na;
connectAttr "groupId16.msg" "lambert2SG.gn" -na;
connectAttr "groupId17.msg" "lambert2SG.gn" -na;
connectAttr "groupId18.msg" "lambert2SG.gn" -na;
connectAttr "groupId19.msg" "lambert2SG.gn" -na;
connectAttr "groupId20.msg" "lambert2SG.gn" -na;
connectAttr "groupId21.msg" "lambert2SG.gn" -na;
connectAttr "groupId22.msg" "lambert2SG.gn" -na;
connectAttr "groupId23.msg" "lambert2SG.gn" -na;
connectAttr "groupId24.msg" "lambert2SG.gn" -na;
connectAttr "groupId25.msg" "lambert2SG.gn" -na;
connectAttr "groupId26.msg" "lambert2SG.gn" -na;
connectAttr "groupId27.msg" "lambert2SG.gn" -na;
connectAttr "groupId28.msg" "lambert2SG.gn" -na;
connectAttr "groupId29.msg" "lambert2SG.gn" -na;
connectAttr "groupId30.msg" "lambert2SG.gn" -na;
connectAttr "groupId31.msg" "lambert2SG.gn" -na;
connectAttr "groupId32.msg" "lambert2SG.gn" -na;
connectAttr "groupId33.msg" "lambert2SG.gn" -na;
connectAttr "groupId34.msg" "lambert2SG.gn" -na;
connectAttr "groupId35.msg" "lambert2SG.gn" -na;
connectAttr "groupId36.msg" "lambert2SG.gn" -na;
connectAttr "groupId37.msg" "lambert2SG.gn" -na;
connectAttr "groupId38.msg" "lambert2SG.gn" -na;
connectAttr "groupId39.msg" "lambert2SG.gn" -na;
connectAttr "groupId40.msg" "lambert2SG.gn" -na;
connectAttr "groupId41.msg" "lambert2SG.gn" -na;
connectAttr "groupId42.msg" "lambert2SG.gn" -na;
connectAttr "groupId43.msg" "lambert2SG.gn" -na;
connectAttr "groupId44.msg" "lambert2SG.gn" -na;
connectAttr "groupId45.msg" "lambert2SG.gn" -na;
connectAttr "groupId46.msg" "lambert2SG.gn" -na;
connectAttr "groupId47.msg" "lambert2SG.gn" -na;
connectAttr "groupId48.msg" "lambert2SG.gn" -na;
connectAttr "groupId49.msg" "lambert2SG.gn" -na;
connectAttr "groupId50.msg" "lambert2SG.gn" -na;
connectAttr "groupId51.msg" "lambert2SG.gn" -na;
connectAttr "groupId52.msg" "lambert2SG.gn" -na;
connectAttr "groupId53.msg" "lambert2SG.gn" -na;
connectAttr "groupId54.msg" "lambert2SG.gn" -na;
connectAttr "lambert2SG.msg" "materialInfo1.sg";
connectAttr "piano.msg" "materialInfo1.m";
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
connectAttr "polyCube2.out" "polyAutoProj1.ip";
connectAttr "pCubeShape2.wm" "polyAutoProj1.mp";
connectAttr "polyAutoProj1.out" "polyPlanarProj19.ip";
connectAttr "pCubeShape2.wm" "polyPlanarProj19.mp";
connectAttr "polyPlanarProj19.out" "polyTweakUV10.ip";
connectAttr "polySurfaceShape10.o" "polyPlanarProj20.ip";
connectAttr "pCylinderShape2.wm" "polyPlanarProj20.mp";
connectAttr "polyPlanarProj20.out" "polyTweakUV11.ip";
connectAttr "polyTweak3.out" "polyPlanarProj21.ip";
connectAttr "pCylinderShape1.wm" "polyPlanarProj21.mp";
connectAttr "deleteComponent2.og" "polyTweak3.ip";
connectAttr "polyPlanarProj21.out" "polyTweakUV12.ip";
connectAttr "polyCube3.out" "polyTweakUV13.ip";
connectAttr "polyTweakUV9.out" "groupParts4.ig";
connectAttr "groupId5.id" "groupParts4.gi";
connectAttr "polyTweakUV12.out" "groupParts5.ig";
connectAttr "groupId7.id" "groupParts5.gi";
connectAttr "polyTweakUV10.out" "groupParts6.ig";
connectAttr "groupId9.id" "groupParts6.gi";
connectAttr "polyTweakUV11.out" "groupParts7.ig";
connectAttr "groupId11.id" "groupParts7.gi";
connectAttr "polyTweakUV1.out" "groupParts8.ig";
connectAttr "groupId13.id" "groupParts8.gi";
connectAttr "polyTweakUV2.out" "groupParts9.ig";
connectAttr "groupId15.id" "groupParts9.gi";
connectAttr "polyTweakUV3.out" "groupParts10.ig";
connectAttr "groupId17.id" "groupParts10.gi";
connectAttr "polyTweakUV4.out" "groupParts11.ig";
connectAttr "groupId19.id" "groupParts11.gi";
connectAttr "polyTweakUV5.out" "groupParts12.ig";
connectAttr "groupId21.id" "groupParts12.gi";
connectAttr "polyTweakUV6.out" "groupParts13.ig";
connectAttr "groupId23.id" "groupParts13.gi";
connectAttr "polyTweakUV7.out" "groupParts14.ig";
connectAttr "groupId25.id" "groupParts14.gi";
connectAttr "polyTweakUV8.out" "groupParts15.ig";
connectAttr "groupId27.id" "groupParts15.gi";
connectAttr "polyTweakUV13.out" "groupParts16.ig";
connectAttr "groupId29.id" "groupParts16.gi";
connectAttr "door1:lambert8SG1.oc" "door1:lambert8SG.ss";
connectAttr "door1:groupId4.msg" "door1:lambert8SG.gn" -na;
connectAttr "door1:pCube12Shape.iog.og[0]" "door1:lambert8SG.dsm" -na;
connectAttr "door1:lambert8SG.msg" "door1:materialInfo1.sg";
connectAttr "door1:lambert8SG1.msg" "door1:materialInfo1.m";
connectAttr "door1:lambert8SG1F.msg" "door1:materialInfo1.t" -na;
connectAttr "door1:lambert8SG1F.oc" "door1:lambert8SG1.c";
connectAttr "door1:lambert8SG1F.ot" "door1:lambert8SG1.it";
connectAttr "door1:lambert8SG1P2D.c" "door1:lambert8SG1F.c";
connectAttr "door1:lambert8SG1P2D.tf" "door1:lambert8SG1F.tf";
connectAttr "door1:lambert8SG1P2D.rf" "door1:lambert8SG1F.rf";
connectAttr "door1:lambert8SG1P2D.s" "door1:lambert8SG1F.s";
connectAttr "door1:lambert8SG1P2D.wu" "door1:lambert8SG1F.wu";
connectAttr "door1:lambert8SG1P2D.wv" "door1:lambert8SG1F.wv";
connectAttr "door1:lambert8SG1P2D.re" "door1:lambert8SG1F.re";
connectAttr "door1:lambert8SG1P2D.of" "door1:lambert8SG1F.of";
connectAttr "door1:lambert8SG1P2D.r" "door1:lambert8SG1F.ro";
connectAttr "door1:lambert8SG1P2D.o" "door1:lambert8SG1F.uv";
connectAttr "door1:lambert8SG1P2D.ofs" "door1:lambert8SG1F.fs";
connectAttr "door1:initialShadingGroup1.oc" "door1:initialShadingGroup.ss";
connectAttr "door1:groupId5.msg" "door1:initialShadingGroup.gn" -na;
connectAttr "door1:pCube12Shape.iog.og[1]" "door1:initialShadingGroup.dsm" -na;
connectAttr "door1:initialShadingGroup.msg" "door1:materialInfo2.sg";
connectAttr "door1:initialShadingGroup1.msg" "door1:materialInfo2.m";
connectAttr "door1:initialShadingGroup1F.msg" "door1:materialInfo2.t" -na;
connectAttr "door1:initialShadingGroup1F.oc" "door1:initialShadingGroup1.c";
connectAttr "door1:initialShadingGroup1P2D.c" "door1:initialShadingGroup1F.c";
connectAttr "door1:initialShadingGroup1P2D.tf" "door1:initialShadingGroup1F.tf";
connectAttr "door1:initialShadingGroup1P2D.rf" "door1:initialShadingGroup1F.rf";
connectAttr "door1:initialShadingGroup1P2D.s" "door1:initialShadingGroup1F.s";
connectAttr "door1:initialShadingGroup1P2D.wu" "door1:initialShadingGroup1F.wu";
connectAttr "door1:initialShadingGroup1P2D.wv" "door1:initialShadingGroup1F.wv";
connectAttr "door1:initialShadingGroup1P2D.re" "door1:initialShadingGroup1F.re";
connectAttr "door1:initialShadingGroup1P2D.of" "door1:initialShadingGroup1F.of";
connectAttr "door1:initialShadingGroup1P2D.r" "door1:initialShadingGroup1F.ro";
connectAttr "door1:initialShadingGroup1P2D.o" "door1:initialShadingGroup1F.uv";
connectAttr "door1:initialShadingGroup1P2D.ofs" "door1:initialShadingGroup1F.fs"
		;
connectAttr "door1:lambert9SG1.oc" "door1:lambert9SG.ss";
connectAttr "door1:groupId3.msg" "door1:lambert9SG.gn" -na;
connectAttr "door1:pCube13Shape.iog.og[0]" "door1:lambert9SG.dsm" -na;
connectAttr "door1:lambert9SG.msg" "door1:materialInfo3.sg";
connectAttr "door1:lambert9SG1.msg" "door1:materialInfo3.m";
connectAttr "door1:lambert9SG1F.msg" "door1:materialInfo3.t" -na;
connectAttr "door1:lambert9SG1F.oc" "door1:lambert9SG1.c";
connectAttr "door1:lambert9SG1P2D.c" "door1:lambert9SG1F.c";
connectAttr "door1:lambert9SG1P2D.tf" "door1:lambert9SG1F.tf";
connectAttr "door1:lambert9SG1P2D.rf" "door1:lambert9SG1F.rf";
connectAttr "door1:lambert9SG1P2D.s" "door1:lambert9SG1F.s";
connectAttr "door1:lambert9SG1P2D.wu" "door1:lambert9SG1F.wu";
connectAttr "door1:lambert9SG1P2D.wv" "door1:lambert9SG1F.wv";
connectAttr "door1:lambert9SG1P2D.re" "door1:lambert9SG1F.re";
connectAttr "door1:lambert9SG1P2D.of" "door1:lambert9SG1F.of";
connectAttr "door1:lambert9SG1P2D.r" "door1:lambert9SG1F.ro";
connectAttr "door1:lambert9SG1P2D.o" "door1:lambert9SG1F.uv";
connectAttr "door1:lambert9SG1P2D.ofs" "door1:lambert9SG1F.fs";
connectAttr "door1:groupParts2.og" "polyPlanarProj22.ip";
connectAttr "door1:pCube12Shape.wm" "polyPlanarProj22.mp";
connectAttr "door1:polySurfaceShape1.o" "door1:groupParts1.ig";
connectAttr "door1:groupId4.id" "door1:groupParts1.gi";
connectAttr "door1:groupParts1.og" "door1:groupParts2.ig";
connectAttr "door1:groupId5.id" "door1:groupParts2.gi";
connectAttr "polyPlanarProj22.out" "polyTweakUV14.ip";
connectAttr "corridor1:pz_fence:pz_fence.pa" ":renderPartition.st" -na;
connectAttr "corridor1:cube_0_5:lambert2SG.pa" ":renderPartition.st" -na;
connectAttr "corridor1:lambert2SG.pa" ":renderPartition.st" -na;
connectAttr "corridor1:lambert3SG.pa" ":renderPartition.st" -na;
connectAttr "corridor1:lambert4SG.pa" ":renderPartition.st" -na;
connectAttr "corridor1:lambert5SG.pa" ":renderPartition.st" -na;
connectAttr "corridor1:lambert6SG.pa" ":renderPartition.st" -na;
connectAttr "corridor1:lambert7SG.pa" ":renderPartition.st" -na;
connectAttr "corridor1:lambert8SG.pa" ":renderPartition.st" -na;
connectAttr "corridor1:lambert9SG.pa" ":renderPartition.st" -na;
connectAttr "lambert2SG.pa" ":renderPartition.st" -na;
connectAttr "door1:lambert8SG.pa" ":renderPartition.st" -na;
connectAttr "door1:initialShadingGroup.pa" ":renderPartition.st" -na;
connectAttr "door1:lambert9SG.pa" ":renderPartition.st" -na;
connectAttr "corridor1:pz_fence:pz_fence1.msg" ":defaultShaderList1.s" -na;
connectAttr "corridor1:cube_0_5:lambert2SG1.msg" ":defaultShaderList1.s" -na;
connectAttr "corridor1:lambert2.msg" ":defaultShaderList1.s" -na;
connectAttr "corridor1:white_wood.msg" ":defaultShaderList1.s" -na;
connectAttr "corridor1:wood.msg" ":defaultShaderList1.s" -na;
connectAttr "corridor1:lambert5.msg" ":defaultShaderList1.s" -na;
connectAttr "corridor1:lambert6.msg" ":defaultShaderList1.s" -na;
connectAttr "corridor1:desk.msg" ":defaultShaderList1.s" -na;
connectAttr "corridor1:door_red.msg" ":defaultShaderList1.s" -na;
connectAttr "corridor1:lambert9.msg" ":defaultShaderList1.s" -na;
connectAttr "piano.msg" ":defaultShaderList1.s" -na;
connectAttr "door1:lambert8SG1.msg" ":defaultShaderList1.s" -na;
connectAttr "door1:initialShadingGroup1.msg" ":defaultShaderList1.s" -na;
connectAttr "door1:lambert9SG1.msg" ":defaultShaderList1.s" -na;
connectAttr "corridor1:pz_fence:pz_fence1P2D.msg" ":defaultRenderUtilityList1.u"
		 -na;
connectAttr "corridor1:cube_0_5:lambert2SG1P2D.msg" ":defaultRenderUtilityList1.u"
		 -na;
connectAttr "corridor1:place2dTexture1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "corridor1:place2dTexture2.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "corridor1:place2dTexture3.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "corridor1:place2dTexture4.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "corridor1:place2dTexture5.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "corridor1:place2dTexture6.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "corridor1:place2dTexture7.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "corridor1:place2dTexture8.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "corridor1:place2dTexture9.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "corridor1:place2dTexture10.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "door1:lambert8SG1P2D.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "door1:initialShadingGroup1P2D.msg" ":defaultRenderUtilityList1.u" -na
		;
connectAttr "door1:lambert9SG1P2D.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "corridor1:pz_fence:pz_fence1F.msg" ":defaultTextureList1.tx" -na;
connectAttr "corridor1:cube_0_5:lambert2SG1F.msg" ":defaultTextureList1.tx" -na;
connectAttr "corridor1:file1.msg" ":defaultTextureList1.tx" -na;
connectAttr "corridor1:file2.msg" ":defaultTextureList1.tx" -na;
connectAttr "corridor1:file3.msg" ":defaultTextureList1.tx" -na;
connectAttr "corridor1:file4.msg" ":defaultTextureList1.tx" -na;
connectAttr "corridor1:blackboard.msg" ":defaultTextureList1.tx" -na;
connectAttr "corridor1:file5.msg" ":defaultTextureList1.tx" -na;
connectAttr "corridor1:file6.msg" ":defaultTextureList1.tx" -na;
connectAttr "corridor1:file7.msg" ":defaultTextureList1.tx" -na;
connectAttr "corridor1:file8.msg" ":defaultTextureList1.tx" -na;
connectAttr "corridor1:file9.msg" ":defaultTextureList1.tx" -na;
connectAttr "file1.msg" ":defaultTextureList1.tx" -na;
connectAttr "door1:lambert8SG1F.msg" ":defaultTextureList1.tx" -na;
connectAttr "door1:initialShadingGroup1F.msg" ":defaultTextureList1.tx" -na;
connectAttr "door1:lambert9SG1F.msg" ":defaultTextureList1.tx" -na;
connectAttr "corridor1:file7.oc" ":lambert1.c";
connectAttr "pTorusShape1.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "pTorusShape1.ciog.cog[0]" ":initialShadingGroup.dsm" -na;
connectAttr "polySurfaceShape1.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "polySurfaceShape2.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "corridor1:pCubeShape11.iog.og[0]" ":initialShadingGroup.dsm" -na;
connectAttr "corridor1:pCubeShape11.ciog.cog[0]" ":initialShadingGroup.dsm" -na;
connectAttr "groupId1.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId2.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId3.msg" ":initialShadingGroup.gn" -na;
connectAttr "groupId4.msg" ":initialShadingGroup.gn" -na;
connectAttr "corridor1:groupId28.msg" ":initialShadingGroup.gn" -na;
connectAttr "corridor1:groupId29.msg" ":initialShadingGroup.gn" -na;
connectAttr "corridor1:file7.msg" ":initialMaterialInfo.t" -na;
dataStructure -fmt "raw" -as "name=externalContentTable:string=node:string=key:string=upath:uint32=upathcrc:string=rpath:string=roles";
applyMetadata -fmt "raw" -v "channel\nname externalContentTable\nstream\nname v1.0\nindexType numeric\nstructure externalContentTable\n0\n\"corridor1:pz_fence:pz_fence1F\" \"fileTextureName\" \"pz-fence.png\" 1984701315 \"C:/Users/Dad/projects/github/music-theory/Assets/obj/pz-fence.png\" \"sourceImages\"\n1\n\"corridor1:cube_0_5:lambert2SG1F\" \"fileTextureName\" \"plat_wood.png\" 688141114 \"\" \"sourceImages\"\n2\n\"corridor1:file1\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/ground2.png\" 194975755 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/ground2.png\" \"sourceImages\"\n3\n\"corridor1:file2\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/white-wood.png\" 4232265713 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/white-wood.png\" \"sourceImages\"\n4\n\"corridor1:file3\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/wood1.png\" 1800306167 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/wood1.png\" \"sourceImages\"\n5\n\"corridor1:file4\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/wood2.png\" 753796903 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/wood2.png\" \"sourceImages\"\n6\n\"corridor1:blackboard\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/blackboard.png\" 14752819 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/blackboard.png\" \"sourceImages\"\n7\n\"corridor1:file5\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/floor1.png\" 1448379758 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/floor1.png\" \"sourceImages\"\n8\n\"corridor1:file6\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/desk1.png\" 1055611876 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/desk1.png\" \"sourceImages\"\n9\n\"corridor1:file7\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/darkness.png\" 781003458 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/darkness.png\" \"sourceImages\"\n10\n\"corridor1:file8\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/door-red.png\" 2901160684 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/door-red.png\" \"sourceImages\"\n11\n\"corridor1:file9\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/window.png\" 1928970815 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/window.png\" \"sourceImages\"\n12\n\"file1\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/piano1.png\" 986069325 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/piano1.png\" \"sourceImages\"\n13\n\"door1:lambert8SG1F\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/gold-frame.png\" 1745522962 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/gold-frame.png\" \"sourceImages\"\n14\n\"door1:initialShadingGroup1F\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/obj/darkness.png\" 2080053804 \"C:/Users/Dad/projects/github/music-theory/Assets/obj/darkness.png\" \"sourceImages\"\n15\n\"door1:lambert9SG1F\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/flower-border-blurred.png\" 650409121 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/flower-border-blurred.png\" \"sourceImages\"\nendStream\nendChannel\nendAssociations\n" 
		-scn;
// End of picture-frame3.ma
