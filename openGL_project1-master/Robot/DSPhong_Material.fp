// ADS Point lighting Shader
// Fragment Shader
// Richard S. Wright Jr.
// OpenGL SuperBible
#version 430
struct MaterialInfo{ //物體與其身上的材質是分開的，而不同材質對於光源的反應都不盡相同
	vec3 Ka; //ambient(各個角落都有的微弱光)
	vec3 Kd; //diffuse(會產生陰影的光)
	vec3 Ks; //specular(會有圓形光暈打在物件上)
};

uniform MaterialInfo Material;
out vec4 vFragColor;//傳到下一階段的資料

//lighting color
vec4    ambientColor = vec4(0.1,0.1,0.1,1);//已經直接把光強度和光顏色融合了(ambientColor = Strength * lightColor;)
vec4    diffuseColor = vec4(0.8,0.8,0.8,1);   
vec4    specularColor = vec4(1,1,1,1);

in vec3 vVaryingNormal;
in vec3 vVaryingLightDir;
in vec2 UV;
float Shininess = 128.0;//for material specular
in vec3 viewDir;

//postprocessing stuff
uniform int mode;
uniform float time; //做煥彩特效
uniform samplerCube skybox; // 做鏡像特效
in vec3 vLight;

void main(void)
{ 
	
    // Dot product gives us diffuse intensity
    float diff = max(0.0, dot(normalize(vVaryingNormal), normalize(vVaryingLightDir)));//物體到光源(單位向量) dot 法向量(單位向量)
					

    // Multiply intensity by diffuse color, force alpha to 1.0
    vFragColor = diff * diffuseColor*vec4(Material.Kd,1); // diffuse 的結果 = 光打到角度不同造成的強弱光 * 光色 * 物體對光的反應


    // Add in ambient light
    vFragColor += ambientColor;// 要輸出的vFragColor要把三種光特性打在物件上的結果加總


    // Specular Light
    vec3 vReflection = normalize(reflect(-normalize(vVaryingLightDir),normalize(vVaryingNormal)));//反射角(反射光角度)， 負號是因為我們要的是物體到光源的向量
								
    //float spec = max(0.0, dot(normalize(vVaryingNormal), vReflection));
	float spec = max(0.0, dot(viewDir, vReflection) );
    if(diff != 0) {
		spec = pow(spec, Shininess);
		vFragColor += specularColor*vec4(Material.Ka,1)*spec; //一樣specular的結果 = 光打到角度不同造成的強弱光 * 光色 * 物體對光的反應
    }

	//測試各種變色情況
	if(mode == 2){
		vFragColor = 1.0 - vFragColor;//逆顏色
	}
	else if(mode == 3){
		float avg = (vFragColor.r + vFragColor.g + vFragColor.b)/3.0;//灰階
		vFragColor = vec4(avg, avg, avg, 1.0);
	}
	else if(mode == 4){ // 煥彩

		vFragColor *= vec4(sin(time),cos(time), cos(tan(time)), 1.0);
	}
	else if(mode == 5){ // 鏡子
		float ratio = 1.00 / 1.52;
		//vec3 I = normalize(Position - cameraPos);
		vec3 I = -viewDir; // viewDir = normalize(viewPos - vPosition)
		vec3 R = refract(I, normalize(vVaryingNormal), ratio);
		vFragColor = vec4(texture(skybox, R).rgb, 1.0);
	}
	else if(mode == 6){ // 新增卡通特效
		float diffuse = dot(normalize(vLight), normalize(vVaryingNormal));
		if (diffuse > 0.8) {
			diffuse = 0.9;
		}
		else if (diffuse > 0.5) {
			diffuse = 0.505;
		}
		else if (diffuse > 0.2) {
			diffuse = 0.205;
		}
		else {
			diffuse = 0.005;
		}

		vFragColor *= diffuse;
	}
	else if (mode == 7){	//black white block
		vec2 position = UV * vec2(2) + vec2(-1);
		vec2 q = vec2(0.1 * sin(0.05*time), 0.4*cos(time*.1));
		float i = 6.7 * sin( time * 0.1 + sin(time*0.21) );
		vec2 c = vec2( 0.1*sin( distance(position, vec2(0.1,0.3)) * i + time * .1), 0.1*sin( (position.x+position.y)*i ) );
		float h = mod( float(int(mod(distance(q,position+c)*20.0,2.0)) + 2 * int(mod(distance(-q,position+c)*20.0,2.0))),3.0) / 2.0; 
		vFragColor = vec4(h,h,h,1.0);
	}
	else if (mode == 8){	//sin wave
		vec2 p = UV -0.5;
		float sx = 0.3 * (p.x*p.x*8.0 - 0.7) * cos( 123.0 * p.x - 15. * pow(time*0.3, 0.7)*9.);
		float dy = 9./ ( 423. * abs(p.y - sx));
		dy += 11./ (200. * length(p - vec2(p.x, 0.0)));
		vFragColor = vec4( (p.x + 0.2) * dy, 0.3 * dy, dy, 6.1 );
	}
	else if (mode == 9){	//laser
		vec2 p = UV -0.5;
		float sx = 0.3 * (p.x*p.x*87.0 - 1.7) * tan( 45.0 * p.y + 45. * pow(time/0.025, 0.06)*9.);
		float dy = 9./ ( 423. * abs(p.y - sx));
		dy += 30./ (11900. * length(p - vec2(p.x, 0.02)));
		vFragColor = mix(vFragColor ,vec4( 0.8 * dy, dy,(p.x + 3.56) * dy, 1.1 ), 0.2);
	}
	else if (mode == 10){//red block
		vec3 color = vec3(0,0,0);
		if(UV.x > sin(UV.x * 100. + time * 5.)) color = vec3(UV.x - UV.y/1.,0,0);
		vFragColor = vec4(color, 4.);
	
	}

	else if (mode == 11){ //color block
		float pi = 3.14159265359;
		vec2 position = UV;
		float ang = atan(position.y, position.x);
		float dist = length(position);
		vFragColor.rgb = vec3(0.5, 0.5, 0.5) * (pow(dist, -1.0) * 0.05);
		for (float ray = 0.0; ray < 18.0; ray += 1.0) {
			float rayang = (((ray) / 9.0) * 3.14) + (time * 0.1);
			rayang = mod(rayang, pi*2.0);
			if (rayang < ang - pi) {rayang += pi*1.0;}
			if (rayang > ang + pi) {rayang -= pi*2.0;}
			float brite = 0.3 - abs(ang - rayang);
			brite -= dist * 0.2;
			if (brite > 0.0) vFragColor.rgb += vec3(sin(ray)+1.0, sin(ray+2.0)+1.0, sin(ray+4.0)+1.0) * brite;
		}
	}


}

mat2 rotate(float a) 
{
	float c = cos(a),
	s = sin(a);
	return mat2(c, -s, s, c);
}

float rnd(float x)
{
   return fract(sin(dot(vec2(x + 47.49, 38.2467 / (x + 2.3)),vec2(12.9898, 78.233))) * (43758.5453));
              
}