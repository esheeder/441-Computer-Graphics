/*
 *   Blur Fragment Shader
 *
 *   Jeffrey Paone, Fall 2015
 *       for CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 120

uniform sampler2D tex;

// in order to look a few pixels to the left/right, we need to know how big the image is
// (since we access our texture using normalized coordinates!) so we assume that the
// framebuffer is square and pass in just its width.
uniform float framebufferSize;
uniform float blurSize;     // the side of our blur window

void main() {
    vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);

    // centered around s, go blurSize steps left and right
    for( float i = -blurSize; i <= blurSize; i += 1.0 ) {
        // centered around t, go blurSize steps up and down
        for( float j = -blurSize; j <= blurSize; j += 1.0 ) {
            // take the texel offset from our fragment texel and add it to our running total
            finalColor += texture2D( tex, gl_TexCoord[0].st + (vec2(i, j) / framebufferSize) ); // we need to normalize our blur window position
        }
    }

    float blurDim = (blurSize * 2.0) + 1.0;
    finalColor *= (1.0 / (blurDim*blurDim));    // average the colors by the number of samples we took

    gl_FragColor = clamp( finalColor, 0.0, 1.0 ); // make sure all our values are between 0 and 1
}
