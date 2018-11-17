#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform int mode;
uniform float time;
const vec2 resolution = vec2(800., 600.);

//---------------[mode 6]-------------------------------

float iTime;
vec3  iResolution;
mat3 getRotZMat(float a){return mat3(cos(a),-sin(a),0.,sin(a),cos(a),0.,0.,0.,1.);}
float dstepf = 0.0;
float map(vec3 p)
{
	p.x += sin(p.z*1.8);
    p.y += cos(p.z*.2) * sin(p.x*.8);
	p *= getRotZMat(p.z*0.8+sin(p.x)+cos(p.y));
    p.xy = mod(p.xy, 0.3) - 0.15;
	dstepf += 0.003;
	return length(p.xy);
}
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 uv = (fragCoord - iResolution.xy*.5 )/iResolution.y;
    vec3 rd = normalize(vec3(uv, (1.-dot(uv, uv)*.5)*.5)); 
    vec3 ro = vec3(0, 0, iTime*1.26), col = vec3(0), sp;
	float cs = cos( iTime*0.375 ), si = sin( iTime*0.375 );    
    rd.xz = mat2(cs, si,-si, cs)*rd.xz;
	float t=0.06, layers=0., d=0., aD;
    float thD = 0.02;
	for(float i=0.; i<250.; i++)	
	{
        if(layers>15. || col.x > 1. || t>5.6) break;
        sp = ro + rd*t;
        d = map(sp); 
        aD = (thD-abs(d)*15./16.)/thD;
        if(aD>0.) 
		{ 
            col += aD*aD*(3.-2.*aD)/(1. + t*t*0.25)*.2; 
            layers++; 
		}
        t += max(d*.7, thD*1.5) * dstepf; 
	}
    col = max(col, 0.);
    col = mix(col, vec3(min(col.x*1.5, 1.), pow(col.x, 2.5), pow(col.x, 12.)), 
              dot(sin(rd.yzx*8. + sin(rd.zxy*8.)), vec3(.1666))+0.4);
    col = mix(col, vec3(col.x*col.x*.85, col.x, col.x*col.x*0.3), 
             dot(sin(rd.yzx*4. + sin(rd.zxy*4.)), vec3(.1666))+0.25);
	fragColor = vec4( clamp(col, 0., 1.), 1.0 );
}


//---------------[mode 4]-------------------------------

float DEG2RAG = .0174532925199;
float tri2(vec2 pos)
{
	float a = dot(pos, vec2(sin( 60.0*DEG2RAG), cos( 60.0*DEG2RAG)));
	float b = dot(pos, vec2(sin(-60.0*DEG2RAG), cos(-60.0*DEG2RAG)));	
	float c = -pos.y;
	float d = 0.2*cos(60.0*DEG2RAG);
			
	float u1 = a - d*0.25;
	float v1 = b - d*0.25;
	float w1 = c - d*0.25;
	
	float u2 = c + d*1.25;
	float v2 = b + d*1.25;
	float w2 = a + d*1.25;
	
	float t1 = min(u2, min(u1, v1));
	float t2 = min(v2, min(w1, u1));
	float t3 = min(w2, min(v1, w1));
	
	float res = max(max(t1, t2), t3);
	return 0.002/abs(res);
}
float tri(vec2 pos)
{
	float a = dot(pos, vec2(sin( 60.0*DEG2RAG), cos( 60.0*DEG2RAG)));
	float b = dot(pos, vec2(sin(-60.0*DEG2RAG), cos(-60.0*DEG2RAG)));	
	float c = -pos.y;
				
	float result = 0.002/abs(min(min(0.05-a,0.05-b), 0.05-c));
	return result;
}

//---------------[mode 5]-------------------------------
// replace shadertoy uniforms with glslsandbox
#define R resolution
#define T time

// toggle for psychedelic madness
#define ENABLE_COLOR_CYCLE 1

// FabriceNeyret2 
#define hue(v)  (.5 + cos(6.3 * (v) + vec4(0, 23, 21, 0)))

int id = -1;

mat2 rotate(float a) {
	float c = cos(a),
		s = sin(a);
	return mat2(c, s, -s, c);
}

float random(in vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.1);
}

float noise(vec2 p) {
	vec2 i = ceil(p);
    vec2 f = fract(p);
    vec2 u = f * f * (3. - 2. * f);
   	float a = random(i);
    float b = random(i + vec2(1., 0.));
    float c = random(i + vec2(0., 1.));
    float d = random(i + vec2(1., 1.));
    return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}

float fbm(in vec2 p) { 
	float s = .0;
	float m = .0;
	float a = .5;	
	for(int i = 0; i < 8; i++) {
		s += a * noise(p);
		m += a;
		a *= .5;
		p *= 2.;
	}
	return s / m;
}

