How to build asset files 

You need to do this for XCode iOS (debug and release) and for Mac/Windows release.

To build assets for iOS:
./make-everything-ios.sh

To rebuild `glue` executable if necessary:

    `
    cd amjulib/Tools/glue/Script/MacOSX
    ./make_glue.sh
    `

Now there is a new glue executable in amulib/Build/glue/Mac/glue.

    `
    cp amjulib/Build/glue/Mac/glue music-theory/Script/AssetCompiler
    `

To rebuild `obj2bin` if necessary:

    `
    cd amjulib/Tools/obj2bin/Script/MacOSX
    ./make_obj2bin.sh
    cp amjulib/Build/obj2bin/Mac/obj2bin music-theory/Script/AssetCompiler
    `

