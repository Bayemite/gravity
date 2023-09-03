uniform int vertsPerCircle;
uniform int endPoint;
uniform int totalVerts;

void main()
{
    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    vec4 color = gl_Color;
    if(gl_VertexID-endPoint>=0)
        color.w = 1.0/(totalVerts/vertsPerCircle)*((gl_VertexID-endPoint)/vertsPerCircle);
    else
        color.w = 1.0/(totalVerts/vertsPerCircle)*((totalVerts+(gl_VertexID-endPoint))/vertsPerCircle);
    // forward the vertex color
    gl_FrontColor = color;
}