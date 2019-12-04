#version 330 core
out vec4 FragColor;

in vec4 outColor;
in vec2 outTexCoord;

uniform sampler2D mainTex;
uniform sampler2D tex2;

void main()
{
    //FragColor = outColor;
    vec4 color1 = texture(mainTex, outTexCoord);
    vec4 color2 = texture(tex2, outTexCoord);

    //混合两个贴图的颜色
    color1.rgb = color1.rgb * (1.0f - color2.a) + color2.rgb * color2.a * 1.0f;
    FragColor = color1;
}