vec3 renderFractal(vec2 uv) {

    vec3 color = vec3(0.);
    vec2 p = uv;
	
    // per channel iters
    float t = T;
    for (int c = 0; c < 3; c++) {
    
        t += .1; // time offset per channel
        
		float l = 0.;
        float s = 1.;
        for (int i = 0; i < 8; i++) {
            // from Kali's fractal iteration
            p = abs(p) / dot(p, p);
            p -= s;
            p *= rotate(t * .5);
            s *= .8;
            l += (s  * .08) / length(p);
        }
        color[c] += l;
    
    }

	return color;

}

float map2(vec3 p) {
	
    float m = 1000.;
    
    vec3 q = p;
    float k = fbm(q.xz + fbm(q.xz + T *2.));
   	
    q.y += .1;
    float d = dot(q, vec3(0., 1., 0.)) + k;
	d = min(5. - d, d);
    if (d < m) { 
        m = d;
        id = 1;
    }
    
    q = p;
    q.xz = mod(q.xz + 2., 4.) - 2.;
    d = min(d, length(q.xz) - .5);
    if (d < m) { 
        m = d;
        id = 2;
    }
    
    return m;
}

vec3 render(vec3 ro, vec3 rd) {

    vec3 col = vec3(0.);
	vec3 p;
    
	float t = 0.;
	for (int i = 0; i < 256; i++) {
		p = ro + rd * t;
		float d = map2(p);
		if (d < .001 || t > 50.) break;
		t += .5 * d;
#if ENABLE_COLOR_CYCLE 
        col += .02 * hue(d * .5 + T * .4).rgb;
#else
        col += .02 * hue(d).rgb;
#endif
	}
	
    vec3 tex =  renderFractal(fract(.1 * p.xz) - .5);
    if (id == 1) col += tex / (1. + t * t * .5);
    if (id == 2) col += abs(.1 / sin(10. * p.y + T)) * vec3(0., 1., 1.);
    
	return col;

}

void mainImage2(out vec4 O, vec2 I) {

    vec2 uv = (2. * I - R)
        / R.y;
	vec3 col = vec3(0.);
	
	vec3 ro = vec3(2., 1., T * 2.);
	vec3 rd = vec3(uv, 1.);
	
    vec3 pc = render(ro, rd);
    
    O = vec4(pc, 1.);
}

//---------------[mode 6]-------------------------------

float N21(vec2 p) {
	p = fract(p * vec2(233.34, 851.73));
    p += dot(p, p + 23.45);
    return fract(p.x * p.y);
}

vec2 N22(vec2 p) {
	float n = N21(p);
    return vec2(n, N21(p + n));
}

vec2 getPos(vec2 id, vec2 offset) {
	vec2 n = N22(id + offset);
    float x = cos(iTime * n.x);
    float y = sin(iTime * n.y);
    return vec2(x, y) * 0.4 + offset;
}

float distanceToLine(vec2 p, vec2 a, vec2 b) {
	vec2 pa = p - a;
    vec2 ba = b - a;
    float t = clamp(dot(pa, ba) / dot(ba, ba), 0., 1.);
    return length(pa - t * ba);
}

float getLine(vec2 p, vec2 a, vec2 b) {
	float distance = distanceToLine(p, a, b);
    float dx = 15./iResolution.y;
    return smoothstep(dx, 0., distance) * smoothstep(1.2, 0.3, length(a - b));
}

float layer(vec2 st) {
    float m = 0.;
    vec2 gv = fract(st) - 0.5;
    vec2 id = floor(st);
    // m = gv.x > 0.48 || gv.y > 0.48 ? 1. : 0.;
    // vec2 pointPos = getPos(id, vec2(0., 0.));
    // m += smoothstep(0.05, 0.03, length(gv - pointPos));
    
    float dx=15./iResolution.y;
    // m += smoothstep(-dx,0., abs(gv.x)-.5);
    // m += smoothstep(-dx,0., abs(gv.y)-.5);
    // m += smoothstep(dx, 0., length(gv - pointPos)-0.03);
    
    vec2 p[9];
    p[0] = getPos(id, vec2(-1., -1.));
    p[1] = getPos(id, vec2(-1.,  0.));
    p[2] = getPos(id, vec2(-1.,  1.));
    p[3] = getPos(id, vec2( 0., -1.));
    p[4] = getPos(id, vec2( 0.,  0.));
    p[5] = getPos(id, vec2( 0.,  1.));
    p[6] = getPos(id, vec2( 1., -1.));
    p[7] = getPos(id, vec2( 1.,  0.));
    p[8] = getPos(id, vec2( 1.,  1.));
    
    for (int j = 0; j <= 8; j++) {
    	m += getLine(gv, p[4], p[j]);
        vec2 temp = (gv - p[j]) * 20.;
        m += 1./dot(temp, temp) * (sin(10. * iTime + fract(p[j].x) * 20.) * 0.5 + 0.5);
        
    }
    
    m += getLine(gv, p[1], p[3]);
    m += getLine(gv, p[1], p[5]);
    m += getLine(gv, p[3], p[7]);
    m += getLine(gv, p[5], p[7]);
    
    // m += smoothstep(0.05, 0.04, length(st - vec2(0., 0.)));
    return m;
}

