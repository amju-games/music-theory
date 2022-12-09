//Maya ASCII 2015 scene
//Name: end1.ma
//Last modified: Sat, Jan 06, 2018 08:26:15 PM
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
	setAttr ".t" -type "double3" 147.93204248549443 103.57072237971116 117.48557365168996 ;
	setAttr ".r" -type "double3" -22.538352864894225 1505.3999999982379 0 ;
createNode camera -s -n "perspShape" -p "persp";
	setAttr -k off ".v";
	setAttr ".fl" 34.999999999999993;
	setAttr ".coi" 137.32918214345281;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".tp" -type "double3" 22.807748660693278 88.791385573521126 163.18380797568273 ;
	setAttr ".hc" -type "string" "viewSet -p %camera";
createNode transform -s -n "top";
	setAttr ".t" -type "double3" 0.015913768886036905 637.20708471030275 54.366621468767235 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	setAttr -k off ".v";
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 256.03316222263982;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
createNode transform -s -n "front";
	setAttr ".t" -type "double3" -16.552183863239339 6.253615344299984 678.89951078325225 ;
createNode camera -s -n "frontShape" -p "front";
	setAttr -k off ".v";
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 71.317188923261341;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
createNode transform -s -n "side";
	setAttr ".t" -type "double3" 843.71377004352428 39.820380879634556 -174.89348968814477 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	setAttr -k off ".v";
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 82.412222041190802;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
createNode transform -n "pCube6";
	setAttr ".t" -type "double3" -55.640325680127035 -19.751321015692916 288.68963807387217 ;
	setAttr ".rp" -type "double3" 80.275768921866103 1.1368683772161603e-013 -155.3161616096975 ;
	setAttr ".sp" -type "double3" 80.275768921866103 1.1368683772161603e-013 -155.3161616096975 ;
createNode mesh -n "pCubeShape6" -p "pCube6";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.49722030013799667 0.36594878882169724 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 6 ".uvst[0].uvsp[0:5]" -type "float2" 0.15892786 0.99069989
		 0.16988064 0.0055276603 0.83125287 0.99555051 0.83105081 0.0036855415 0.83338279
		 0.66701818 0.16105781 0.66662931;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 6 ".vt[0:5]"  -16.21328735 19.75132179 -153.41178894 195.29986572 19.75132179 -153.41178894
		 -16.21322632 19.75132179 -288.79083252 195.29989624 19.75132179 -288.68798828 59.5525322 19.75132179 -288.7394104
		 59.55248642 19.75132179 -153.41178894;
	setAttr -s 9 ".ed[0:8]"  2 4 0 0 2 0 4 3 0 5 1 0 0 5 0 1 3 0 5 4 1
		 0 4 1 1 4 1;
	setAttr -s 4 -ch 12 ".fc[0:3]" -type "polyFaces" 
		f 3 -2 7 -1
		mu 0 3 2 0 4
		f 3 -8 4 6
		mu 0 3 4 0 5
		f 3 3 8 -7
		mu 0 3 5 1 4
		f 3 -9 5 -3
		mu 0 3 4 1 3;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "left";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -347.68232400733405 61.171016544573263 153.99010241051892 ;
	setAttr ".r" -type "double3" 0 -89.999999999999986 0 ;
createNode camera -n "leftShape" -p "left";
	setAttr -k off ".v";
	setAttr ".rnd" no;
	setAttr ".coi" 308.56522706313001;
	setAttr ".ow" 554.54460046370536;
	setAttr ".imn" -type "string" "left1";
	setAttr ".den" -type "string" "left1_depth";
	setAttr ".man" -type "string" "left1_mask";
	setAttr ".hc" -type "string" "viewSet -ls %camera";
	setAttr ".o" yes;
createNode transform -n "skirting_and_door_frame1";
	setAttr ".t" -type "double3" -273.51787463849388 4.1760727693097337 147.78609917896074 ;
	setAttr ".s" -type "double3" 2.9995435577127907 1 1 ;
	setAttr ".rp" -type "double3" 273.9736809909034 -0.036178827285766602 -80.319259643554688 ;
	setAttr ".sp" -type "double3" 275.005615234375 -0.036178827285766602 -80.319259643554688 ;
	setAttr ".spt" -type "double3" -1.0319342434715963 0 0 ;
createNode mesh -n "skirting_and_door_frame1Shape" -p "skirting_and_door_frame1";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.55226727575063705 0.36318495869636536 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 36 ".uvst[0].uvsp[0:35]" -type "float2" 0.30334616 0.047137309
		 0.87284702 0 0.87284702 0.25 0.87284702 0 -1.50204444 -0.16801292 -0.16601861 -0.16801292
		 -0.16601861 1.16801286 -1.50204444 1.16801286 -1.50204444 -0.16801292 -0.16601861
		 -0.16801292 -0.16601861 1.16801286 -1.50204444 1.16801286 -1.50204444 -0.16801292
		 -0.16601861 -0.16801292 -0.16601861 1.16801286 -1.50204444 1.16801286 -1.50204444
		 -0.16801292 -0.16601861 -0.16801292 -0.16601861 1.16801286 -1.50204444 1.16801286
		 0.87284702 0.25 0.87284702 0 0.87284702 0 0.87284702 0.25 0.30334616 0.27264026 0.23168753
		 0.047137309 0.87284702 0.25 0.23168753 0.27264026 0.33124226 0.72636992 0.33124226
		 0.72217 0.35068303 0.72636992 0.35068303 0.72217 0.33124226 0.72636992 0.33124226
		 0.72217 0.35068303 0.72636992 0.35068303 0.72217;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 16 ".pt[8:23]" -type "float3"  0 0 92.861809 0 0 92.861809 
		0 0 92.861809 0 0 92.861809 0 0 92.861809 0 0 92.861809 0 0 92.861809 0 0 92.861809 
		0 0 92.861809 0 0 92.861809 0 0 92.861809 0 0 92.861809 0 0 3.0517578e-005 0 0 3.0517578e-005 
		0 0 3.0517578e-005 0 0 3.0517578e-005;
	setAttr -s 20 ".vt[0:19]"  276.27206421 -4.17606688 -148.105896 276.27206421 4.17607355 -148.105896
		 269.87380981 4.17607355 -148.105896 269.87380981 -4.17606688 -148.105896 276.27206421 -4.17606688 -148.105896
		 276.27206421 4.17607355 -148.105896 269.87380981 4.17607355 -148.105896 269.87380981 -4.17606688 -148.105896
		 269.87380981 -4.17606688 -104.5307312 269.87380981 4.17607355 -104.5307312 276.27206421 -4.17606688 -104.5307312
		 276.27206421 4.17607355 -104.5307312 276.27206421 4.17607355 -104.4981842 276.27206421 -4.17606688 -104.4981842
		 269.87380981 -4.17606688 -104.4981842 269.87380981 4.17607355 -104.4981842 269.87380981 -4.17606688 -104.5307312
		 269.87380981 4.17607355 -104.5307312 276.27206421 4.17607355 -104.5307312 276.27206421 -4.17606688 -104.5307312;
	setAttr -s 41 ".ed[0:40]"  0 1 0 2 1 0 3 0 0 3 2 0 0 4 0 1 5 0 4 5 0
		 2 6 0 6 5 0 3 7 0 7 4 0 7 6 0 3 14 0 2 15 0 8 9 0 0 13 0 8 10 0 1 12 0 10 11 0 9 11 0
		 12 13 0 13 14 0 14 15 0 15 12 0 0 5 0 2 5 0 3 4 0 3 6 0 3 15 0 0 14 0 1 13 0 2 12 0
		 8 16 0 9 17 0 16 17 0 11 18 0 16 18 0 17 18 0 10 19 0 16 19 0 19 18 0;
	setAttr -s 70 ".n[0:69]" -type "float3"  -5.3636012e-007 0 1 -5.3636012e-007
		 0 1 -5.3636012e-007 0 1 -5.3636012e-007 0 1 -5.3636012e-007 0 1 -5.3636012e-007 0
		 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 -0.99999982 0 0 -0.99999982
		 0 0 -0.99999982 0 0 -0.99999988 0 0 -0.99999988 0 0 -0.99999988 0 0 3.3522507e-008
		 -0.99999994 0 3.3522507e-008 -0.99999994 0 3.3522507e-008 -0.99999994 0 3.3522507e-008
		 -1 0 3.3522507e-008 -1 0 3.3522507e-008 -1 0 0.99999982 0 0 0.99999982 0 0 0.99999982
		 0 0 0.99999988 0 0 0.99999988 0 0 0.99999988 0 0 -3.3522507e-008 1 0 -3.3522507e-008
		 1 0 -3.3522507e-008 1 0 -3.3522507e-008 0.99999994 0 -3.3522507e-008 0.99999994 0
		 -3.3522507e-008 0.99999994 0 1e+020 1e+020 1e+020 1e+020 1e+020 1e+020 -5.3636012e-007
		 0 1 -5.3636012e-007 0 1 1e+020 1e+020 1e+020 1e+020 1e+020 1e+020 -5.3636012e-007
		 0 1 -5.3636012e-007 0 1 1e+020 1e+020 1e+020 1e+020 1e+020 1e+020 -5.3636012e-007
		 0 1 -5.3636012e-007 0 1 1e+020 1e+020 1e+020 1e+020 1e+020 1e+020 -5.3636012e-007
		 0 1 -5.3636012e-007 0 1;
	setAttr -s 22 -ch 70 ".fc[0:21]" -type "polyFaces" 
		f 3 -35 36 -38
		mu 0 3 32 33 34
		f 3 -37 39 40
		mu 0 3 34 33 35
		f 3 6 -25 4
		mu 0 3 7 6 4
		f 3 24 -6 -1
		mu 0 3 4 6 5
		f 3 -8 25 -9
		mu 0 3 11 8 10
		f 3 1 5 -26
		mu 0 3 8 9 10
		f 3 10 -27 9
		mu 0 3 15 14 12
		f 3 26 -5 -3
		mu 0 3 12 14 13
		f 3 -10 27 -12
		mu 0 3 19 16 18
		f 3 3 7 -28
		mu 0 3 16 17 18
		f 3 -4 28 -14
		mu 0 3 2 3 23
		f 3 -29 12 22
		mu 0 3 23 3 22
		f 3 2 29 -13
		mu 0 3 3 1 22
		f 3 -30 15 21
		mu 0 3 22 1 21
		f 3 0 30 -16
		mu 0 3 0 24 25
		f 3 -31 17 20
		mu 0 3 25 24 27
		f 3 -2 31 -18
		mu 0 3 26 2 20
		f 3 -32 13 23
		mu 0 3 20 2 23
		f 4 -15 32 34 -34
		mu 0 4 28 29 33 32
		f 4 -20 33 37 -36
		mu 0 4 30 28 32 34
		f 4 16 38 -40 -33
		mu 0 4 29 31 35 33
		f 4 18 35 -41 -39
		mu 0 4 31 30 34 35;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr ".ndt" 2;
	setAttr ".dn" yes;
