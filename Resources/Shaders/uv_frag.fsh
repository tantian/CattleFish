#ifdef GL_ES                                                            
precision lowp float;                                                   
#endif                                                                  

varying vec4 v_fragmentColor;                                           
varying vec2 v_texCoord;                                                
uniform vec2 texOffset;                                                 
uniform sampler2D u_Texture0;   
uniform sampler2D u_Texture1;                                          
uniform vec2 uRange;                                                    
uniform vec2 vRange; 
uniform vec4 u_light;         

void main()                                                             
{                                                                       
    vec2 texcoord = texOffset+v_texCoord;                               
    texcoord.x = mod(texcoord.x - uRange.x,uRange.y-uRange.x) + uRange.x;   
    texcoord.y = mod(texcoord.y - vRange.x,vRange.y-vRange.x) + vRange.x;   
    vec4 m_lg = texture2D(u_Texture0, texcoord)*u_light;	
	vec4 m_fragcolor = m_lg;
	float apahe = dot(m_lg, u_light);
	//if (apahe <= 0.3) {
	//	m_fragcolor.a = 0.0;
	//} else {
		m_fragcolor.a = apahe / 3.0;
	//}
	
	gl_FragColor =  texture2D(CC_Texture0, v_texCoord)*m_fragcolor;
} 