
uniform float near;
uniform float far;
varying float depth;
varying float lighting;
varying float specular;



void main()
{	
    /* The usual texture and vertex positioning stuff */
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = ftransform();

    /* ambient light based on the material and the lighting in the scene */
    float ambient = length(gl_FrontMaterial.ambient.rgb * gl_LightModel.ambient.rgb);
    /* calculate the normal and the light vector for diffuse lighting */
    vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
    vec3 light = normalize(vec3(gl_LightSource[0].position));
    float diffuse = length(max(dot(normal,light),0.0) * gl_FrontMaterial.diffuse
      * gl_LightSource[0].diffuse);
    /* get a single term for lighting, sum of lengths of diffuse and ambient */
    lighting = diffuse + ambient;
    /* also calculate a specular */
    specular = length(pow(max(dot(normal, gl_LightSource[0].halfVector.xyz),0.0)
      ,gl_FrontMaterial.shininess) * gl_LightSource[0].specular.rgb * gl_FrontMaterial.specular.rgb);
    
    /* this is the important bit. the distance from the vertex to the camera
       will be used to calculate what the color of the pixel will be. */
    depth = clamp((gl_Position.z-near)/far,0.0,1.0);
}