createNode mesh -n "polySurfaceShape4" -p "skirting_and_door_frame1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.49999997019767761 1.0999807119369507 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 96 ".uvst[0].uvsp[0:95]" -type "float2" 0.87284702 0.25 0.87284702
		 0 0.875 0 0.875 0.25 0.87284702 0.25 0.875 0.25 0.875 0 0.87284702 0 -0.16801292
		 -0.16801292 1.16801286 -0.16801292 1.16801286 1.16801286 -0.16801292 1.16801286 -0.16801292
		 -0.16801292 1.16801286 -0.16801292 1.16801286 1.16801286 -0.16801292 1.16801286 -0.16801292
		 -0.16801292 1.16801286 -0.16801292 1.16801286 1.16801286 -0.16801292 1.16801286 -0.16801292
		 -0.16801292 1.16801286 -0.16801292 1.16801286 1.16801286 -0.16801292 1.16801286 0.87284702
		 0.25 0.87284702 0 0.87284702 0 0.87284702 0.25 0.87284702 0.25 0.87284702 0 0.87284702
		 0 0.87284702 0.25 0.87284702 0.25 0.87284702 0.25 0.87284702 0.25 0.87284702 0.25
		 0.87284702 0.25 0.87284702 0.25 0.87284702 0.25 0.87284702 0.25 0.87284702 0.25 0.87284702
		 0.25 0.87284702 0.25 0.87284702 0.25 -0.16801292 -0.16801292 1.16801286 -0.16801292
		 1.16801286 1.16801286 -0.16801292 1.16801286 -0.16801292 -0.16801292 1.16801286 -0.16801292
		 1.16801286 1.16801286 -0.16801292 1.16801286 -0.16801292 -0.16801292 1.16801286 -0.16801292
		 1.16801286 1.16801286 -0.16801292 1.16801286 -0.16801292 -0.16801292 1.16801286 -0.16801292
		 1.16801286 1.16801286 -0.16801292 1.16801286 -0.16801292 -0.16801292 1.16801286 -0.16801292
		 1.16801286 1.16801286 -0.16801292 1.16801286 -0.16801292 -0.16801292 1.16801286 -0.16801292
		 1.16801286 1.16801286 -0.16801292 1.16801286 -0.16801292 -0.16801292 1.16801286 -0.16801292
		 1.16801286 1.16801286 -0.16801292 1.16801286 -0.16801292 -0.16801292 1.16801286 -0.16801292
		 1.16801286 1.16801286 -0.16801292 1.16801286 -0.16801292 1.031948566 -0.16801292
		 1.031948566 1.16801286 1.031948566 1.16801286 1.031948566 1.16801286 1.031948566
		 -0.16801292 1.031948566 1.16801286 1.031948566 -0.16801292 1.031948566 -0.16801292
		 1.031948566 1.16801286 1.031948566 1.16801286 1.16801286 -0.16801292 1.16801286 -0.16801292
		 1.16801286 1.16801286 1.16801286 1.16801286 1.031948566 -0.16801292 1.031948566 1.16801286
		 1.16801286 -0.16801292 1.16801286 -0.16801292 1.16801286 1.16801286 1.16801286;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 3 ".pt";
	setAttr ".pt[37]" -type "float3" 0 -6.6791017e-013 4.7683716e-006 ;
	setAttr ".pt[39]" -type "float3" 0 -6.6791017e-013 4.7683716e-006 ;
	setAttr ".pt[41]" -type "float3" 0 -6.6791017e-013 4.7683716e-006 ;
	setAttr -s 56 ".vt[0:55]"  276.27203369 -4.1760726 -146.94355774 276.27203369 4.1760726 -146.94355774
		 276.27203369 -4.1760726 -149.51881409 276.27203369 4.1760726 -149.51881409 273.73919678 4.1760726 -146.94355774
		 273.73919678 4.1760726 -149.51881409 273.73919678 -4.1760726 -149.51881409 273.73919678 -4.1760726 -146.94355774
		 276.27203369 -4.1760726 -146.94355774 276.27203369 4.1760726 -146.94355774 273.73919678 4.1760726 -146.94355774
		 273.73919678 -4.1760726 -146.94355774 273.73919678 -4.1760726 -104.53091431 273.73919678 4.1760726 -104.53091431
		 276.27203369 -4.1760726 -104.53091431 276.27203369 4.1760726 -104.53091431 276.27203369 4.1760726 -111.10068512
		 276.27203369 -4.1760726 -111.10068512 273.73919678 -4.1760726 -111.10068512 273.73919678 4.1760726 -111.10068512
		 273.73919678 4.1760726 -111.10068512 276.27203369 4.1760726 -111.10068512 273.73919678 4.1760726 -104.53091431
		 276.27203369 4.1760726 -104.53091431 273.73919678 84.96228027 -111.10068512 276.27203369 84.96228027 -111.10068512
		 273.73919678 84.96228027 -104.53091431 276.27203369 84.96228027 -104.53091431 273.73919678 78.85879517 -104.53091431
		 273.73919678 78.85879517 -111.10068512 276.27203369 78.85879517 -111.10068512 276.27203369 78.85879517 -104.53091431
		 273.73919678 84.96228027 -104.53091431 276.27203369 84.96228027 -104.53091431 273.73919678 78.85879517 -104.53091431
		 276.27203369 78.85879517 -104.53091431 273.73919678 84.96228027 -52.8311615 276.27203369 84.96228027 -52.8311615
		 273.73919678 78.85879517 -52.8311615 276.27203369 78.85879517 -52.8311615 276.27203369 78.85879517 -59.21468353
		 276.27203369 84.96228027 -59.21468353 273.73919678 84.96228027 -59.21468353 273.73919678 78.85879517 -59.21468353
		 273.73919678 -4.2484374 -59.21468353 276.27203369 -4.2484374 -59.21468353 273.73919678 -4.2484374 -52.8311615
		 276.27203369 -4.2484374 -52.8311615 276.27203369 4.12795973 -52.8311615 273.73919678 4.12795973 -52.8311615
		 273.73919678 4.12795973 -59.21468353 276.27203369 4.12795973 -59.21468353 276.27203369 4.12795973 150.69284058
		 273.73919678 4.12795973 150.69284058 276.27203369 -4.2484374 150.69284058 273.73919678 -4.2484374 150.69284058;
	setAttr -s 161 ".ed[0:160]"  0 1 0 2 0 0 3 2 0 1 3 0 4 1 0 5 3 0 4 5 0
		 6 2 0 5 6 0 7 0 0 6 7 0 7 4 0 0 8 0 1 9 0 8 9 0 4 10 0 10 9 0 7 11 0 11 8 0 11 10 0
		 7 18 0 4 19 0 12 13 0 0 17 0 12 14 0 1 16 0 14 15 0 13 15 0 16 15 0 17 14 0 16 17 0
		 18 12 0 17 18 0 19 13 0 18 19 0 19 16 0 19 20 0 16 21 0 20 21 0 13 22 0 20 22 0 15 23 0
		 22 23 0 21 23 0 20 29 0 21 30 0 24 25 0 22 28 0 24 26 0 23 31 0 26 27 0 25 27 0 28 26 0
		 29 24 0 28 29 0 30 25 0 29 30 0 31 27 0 30 31 0 31 28 0 26 32 0 27 33 0 32 33 0 28 34 0
		 34 32 0 31 35 0 35 33 0 35 34 0 32 42 0 33 41 0 36 37 0 34 43 0 38 36 0 35 40 0 39 37 0
		 39 38 0 40 39 0 41 37 0 40 41 0 42 36 0 41 42 0 43 38 0 42 43 0 43 40 0 43 50 0 40 51 0
		 44 45 0 38 49 0 44 46 0 39 48 0 47 46 0 45 47 0 48 47 0 49 46 0 48 49 0 50 44 0 49 50 0
		 51 45 0 50 51 0 51 48 0 48 52 0 49 53 0 52 53 0 47 54 0 52 54 0 46 55 0 54 55 0 53 55 0
		 0 3 0 4 3 0 5 2 0 6 0 0 12 15 0 0 9 0 4 9 0 7 8 0 7 10 0 7 19 0 0 18 0 1 17 0 4 16 0
		 16 14 0 17 12 0 18 13 0 24 27 0 16 20 0 19 22 0 13 23 0 15 21 0 20 30 0 22 29 0 23 28 0
		 21 31 0 28 24 0 29 25 0 30 27 0 31 26 0 26 33 0 28 32 0 31 33 0 31 34 0 33 42 0 32 43 0
		 41 39 0 34 40 0 33 40 0 41 36 0 42 38 0 44 47 0 43 51 0 38 50 0 39 49 0 40 48 0 52 55 0
		 49 44 0 50 45 0 51 47 0 48 53 0 47 52 0 46 54 0 49 55 0;
	setAttr -s 318 ".n";
	setAttr ".n[0:165]" -type "float3"  1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 0
		 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 -1
		 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 0 0
		 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 -0.99999994 0 0 -0.99999994
		 0 0 -0.99999994 0 0 -1 0 0 -1 0 0 -1 0 0 0 -0.99999994 0 0 -0.99999994 0 0 -0.99999994
		 0 0 -1 0 0 -1 0 0 -1 0 0.99999994 0 0 0.99999994 0 0 0.99999994 0 0 1 0 0 1 0 0 1
		 0 0 0 1 0 0 1 0 0 1 0 0 0.99999994 0 0 0.99999994 0 0 0.99999994 0 0.99999994 0 0
		 0.99999994 0 0 0.99999994 0 0 1 0 0 1 0 0 1 0 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1
		 0 0 -1 0 -0.99999994 0 0 -0.99999994 0 0 -0.99999994 0 0 -1 0 0 -1 0 0 -1 0 0 0 1
		 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		 0 0 0 0 0 0 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 -0.99999994 0 0 -0.99999994
		 0 0 -0.99999994 0 0 -0.99999994 0 0 -0.99999994 0 0 -0.99999994 0 0 0 0 1 0 0 1 0
		 0 1 0 0 1 0 0 1 0 0 1 0.99999994 0 0 0.99999994 0 0 0.99999994 0 0 0.99999994 0 0
		 0.99999994 0 0 0.99999994 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 0 0 -1 0
		 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 1 0 0 1 0 0 1 0 0 1 0 0;
	setAttr ".n[166:317]" -type "float3"  1 0 0 1 0 0 0 0 1 0 0 1 0 0 1 0 0 1 0
		 0 1 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 1
		 0 0 1 0 0 1 0 0 1 0 0 1 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0
		 -1 0 0 -1 0 0 -1 0 0 -1 0 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 -1 0 0 -1 0
		 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 -1 0 0 -1 0 0 -1
		 0 0 -1 0 0 -1 0 0 -1 0 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 0 -1 0 0 -1
		 0 0 -1 0 0 -1 0 0 -1 0 0 -1 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 0 0 1 0 0 1
		 0 0 1 0 0 1 0 0 1 0 0 1 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 0 0 0.99999994 0 0 0.99999994
		 0 0 0.99999994 0 0 1 0 0 1 0 0 1 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 0 0 -0.99999994
		 0 0 -0.99999994 0 0 -0.99999994 0 0 -1 0 0 -1 0 0 -1 1 0 0 1 0 0 1 0 0 1 0 0 1 0
		 0 1 0 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0.99999994 0 0 0.99999994 0 0 0.99999994
		 0 0 0.99999994 0 0 0.99999994 0 0 0.99999994 0 0 0 -1 0 0 -1 0 0 -1 0 0 -1 0 0 -1
		 0 0 -1 0 -0.99999994 0 0 -0.99999994 0 0 -0.99999994 0 0 -0.99999994 0 0 -0.99999994
		 0 0 -0.99999994 0 0;
	setAttr -s 106 -ch 318 ".fc[0:105]" -type "polyFaces" 
		f 3 -1 108 -4
		mu 0 3 0 1 3
		f 3 -109 -2 -3
		mu 0 3 3 1 2
		f 3 -7 109 -6
		mu 0 3 5 4 3
		f 3 -110 4 3
		mu 0 3 3 4 0
		f 3 -9 110 -8
		mu 0 3 6 5 2
		f 3 -111 5 2
		mu 0 3 2 5 3
		f 3 -11 111 -10
		mu 0 3 7 6 1
		f 3 -112 7 1
		mu 0 3 1 6 2
		f 3 -23 112 -28
		mu 0 3 24 25 27
		f 3 -113 24 26
		mu 0 3 27 25 26
		f 3 -13 113 -15
		mu 0 3 11 8 10
		f 3 0 13 -114
		mu 0 3 8 9 10
		f 3 -16 114 -17
		mu 0 3 15 12 14
		f 3 4 13 -115
		mu 0 3 12 13 14
		f 3 -18 115 -19
		mu 0 3 19 16 18
		f 3 9 12 -116
		mu 0 3 16 17 18
		f 3 -18 116 -20
		mu 0 3 23 20 22
		f 3 11 15 -117
		mu 0 3 20 21 22
		f 3 -12 117 -22
		mu 0 3 4 7 31
		f 3 -118 20 34
		mu 0 3 31 7 30
		f 3 9 118 -21
		mu 0 3 7 1 30
		f 3 -119 23 32
		mu 0 3 30 1 29
		f 3 0 119 -24
		mu 0 3 1 0 29
		f 3 -120 25 30
		mu 0 3 29 0 28
		f 3 -5 120 -26
		mu 0 3 0 4 28
		f 3 -121 21 35
		mu 0 3 28 4 31
		f 3 -31 121 -30
		mu 0 3 29 28 26
		f 3 -122 28 -27
		mu 0 3 26 28 27
		f 3 -33 122 -32
		mu 0 3 30 29 25
		f 3 -123 29 -25
		mu 0 3 25 29 26
		f 3 -35 123 -34
		mu 0 3 31 30 24
		f 3 -124 31 22
		mu 0 3 24 30 25
		f 3 -47 124 -52
		mu 0 3 36 37 39
		f 3 -125 48 50
		mu 0 3 39 37 38
		f 3 -38 125 38
		mu 0 3 32 28 33
		f 3 -36 36 -126
		mu 0 3 28 31 33
		f 3 -37 126 -41
		mu 0 3 33 31 34
		f 3 33 39 -127
		mu 0 3 31 24 34
		f 3 -40 127 -43
		mu 0 3 34 24 35
		f 3 27 41 -128
		mu 0 3 24 27 35
		f 3 -42 128 43
		mu 0 3 35 27 32
		f 3 -29 37 -129
		mu 0 3 27 28 32
		f 3 -39 129 -46
		mu 0 3 32 33 42
		f 3 -130 44 56
		mu 0 3 42 33 41
		f 3 40 130 -45
		mu 0 3 33 34 41
		f 3 -131 47 54
		mu 0 3 41 34 40
		f 3 42 131 -48
		mu 0 3 34 35 40
		f 3 -132 49 59
		mu 0 3 40 35 43
		f 3 -44 132 -50
		mu 0 3 35 32 43
		f 3 -133 45 58
		mu 0 3 43 32 42
		f 3 -55 133 -54
		mu 0 3 41 40 37
		f 3 -134 52 -49
		mu 0 3 37 40 38
		f 3 -57 134 -56
		mu 0 3 42 41 36
		f 3 -135 53 46
		mu 0 3 36 41 37
		f 3 -59 135 -58
		mu 0 3 43 42 39
		f 3 -136 55 51
		mu 0 3 39 42 36
		f 3 -60 136 -53
		mu 0 3 40 43 38
		f 3 -137 57 -51
		mu 0 3 38 43 39
		f 3 -61 137 -63
		mu 0 3 47 44 46
		f 3 50 61 -138
		mu 0 3 44 45 46
		f 3 -64 138 -65
		mu 0 3 51 48 50
		f 3 52 60 -139
		mu 0 3 48 49 50
		f 3 -66 139 -67
		mu 0 3 55 52 54
		f 3 57 61 -140
		mu 0 3 52 53 54
		f 3 -66 140 -68
		mu 0 3 59 56 58
		f 3 59 63 -141
		mu 0 3 56 57 58
		f 3 68 -142 -63
		mu 0 3 60 81 61
		f 3 -81 -70 141
		mu 0 3 81 78 61
		f 3 71 -143 -65
		mu 0 3 64 83 65
		f 3 -83 -69 142
		mu 0 3 83 80 65
		f 3 76 -144 -79
		mu 0 3 76 71 79
		f 3 74 -78 143
		mu 0 3 71 70 79
		f 3 73 -145 -68
		mu 0 3 72 77 73
		f 3 -84 -72 144
		mu 0 3 77 82 73
		f 3 73 -146 -67
		mu 0 3 68 76 69
		f 3 78 -70 145
		mu 0 3 76 79 69
		f 3 79 -147 80
		mu 0 3 81 63 78
		f 3 70 -78 146
		mu 0 3 63 62 78
		f 3 81 -148 82
		mu 0 3 83 67 80
		f 3 72 -80 147
		mu 0 3 67 66 80
		f 3 91 -149 86
		mu 0 3 84 87 85
		f 3 90 -89 148
		mu 0 3 87 86 85
		f 3 85 -150 83
		mu 0 3 77 91 82
		f 3 -99 -85 149
		mu 0 3 91 90 82
		f 3 84 -151 -82
		mu 0 3 82 90 74
		f 3 -97 -88 150
		mu 0 3 90 89 74
		f 3 87 -152 75
		mu 0 3 74 89 75
		f 3 -95 -90 151
		mu 0 3 89 88 75
		f 3 89 -153 76
		mu 0 3 75 88 77
		f 3 -100 -86 152
		mu 0 3 88 91 77
		f 3 107 -154 102
		mu 0 3 92 95 93
		f 3 -107 -105 153
		mu 0 3 95 94 93
		f 3 95 -155 96
		mu 0 3 90 85 89
		f 3 88 -94 154
		mu 0 3 85 86 89
		f 3 97 -156 98
		mu 0 3 91 84 90
		f 3 -87 -96 155
		mu 0 3 84 85 90
		f 3 92 -157 99
		mu 0 3 88 87 91
		f 3 -92 -98 156
		mu 0 3 87 84 91
		f 3 101 -158 94
		mu 0 3 89 92 88
		f 3 -103 -101 157
		mu 0 3 92 93 88
		f 3 100 -159 -93
		mu 0 3 88 93 87
		f 3 104 -104 158
		mu 0 3 93 94 87
		f 3 103 -160 -91
		mu 0 3 87 94 86
		f 3 106 -106 159
		mu 0 3 94 95 86
		f 3 105 -161 93
		mu 0 3 86 95 89
		f 3 -108 -102 160
		mu 0 3 95 92 89;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr ".ndt" 2;
	setAttr ".dn" yes;
