//
//  Shader.fsh
//  Amjula Music Theory
//
//  Created by JC on 18/03/2017.
//  Copyright © 2017 Amju. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