void mainImage3( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = (fragCoord - 0.5 * iResolution.xy) / iResolution.y;
    
    float m = 0.;
    
    float theta = iTime * 0.1;
    mat2 rot = mat2(cos(theta), -sin(theta), sin(theta), cos(theta));
    vec2 gradient = uv;
    uv = rot * uv;
    
    for (float i = 0.; i < 1.0 ; i += 0.25) {
    	float depth = fract(i + iTime * 0.1);
        m += layer(uv * mix(10., 0.5, depth) + i * 20.) * smoothstep(0., 0.2, depth) * smoothstep(1., 0.8, depth);
    }
    
    vec3 baseColor = sin(vec3(3.45, 6.56, 8.78) * iTime * 0.2) * 0.5 + 0.5;
    
    vec3 col = (m - gradient.y) * baseColor;
    // Output to screen
    fragColor = vec4(col, 1.0);
}

//---------------[mode 7]-------------------------------

#define OCT 6
#define ITER 40
#define EPS 0.001
#define NEAR .3
#define FAR 11.

vec3 rotX(vec3 p,float a){return vec3(p.x,p.y*cos(a)-p.z*sin(a),p.y*sin(a)+p.z*cos(a));}
vec3 rotY(vec3 p,float a){return vec3(p.x*cos(a)-p.z*sin(a),p.y,p.x*sin(a)+p.z*cos(a));}
vec3 rotZ(vec3 p,float a){return vec3(p.x*cos(a)-p.y*sin(a), p.x*sin(a)+p.y*cos(a), p.z);}
vec3 hsv(float h,float s,float v){return ((clamp(abs(fract(h+vec3(0.,.666,.333))*6.-3.)-1.,0.,1.)-1.)*s+1.)*v;}

float map3(vec3 p){
	float s=.98,df=1.;
	p=rotX(p,time*.17);p=rotY(p,time*.13);p+=1.;
	for(int i=0;i<OCT;i++){
		if(p.x>1.){p.x=2.-p.x;}else if(p.x<-1.){p.x=-2.-p.x;}
		if(p.y>1.){p.y=2.-p.y;}else if(p.y<-1.){p.y=-2.-p.y;}
		if(p.z>1.){p.z=2.-p.z;}else if(p.z<-1.){p.z=-2.-p.z;}
		float q=p.x*p.x+p.y*p.y+p.z*p.z;
		if(q<.25){p*=4.;df*=3.;}else if(q<1.){p*=1./q;df*=.9/q;}
		p*=s;p+=.2;df*=s;		
	}
	return (length(p)-1.55)/df;
}

float trace(vec3 ro,vec3 rd,out float n){float t=NEAR,d;
	for(int i=0;i<ITER;i++){d=map3(ro+rd*t);if(abs(d)<EPS||t>FAR)break;t+=step(d,1.)*d*.2+d*.5;n+=1.;}
	return min(t,FAR);}

//-------------------[mode 8]---------------

#define OCT2 5
#define ITER2 80
#define EPS2 .002
#define NEAR2 0.
#define FAR2 24.

float hex(vec3 p){vec3 q = abs(p);return max(q.z-.01,max((q.x*.866+q.y*.5),q.y)-.95);}

float map4(vec3 p){
	float s=.94,df=1.3;
	p=rotX(p,time*.11);
	p=rotZ(p,time*.27);
	for(int i=0;i<OCT2;i++){
		if(p.x>.7){p.x=1.76-p.x;}else if(p.x<-.88){p.x=-1.76-p.x;}
		if(p.y>.88){p.y=1.76-p.y;}else if(p.y<-.88){p.y=-1.76-p.y;}
		if(p.z>.88){p.z=1.76-p.z;}else if(p.z<-.88){p.z=-1.76-p.z;}
		float q=p.x*p.x+p.y*p.y+p.z*p.z;
		if(q<.16){p*=6.25;df*=6.25;}
		else if(q<1.02){p*=1.02/q;df*=1.02/q;}
		p*=s;df*=s;		
	}
	return (hex(p))/df;
}