createNode transform -n "pCube15";
	setAttr ".t" -type "double3" 0.90712962900548888 0 -0.71568509358590404 ;
	setAttr ".rp" -type "double3" -8.4359073638916016 73.253517150878906 68.679157257080078 ;
	setAttr ".sp" -type "double3" -8.4359073638916016 73.253517150878906 68.679157257080078 ;
createNode mesh -n "pCube15Shape" -p "pCube15";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.50841736793518066 0.43057337403297424 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 105 ".uvst[0].uvsp[0:104]" -type "float2" 0 0.19937074 0 1
		 1 0.19937074 0.66980457 0.2751137 0.64710259 0.27541637 0.61174321 0.27565664 0.59860182
		 0.27570212 0.56718779 0.27581087 0.55262065 0.27582651 0.51779795 0.27586398 0.50323081
		 0.27584833 0.46840811 0.27581087 0.45526695 0.27576536 0.42385292 0.27565664 0.38849354
		 0.27541637 0.36579156 0.27511367 0.36755514 0.48373085 0.37490869 0.52737892 0.39624906
		 0.56675452 0.42948747 0.59800315 1 1 0.47137046 0.61806595 0.51779795 0.62497914
		 0.56422567 0.61806595 0.57633758 0.61226404 0.60610867 0.59800315 0.61574316 0.58894533
		 0.63934708 0.56675452 0.66068745 0.52737904 0.66283464 0.5146333 0.66804075 0.48373085
		 0.4861736 0.14048865 0.49907401 0.14048937 0.50447059 0.11723611 0.48617354 0.11723611
		 0.55449033 0.14048856 0.55988687 0.14048865 0.55988687 0.11723611 0.59246558 0.37554231
		 0.50841737 0.42682824 0.6072219 0.40164179 0.50447071 0.14048964 0.5415898 0.14048833
		 0.5415898 0.11723611 0.56948197 0.35482976 0.61230659 0.4305732 0.52276778 0.14049116
		 0.52276778 0.11723611 0.53619331 0.14048845 0.5405212 0.34153154 0.6072219 0.4595046
		 0.37192023 0.080408692 0.37192029 0.016185164 0.52329272 0.14048865 0.52329272 0.11723611
		 0.50841737 0.33694926 0.59246558 0.48560411 0.39021736 0.079960018 0.39021736 0.016185164
		 0.51789612 0.14048895 0.47631365 0.34153154 0.50499564 0.14048964 0.50499564 0.11723611
		 0.56948197 0.50631696 0.40317157 0.079765677 0.40851441 0.016185164 0.4473528 0.35482976
		 0.48669851 0.14049116 0.48669851 0.11723611 0.5405212 0.51961523 0.40851447 0.079685509
		 0.42436916 0.37554231 0.42681152 0.079437822 0.42681155 0.016185164 0.46840143 0.14049304
		 0.46840143 0.11723611 0.50841737 0.52419752 0.40961286 0.40164179 0.439805 0.079298258
		 0.44510868 0.016185164 0.57318819 0.079960018 0.57318819 0.016185164 0.47631365 0.51961523
		 0.40452802 0.4305732 0.44510862 0.079241306 0.45811442 0.079151601 0.46340573 0.016185164
		 0.55489123 0.079685509 0.55489111 0.016185164 0.4473528 0.50631696 0.40961286 0.4595046
		 0.46340573 0.079115123 0.4817028 0.079071611 0.4817028 0.016185164 0.53659403 0.079437822
		 0.53659403 0.016185164 0.42436916 0.48560411 0.49999988 0.079115123 0.49999988 0.016185164
		 0.51829696 0.079241306 0.51829696 0.016185164 0.59148532 0.016185164 0.59148532 0.080408692
		 0.53356117 0.11972675 0.54106474 0.14049304;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".clst[0].clsn" -type "string" "Colors";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 52 ".vt[0:51]"  2.23389435 0 137.54394531 2.23389435 146.5070343 137.54394531
		 2.23389435 0 -0.1856308 2.23389435 40.30609131 45.29204559 2.23389435 40.35043716 48.4187851
		 2.23389435 40.38563919 53.28880692 2.23389435 40.39230347 55.098766327 2.23389435 40.40823364 59.42538834
		 2.23389435 40.41052628 61.43172073 2.23389435 40.41601181 66.22784424 2.23389435 40.41371918 68.23416901
		 2.23389435 40.40823364 73.030296326 2.23389435 40.40156555 74.84024811 2.23389435 40.38563919 79.16687775
		 2.23389435 40.35043716 84.036895752 2.23389435 40.30609131 87.16363525 2.23389435 70.86997986 86.92071533
		 2.23389435 77.26472473 85.9079361 2.23389435 83.03352356 82.96872711 2.23389435 87.61166382 78.39080811
		 2.23389435 146.5070343 -0.1856308 2.23389435 90.55101013 72.62229156 2.23389435 91.56384277 66.22784424
		 2.23389435 90.55101013 59.8333931 2.23389435 89.70098877 58.16519928 2.23389435 87.61166382 54.064876556
		 2.23389435 86.28462982 52.73790741 2.23389435 83.03352356 49.48695374 2.23389435 77.26473236 46.54774475
		 2.23389435 75.3973999 46.25200272 2.23389435 70.86997223 45.53496552 -12.45245838 40.52983856 53.28880692
		 -12.45223522 40.5524292 59.42538834 -12.35063076 71.013175964 66.22784424 -12.45215893 40.56021118 66.22784424
		 -12.45280361 40.49464035 48.4187851 -12.45323849 40.45029831 45.29204559 -12.45223522 40.5524292 73.030296326
		 -12.35063076 71.013175964 45.53496552 -12.45245838 40.52983856 79.16687775 -12.28784847 77.40731812 46.54774475
		 -12.45280361 40.49464035 84.036895752 -12.23121357 83.17555237 49.48695374 -12.45323849 40.45029831 87.16363525
		 -12.1862669 87.75325012 54.064876556 -12.35063076 71.013175964 86.92071533 -12.15740871 90.69231415 59.8333931
		 -12.28784847 77.40731049 85.9079361 -12.14746571 91.70504761 66.22784424 -12.23121357 83.17555237 82.96872711
		 -12.15740871 90.69231415 72.62229156 -12.1862669 87.75325012 78.39080811;
	setAttr -s 149 ".ed[0:148]"  0 1 0 2 0 0 2 3 1 3 4 0 4 5 0 5 6 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 12 0 12 13 0 13 14 0 14 15 0 15 16 0 16 17 0 17 18 0
		 18 19 0 19 20 1 20 1 0 19 21 0 21 22 0 22 23 0 23 24 0 24 25 0 25 26 0 26 27 0 27 28 0
		 28 29 0 29 30 0 30 3 0 2 20 0 6 31 1 31 32 0 32 7 1 32 8 1 31 33 1 33 32 1 5 31 1
		 32 34 0 34 9 1 35 33 1 31 35 0 33 34 1 4 35 1 34 10 1 36 33 1 35 36 0 33 37 1 37 34 0
		 3 36 0 37 11 1 38 33 1 36 38 0 33 39 1 39 37 0 30 38 1 37 12 1 40 33 1 38 40 0 39 13 1
		 33 41 1 41 39 0 29 40 1 42 33 1 40 42 0 41 14 1 33 43 1 43 41 0 28 40 1 44 33 1 42 44 0
		 27 42 1 43 15 0 33 45 1 45 43 0 46 33 1 44 46 0 26 44 1 45 16 1 33 47 1 47 45 0 48 33 1
		 46 48 0 24 46 1 44 25 1 47 17 1 33 49 1 49 47 0 50 33 1 48 50 0 22 48 1 46 23 1 49 18 1
		 33 51 1 51 49 0 50 51 0 21 50 1 51 19 1 1 19 1 3 0 1 4 0 1 5 0 1 6 0 1 7 0 1 8 0 1
		 9 0 1 10 0 1 11 0 1 12 0 1 13 0 1 14 0 1 15 0 1 15 1 1 16 1 1 17 1 1 18 1 1 21 20 1
		 22 20 1 23 20 1 20 3 1 20 30 1 20 29 1 20 28 1 20 27 1 24 20 1 25 20 1 20 26 1 6 32 1
		 8 34 1 4 31 1 3 35 1 10 37 1 30 36 1 12 39 1 29 38 1 13 41 1 27 40 1 14 43 1 26 42 1
		 43 16 1 24 44 1 45 17 1 22 46 1 47 18 1 50 22 1 49 19 1 51 21 1;
	setAttr -s 98 -ch 294 ".fc[0:97]" -type "polyFaces" 
		f 3 20 100 19
		mu 0 3 20 1 19
		f 3 2 101 -2
		mu 0 3 2 3 0
		f 3 3 102 -102
		mu 0 3 3 4 0
		f 3 4 103 -103
		mu 0 3 4 5 0
		f 3 5 104 -104
		mu 0 3 5 6 0
		f 3 6 105 -105
		mu 0 3 6 7 0
		f 3 7 106 -106
		mu 0 3 7 8 0
		f 3 8 107 -107
		mu 0 3 8 9 0
		f 3 9 108 -108
		mu 0 3 9 10 0
		f 3 10 109 -109
		mu 0 3 10 11 0
		f 3 11 110 -110
		mu 0 3 11 12 0
		f 3 12 111 -111
		mu 0 3 12 13 0
		f 3 13 112 -112
		mu 0 3 13 14 0
		f 3 14 113 -113
		mu 0 3 14 15 0
		f 3 -114 114 -1
		mu 0 3 0 15 1
		f 3 15 115 -115
		mu 0 3 15 16 1
		f 3 16 116 -116
		mu 0 3 16 17 1
		f 3 17 117 -117
		mu 0 3 17 18 1
		f 3 18 -101 -118
		mu 0 3 18 19 1
		f 3 21 118 -20
		mu 0 3 19 21 20
		f 3 22 119 -119
		mu 0 3 21 22 20
		f 3 23 120 -120
		mu 0 3 22 23 20
		f 3 32 121 -3
		mu 0 3 2 20 3
		f 3 -122 122 31
		mu 0 3 3 20 30
		f 3 -123 123 30
		mu 0 3 30 20 29
		f 3 -124 124 29
		mu 0 3 29 20 28
		f 3 -125 125 28
		mu 0 3 28 20 27
		f 3 24 126 -121
		mu 0 3 23 24 20
		f 3 25 127 -127
		mu 0 3 24 25 20
		f 3 -126 128 27
		mu 0 3 27 20 26
		f 3 26 -129 -128
		mu 0 3 25 26 20
		f 3 -7 129 35
		mu 0 3 31 32 34
		f 3 -130 33 34
		mu 0 3 34 32 33
		f 3 -8 -36 36
		mu 0 3 35 36 37
		f 3 37 38 -35
		mu 0 3 38 39 40
		f 3 -6 39 -34
		mu 0 3 32 41 33
		f 3 -9 130 41
		mu 0 3 42 35 43
		f 3 -131 -37 40
		mu 0 3 43 35 37
		f 3 42 -38 43
		mu 0 3 44 39 38
		f 3 -39 44 -41
		mu 0 3 40 39 45
		f 3 -5 131 -40
		mu 0 3 41 46 33
		f 3 -132 45 -44
		mu 0 3 33 46 47
		f 3 -10 -42 46
		mu 0 3 48 42 43
		f 3 47 -43 48
		mu 0 3 49 39 44
		f 3 -45 49 50
		mu 0 3 45 39 50
		f 3 -4 132 -46
		mu 0 3 46 103 47
		f 3 -133 51 -49
		mu 0 3 47 103 104
		f 3 -11 133 52
		mu 0 3 53 48 54
		f 3 -134 -47 -51
		mu 0 3 54 48 43
		f 3 53 -48 54
		mu 0 3 55 39 49
		f 3 -50 55 56
		mu 0 3 50 39 56
		f 3 -32 134 -52
		mu 0 3 51 57 52
		f 3 -135 57 -55
		mu 0 3 52 57 58
		f 3 -12 -53 58
		mu 0 3 59 53 54
		f 3 59 -54 60
		mu 0 3 60 39 55
		f 3 -13 135 61
		mu 0 3 61 59 62
		f 3 -136 -59 -57
		mu 0 3 62 59 54
		f 3 -56 62 63
		mu 0 3 56 39 63
		f 3 -31 136 -58
		mu 0 3 57 64 58
		f 3 -137 64 -61
		mu 0 3 58 64 65
		f 3 65 -60 66
		mu 0 3 66 39 60
		f 3 -14 137 67
		mu 0 3 67 61 68
		f 3 -138 -62 -64
		mu 0 3 68 61 62
		f 3 -63 68 69
		mu 0 3 63 39 69
		f 3 -30 70 -65
		mu 0 3 64 70 65
		f 3 71 -66 72
		mu 0 3 71 39 66
		f 3 -29 138 -71
		mu 0 3 70 72 65
		f 3 -139 73 -67
		mu 0 3 65 72 73
		f 3 -15 139 74
		mu 0 3 74 67 75
		f 3 -140 -68 -70
		mu 0 3 75 67 68
		f 3 -69 75 76
		mu 0 3 69 39 76
		f 3 77 -72 78
		mu 0 3 77 39 71
		f 3 -28 140 -74
		mu 0 3 72 78 73
		f 3 -141 79 -73
		mu 0 3 73 78 79
		f 3 -75 141 -16
		mu 0 3 102 101 80
		f 3 -142 -77 80
		mu 0 3 80 101 81
		f 3 -76 81 82
		mu 0 3 76 39 82
		f 3 83 -78 84
		mu 0 3 83 39 77
		f 3 -26 142 86
		mu 0 3 84 85 79
		f 3 -143 85 -79
		mu 0 3 79 85 86
		f 3 -27 -87 -80
		mu 0 3 78 84 79
		f 3 -81 143 -17
		mu 0 3 80 81 87
		f 3 -144 -83 87
		mu 0 3 87 81 88
		f 3 -82 88 89
		mu 0 3 82 39 89
		f 3 90 -84 91
		mu 0 3 90 39 83
		f 3 -24 144 93
		mu 0 3 91 92 86
		f 3 -145 92 -85
		mu 0 3 86 92 93
		f 3 -25 -94 -86
		mu 0 3 85 91 86
		f 3 -88 145 -18
		mu 0 3 87 88 94
		f 3 -146 -90 94
		mu 0 3 94 88 95
		f 3 -89 95 96
		mu 0 3 89 39 96
		f 3 -96 -91 97
		mu 0 3 96 39 90
		f 3 98 146 -23
		mu 0 3 97 98 92
		f 3 -147 -92 -93
		mu 0 3 92 98 93
		f 3 -95 147 -19
		mu 0 3 94 95 99
		f 3 -148 -97 99
		mu 0 3 99 95 100
		f 3 -100 148 -22
		mu 0 3 99 100 97
		f 3 -149 -98 -99
		mu 0 3 97 100 98;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
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
	setAttr -s 13 ".lnk";
	setAttr -s 13 ".slnk";
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
createNode script -n "uiConfigurationScriptNode";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"top\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n"
		+ "                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 0\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"vp2Renderer\" \n                -objectFilterShowInHUD 1\n"
		+ "                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n"
		+ "                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n"
		+ "            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n"
		+ "            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"side\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 0\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n"
		+ "                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"vp2Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n"
		+ "                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n"
		+ "                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"side\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n"
		+ "            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n"
		+ "            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n"
		+ "\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"front\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 0\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n"
		+ "                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"vp2Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n"
		+ "                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n"
		+ "                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n"
		+ "            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n"
		+ "            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n"
		+ "            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"flat\" \n                -displayAppearance \"smoothShaded\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n"
		+ "                -twoSidedLighting 0\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 1\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"vp2Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n"
		+ "                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n"
		+ "                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"flat\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n"
		+ "            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 1\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n"
		+ "            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n"
		+ "            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n                -docTag \"isolOutln_fromSeln\" \n                -showShapes 0\n                -showReferenceNodes 1\n                -showReferenceMembers 1\n                -showAttributes 0\n"
		+ "                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 1\n                -showAssets 1\n                -showContainedOnly 1\n                -showPublishedAsConnected 0\n                -showContainerContents 1\n                -ignoreDagHierarchy 0\n                -expandConnections 0\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"defaultSetFilter\" \n                -showSetMembers 1\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n"
		+ "                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 0\n                -ignoreHiddenAttribute 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -docTag \"isolOutln_fromSeln\" \n            -showShapes 0\n            -showReferenceNodes 1\n            -showReferenceMembers 1\n"
		+ "            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n"
		+ "            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"graphEditor\" -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showReferenceNodes 0\n"
		+ "                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n"
		+ "                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -autoFit 0\n"
		+ "                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n"
		+ "                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n"
		+ "                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n"
		+ "                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dopeSheetPanel\" -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n"
		+ "                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n"
		+ "                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n"
		+ "                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n"
		+ "                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n"
		+ "                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"clipEditorPanel\" -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n"
		+ "                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"sequenceEditorPanel\" -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperGraphPanel\" -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n"
		+ "                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n"
		+ "                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n"
		+ "                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperShadePanel\" -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" == $panelName) {\n"
		+ "\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"visorPanel\" -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"nodeEditorPanel\" (localizedPanelLabel(\"Node Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"nodeEditorPanel\" -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -defaultPinnedState 0\n                -ignoreAssets 1\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n"
		+ "                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -keyReleaseCommand \"nodeEdKeyReleaseCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                $editorName;;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -defaultPinnedState 0\n                -ignoreAssets 1\n"
		+ "                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -keyReleaseCommand \"nodeEdKeyReleaseCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                $editorName;;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"createNodePanel\" (localizedPanelLabel(\"Create Node\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"createNodePanel\" -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Texture Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"polyTexturePlacementPanel\" -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"renderWindowPanel\" -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"blendShapePanel\" (localizedPanelLabel(\"Blend Shape\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tblendShapePanel -unParent -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tblendShapePanel -edit -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynRelEdPanel\" -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n"
		+ "\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"relationshipPanel\" -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"referenceEditorPanel\" -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"componentEditorPanel\" -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynPaintScriptedPanelType\" -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"scriptEditorPanel\" -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-defaultImage \"vacantCell.xP:/\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n"
		+ "\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"flat\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 1\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"flat\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 1\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        setFocus `paneLayout -q -p1 $gMainPane`;\n        sceneUIReplacement -deleteRemaining;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 120 -ast 1 -aet 200 ";
	setAttr ".st" 6;
createNode lambert -n "wall";
createNode shadingEngine -n "lambert2SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
createNode file -n "file1";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//wall1.png";
createNode place2dTexture -n "place2dTexture1";
createNode lambert -n "woodwork";
createNode shadingEngine -n "lambert3SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo2";
createNode file -n "file2";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//woodwork.png";
createNode place2dTexture -n "place2dTexture2";
createNode lambert -n "wood";
createNode shadingEngine -n "lambert4SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo3";
createNode file -n "file3";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//wood1.png";
createNode place2dTexture -n "place2dTexture3";
createNode file -n "file4";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//wood2.png";
createNode place2dTexture -n "place2dTexture4";
createNode lambert -n "lambert5";
createNode shadingEngine -n "lambert5SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo4";
createNode file -n "blackboard";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//blackboard.png";
createNode place2dTexture -n "place2dTexture5";
createNode lambert -n "floor";
createNode shadingEngine -n "lambert6SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo5";
createNode file -n "file5";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//floor2.png";
createNode place2dTexture -n "place2dTexture6";
createNode lambert -n "desk";
createNode shadingEngine -n "lambert7SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo6";
createNode file -n "file6";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//desk1.png";
createNode place2dTexture -n "place2dTexture7";
createNode file -n "file7";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//darkness.png";
createNode place2dTexture -n "place2dTexture8";
createNode lambert -n "door_red";
createNode shadingEngine -n "lambert8SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo7";
createNode file -n "file8";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//door-red.png";
createNode place2dTexture -n "place2dTexture9";
createNode lambert -n "lambert9";
createNode shadingEngine -n "lambert9SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo8";
createNode file -n "file9";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//window.png";
createNode place2dTexture -n "place2dTexture10";
createNode lambert -n "picture_rail";
createNode shadingEngine -n "lambert10SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo9";
createNode file -n "file10";
	setAttr ".ftn" -type "string" "C:/Users/Dad/projects/github/music-theory/Assets/Maya//woodwork.png";
createNode place2dTexture -n "place2dTexture11";
createNode groupId -n "groupId34";
	setAttr ".ihi" 0;
createNode groupId -n "groupId39";
	setAttr ".ihi" 0;
select -ne :time1;
	setAttr ".o" 1;
	setAttr ".unw" 1;
select -ne :renderPartition;
	setAttr -s 13 ".st";
select -ne :renderGlobalsList1;
select -ne :defaultShaderList1;
	setAttr -s 13 ".s";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
	setAttr -s 13 ".u";
select -ne :defaultRenderingList1;
select -ne :defaultTextureList1;
	setAttr -s 13 ".tx";
select -ne :initialShadingGroup;
	setAttr ".ro" yes;
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
relationship "link" ":lightLinker1" "lambert8SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "lambert9SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "lambert10SG.message" ":defaultLightSet.message";
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
relationship "shadowLink" ":lightLinker1" "lambert8SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "lambert9SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "lambert10SG.message" ":defaultLightSet.message";
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
connectAttr "file1.oc" "wall.c";
connectAttr "wall.oc" "lambert2SG.ss";
connectAttr "pCube15Shape.iog" "lambert2SG.dsm" -na;
connectAttr "lambert2SG.msg" "materialInfo1.sg";
connectAttr "wall.msg" "materialInfo1.m";
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
connectAttr "file2.oc" "woodwork.c";
connectAttr "woodwork.oc" "lambert3SG.ss";
connectAttr "skirting_and_door_frame1Shape.iog" "lambert3SG.dsm" -na;
connectAttr "lambert3SG.msg" "materialInfo2.sg";
connectAttr "woodwork.msg" "materialInfo2.m";
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
connectAttr "file4.oc" "wood.c";
connectAttr "wood.oc" "lambert4SG.ss";
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
connectAttr "blackboard.oc" "lambert5.c";
connectAttr "lambert5.oc" "lambert5SG.ss";
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
connectAttr "file5.oc" "floor.c";
connectAttr "floor.oc" "lambert6SG.ss";
connectAttr "pCubeShape6.iog" "lambert6SG.dsm" -na;
connectAttr "lambert6SG.msg" "materialInfo5.sg";
connectAttr "floor.msg" "materialInfo5.m";
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
connectAttr "file6.oc" "desk.c";
connectAttr "desk.oc" "lambert7SG.ss";
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
connectAttr "place2dTexture8.c" "file7.c";
connectAttr "place2dTexture8.tf" "file7.tf";
connectAttr "place2dTexture8.rf" "file7.rf";
connectAttr "place2dTexture8.mu" "file7.mu";
connectAttr "place2dTexture8.mv" "file7.mv";
connectAttr "place2dTexture8.s" "file7.s";
connectAttr "place2dTexture8.wu" "file7.wu";
connectAttr "place2dTexture8.wv" "file7.wv";
connectAttr "place2dTexture8.re" "file7.re";
connectAttr "place2dTexture8.of" "file7.of";
connectAttr "place2dTexture8.r" "file7.ro";
connectAttr "place2dTexture8.n" "file7.n";
connectAttr "place2dTexture8.vt1" "file7.vt1";
connectAttr "place2dTexture8.vt2" "file7.vt2";
connectAttr "place2dTexture8.vt3" "file7.vt3";
connectAttr "place2dTexture8.vc1" "file7.vc1";
connectAttr "place2dTexture8.o" "file7.uv";
connectAttr "place2dTexture8.ofs" "file7.fs";
connectAttr "file8.oc" "door_red.c";
connectAttr "door_red.oc" "lambert8SG.ss";
connectAttr "lambert8SG.msg" "materialInfo7.sg";
connectAttr "door_red.msg" "materialInfo7.m";
connectAttr "file8.msg" "materialInfo7.t" -na;
connectAttr "place2dTexture9.c" "file8.c";
connectAttr "place2dTexture9.tf" "file8.tf";
connectAttr "place2dTexture9.rf" "file8.rf";
connectAttr "place2dTexture9.mu" "file8.mu";
connectAttr "place2dTexture9.mv" "file8.mv";
connectAttr "place2dTexture9.s" "file8.s";
connectAttr "place2dTexture9.wu" "file8.wu";
connectAttr "place2dTexture9.wv" "file8.wv";
connectAttr "place2dTexture9.re" "file8.re";
connectAttr "place2dTexture9.of" "file8.of";
connectAttr "place2dTexture9.r" "file8.ro";
connectAttr "place2dTexture9.n" "file8.n";
connectAttr "place2dTexture9.vt1" "file8.vt1";
connectAttr "place2dTexture9.vt2" "file8.vt2";
connectAttr "place2dTexture9.vt3" "file8.vt3";
connectAttr "place2dTexture9.vc1" "file8.vc1";
connectAttr "place2dTexture9.o" "file8.uv";
connectAttr "place2dTexture9.ofs" "file8.fs";
connectAttr "file9.oc" "lambert9.c";
connectAttr "lambert9.oc" "lambert9SG.ss";
connectAttr "lambert9SG.msg" "materialInfo8.sg";
connectAttr "lambert9.msg" "materialInfo8.m";
connectAttr "file9.msg" "materialInfo8.t" -na;
connectAttr "place2dTexture10.c" "file9.c";
connectAttr "place2dTexture10.tf" "file9.tf";
connectAttr "place2dTexture10.rf" "file9.rf";
connectAttr "place2dTexture10.mu" "file9.mu";
connectAttr "place2dTexture10.mv" "file9.mv";
connectAttr "place2dTexture10.s" "file9.s";
connectAttr "place2dTexture10.wu" "file9.wu";
connectAttr "place2dTexture10.wv" "file9.wv";
connectAttr "place2dTexture10.re" "file9.re";
connectAttr "place2dTexture10.of" "file9.of";
connectAttr "place2dTexture10.r" "file9.ro";
connectAttr "place2dTexture10.n" "file9.n";
connectAttr "place2dTexture10.vt1" "file9.vt1";
connectAttr "place2dTexture10.vt2" "file9.vt2";
connectAttr "place2dTexture10.vt3" "file9.vt3";
connectAttr "place2dTexture10.vc1" "file9.vc1";
connectAttr "place2dTexture10.o" "file9.uv";
connectAttr "place2dTexture10.ofs" "file9.fs";
connectAttr "file10.oc" "picture_rail.c";
connectAttr "picture_rail.oc" "lambert10SG.ss";
connectAttr "lambert10SG.msg" "materialInfo9.sg";
connectAttr "picture_rail.msg" "materialInfo9.m";
connectAttr "file10.msg" "materialInfo9.t" -na;
connectAttr "place2dTexture11.c" "file10.c";
connectAttr "place2dTexture11.tf" "file10.tf";
connectAttr "place2dTexture11.rf" "file10.rf";
connectAttr "place2dTexture11.mu" "file10.mu";
connectAttr "place2dTexture11.mv" "file10.mv";
connectAttr "place2dTexture11.s" "file10.s";
connectAttr "place2dTexture11.wu" "file10.wu";
connectAttr "place2dTexture11.wv" "file10.wv";
connectAttr "place2dTexture11.re" "file10.re";
connectAttr "place2dTexture11.of" "file10.of";
connectAttr "place2dTexture11.r" "file10.ro";
connectAttr "place2dTexture11.n" "file10.n";
connectAttr "place2dTexture11.vt1" "file10.vt1";
connectAttr "place2dTexture11.vt2" "file10.vt2";
connectAttr "place2dTexture11.vt3" "file10.vt3";
connectAttr "place2dTexture11.vc1" "file10.vc1";
connectAttr "place2dTexture11.o" "file10.uv";
connectAttr "place2dTexture11.ofs" "file10.fs";
connectAttr "pz_fence:pz_fence.pa" ":renderPartition.st" -na;
connectAttr "cube_0_5:lambert2SG.pa" ":renderPartition.st" -na;
connectAttr "lambert2SG.pa" ":renderPartition.st" -na;
connectAttr "lambert3SG.pa" ":renderPartition.st" -na;
connectAttr "lambert4SG.pa" ":renderPartition.st" -na;
connectAttr "lambert5SG.pa" ":renderPartition.st" -na;
connectAttr "lambert6SG.pa" ":renderPartition.st" -na;
connectAttr "lambert7SG.pa" ":renderPartition.st" -na;
connectAttr "lambert8SG.pa" ":renderPartition.st" -na;
connectAttr "lambert9SG.pa" ":renderPartition.st" -na;
connectAttr "lambert10SG.pa" ":renderPartition.st" -na;
connectAttr "pz_fence:pz_fence1.msg" ":defaultShaderList1.s" -na;
connectAttr "cube_0_5:lambert2SG1.msg" ":defaultShaderList1.s" -na;
connectAttr "wall.msg" ":defaultShaderList1.s" -na;
connectAttr "woodwork.msg" ":defaultShaderList1.s" -na;
connectAttr "wood.msg" ":defaultShaderList1.s" -na;
connectAttr "lambert5.msg" ":defaultShaderList1.s" -na;
connectAttr "floor.msg" ":defaultShaderList1.s" -na;
connectAttr "desk.msg" ":defaultShaderList1.s" -na;
connectAttr "door_red.msg" ":defaultShaderList1.s" -na;
connectAttr "lambert9.msg" ":defaultShaderList1.s" -na;
connectAttr "picture_rail.msg" ":defaultShaderList1.s" -na;
connectAttr "pz_fence:pz_fence1P2D.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "cube_0_5:lambert2SG1P2D.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture2.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture3.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture4.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture5.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture6.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture7.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture8.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture9.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture10.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture11.msg" ":defaultRenderUtilityList1.u" -na;
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
connectAttr "file7.msg" ":defaultTextureList1.tx" -na;
connectAttr "file8.msg" ":defaultTextureList1.tx" -na;
connectAttr "file9.msg" ":defaultTextureList1.tx" -na;
connectAttr "file10.msg" ":defaultTextureList1.tx" -na;
connectAttr "file7.oc" ":lambert1.c";
connectAttr "file7.msg" ":initialMaterialInfo.t" -na;
dataStructure -fmt "raw" -as "name=externalContentTable:string=node:string=key:string=upath:uint32=upathcrc:string=rpath:string=roles";
applyMetadata -fmt "raw" -v "channel\nname externalContentTable\nstream\nname v1.0\nindexType numeric\nstructure externalContentTable\n0\n\"pz_fence:pz_fence1F\" \"fileTextureName\" \"pz-fence.png\" 1984701315 \"\" \"sourceImages\"\n1\n\"cube_0_5:lambert2SG1F\" \"fileTextureName\" \"plat_wood.png\" 688141114 \"\" \"sourceImages\"\n2\n\"file1\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/wall1.png\" 2743072565 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/wall1.png\" \"sourceImages\"\n3\n\"file2\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/woodwork.png\" 1457291753 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/woodwork.png\" \"sourceImages\"\n4\n\"file3\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/wood1.png\" 1800306167 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/wood1.png\" \"sourceImages\"\n5\n\"file4\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/wood2.png\" 753796903 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/wood2.png\" \"sourceImages\"\n6\n\"blackboard\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/blackboard.png\" 14752819 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/blackboard.png\" \"sourceImages\"\n7\n\"file5\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/floor2.png\" 301268926 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/floor2.png\" \"sourceImages\"\n8\n\"file6\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/desk1.png\" 1055611876 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/desk1.png\" \"sourceImages\"\n9\n\"file7\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/darkness.png\" 781003458 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/darkness.png\" \"sourceImages\"\n10\n\"file8\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/door-red.png\" 2901160684 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/door-red.png\" \"sourceImages\"\n11\n\"file9\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/window.png\" 1928970815 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/window.png\" \"sourceImages\"\n12\n\"file10\" \"fileTextureName\" \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/woodwork.png\" 1457291753 \"C:/Users/Dad/projects/github/music-theory/Assets/Maya/woodwork.png\" \"sourceImages\"\nendStream\nendChannel\nendAssociations\n" 
		-scn;
// End of end1.ma
