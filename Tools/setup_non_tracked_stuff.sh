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
make -f ListenMakefile
make runsetup
cd ../../Tools
echo Copying listen...
cp ../Build/Mac/listen .
cp ../Build/Mac/*.dylib .

echo All done!