float trace2(vec3 ro,vec3 rd,out float n){float t=NEAR2,d;
	for(int i=0;i<ITER2;i++){d=map4(ro+rd*t);if(abs(d)<EPS2||t>FAR2)break;t+=step(d,1.)*d*.2+d*.5;n+=1.;}
	return min(t,FAR2);}

//----------[main]---------------------

void main()
{    
	if(mode == 0){
		FragColor = texture(skybox, TexCoords);
	}
    else if(mode == 1){
		vec3 d = normalize(vec3((gl_FragCoord.xy - resolution.xy * .5) / resolution.x, .5));
		vec3 p, c, f, g=d, o, y=vec3(1.0,3.0,0.0);
 		o.y = 1.2*cos((o.x=0.3)*(o.z=time * 10.0));
		o.x -= cos(time) + 3.0;

		for( float i=.0; i<8.; i+=.01 ) {
			f = fract(c = o += d*i*.01); 
			p = floor( c )*.4;
			if( cos(p.z) + sin(p.x) > ++p.y ) {
	    		g = (f.y-.04*cos((c.x+c.z)*10.)>.7?y:f.x*y.yxz) / i;
				break;
			}
		}
		FragColor = vec4(g,1.0);
	}
	else if(mode == 2){
		vec2 p = ( gl_FragCoord.xy / resolution.xy );
		p = 2.0 * p - 1.0;
		p.x *= resolution.x / resolution.y;
		p*=0.07;
		float color = 0.0;
		float d0 = (length(p));
		vec2 q = mod(sin(p * 3.141592 * 2.0) - 0.5, 1.0) - 0.5;
		vec2 r = mod(cos(q * 3.141592 * 3.0) - 0.5, 1.0) - 0.5;
		float d = length(q);
		float dr = length(r);
		float w1 = sin(time + 5.0 * d * 3.141592) * 1. ;
		float w2 = cos(8.2 * dr * 3.141592*sin(d*33. - dr*w1*1.3 + w1*d0 + time*0.3)) * 1. ;
	
		color = w1*dr-w2*d +d0*dr;

		FragColor = vec4( vec3( -color*color*color*3., abs(color) * 0.5, sin( color + time / 3.0 ) * 0.75 ), 1.0 );
	}
	else if(mode == 3){
		iTime = time;
		iResolution = vec3(resolution, 0.0);

		mainImage(FragColor, gl_FragCoord.xy);
	}
	else if(mode == 4){
			vec2 pos = (2.0*gl_FragCoord.xy - resolution.xy)/max(resolution.x, resolution.y) / 16.0;
	
		//rotate & zoom
		mat2 rot = mat2(cos(time),-sin(time), 
						sin(time), cos(time));
		pos *= rot;
	
		float zoom = 1.0/(mod(time, 1.0)+1.0)*3.0-1.0;
		pos *= zoom;
	
		//effect
		vec3 result = vec3(0.0);
		for(int i = 0 ; i < 6 ; i++)
		{
			result += vec3(tri(pos), tri(-pos*0.5), tri2(pos));
			pos /= 0.25;
		}
		FragColor = vec4( mix(vec3(0.0, 0.0, 0.0), vec3(0.5, 0.0,1.0), result.x)
			+mix(vec3(0.0, 0.0, 0.0), vec3(0.5,0.25,0.0), result.y)
			+mix(vec3(0.0, 0.0, 0.0), vec3(0.0,0.5,0.1), result.z), 1.0 );
		
	}
	else if(mode == 5){
		mainImage2(FragColor, gl_FragCoord.xy);
	}
	else if(mode == 6){
		iTime = time;
		iResolution = vec3(resolution, 0.0);

		mainImage3(FragColor, gl_FragCoord.xy);
	}
	else if(mode == 7){
		vec2 uv=(gl_FragCoord.xy-.5*resolution.xy)/resolution.y;
		float n=0.,v=trace(vec3(.0,-1.,0),vec3(uv,-.7),n)*.3;n/=float(ITER);
		FragColor=vec4(mix(hsv(v+time*.05,.5,n),vec3(1),n),1);
	}
	else if(mode == 8){
		
		vec2 uv=(gl_FragCoord.xy-.5*resolution.xy)/resolution.y;
		float n=0.,v=trace2(vec3(0,1,sin(time*.3)*1.5-2.),vec3(uv,.5),n)*.05;n/=float(ITER2);
		FragColor=vec4(mix(hsv(1.15-n*.1,.9,v),vec3(1),(v*.2)*(n*5.)),1);
	}
	
}