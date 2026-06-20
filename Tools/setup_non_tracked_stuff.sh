# Set up tools we aren't tracking in git

# Midiscore
echo Building midiscore...
cd ../Source/Midiscore
make
echo Copying midiscore...
cp midiscore ../../Tools
cd ../../Tools

# Makescore
echo Building makescore...
cd ../Source/Makescore
make
echo Copying makescore...
cp makescore ../../Tools
cd ../../Tools

# Listen + dylibs
echo Building listen...
cd ../Script/Macosx
source ./setup_make_vars.sh 
./touch_main_cpps.sh
make -f ListenMakefile
make runsetup
cd ../../Tools
echo Copying listen...
cp ../Build/Mac/listen .
cp ../Build/Mac/*.dylib .
# Copy sound fonts to this dir so we don't rely on hard-coded
#  Assets path
mkdir Sound
cp ../Assets/Sound/steinway_concert_piano.sf2 Sound
cp ../Assets/Sound/Colin_s_Double_Bass.sf2 Sound
cp ../Assets/Sound/Jazz\ Kit.sf2 Sound
echo All done!

