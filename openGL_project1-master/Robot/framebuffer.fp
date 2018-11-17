#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

//選擇哪一種模式
uniform int mode;
const float offset = 1.0 / 300.0; // from learnOpenGL
const vec2 texSize = vec2(800., 600.); // for pixelize
const vec2 mosaicSize = vec2(8., 8.);
const highp vec3 W = vec3(0.2125, 0.7154, 0.0721); // for 雕刻畫
const vec4 bkColor = vec4(0.5, 0.5, 0.5, 1.0);
uniform float len;//六邊形的邊長

uniform float time;
const float step_w = 0.0015625;//水波紋 
const float step_h = 0.0027778; 
const float maxW   = 0.04; 

float PI = 3.14159265359;//霧閃電
const vec2 resolution = vec2(800., 600.);

const float aoinParam1 = 0.7;
vec3 hsv2rgb(vec3 c);
float snow(vec2 uv,float scale);

#define _SnowflakeAmount 400//雪花飄   
#define _BlizardFactor 0.25           

const vec2 iResolution = vec2(512., 512.);
float rnd(float x);
float drawCircle(vec2 uv, vec2 center, float radius);
mat2 rotate(float a);


void main()
{
    vec3 col = texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0);

	if(mode == 1){ // 反色系
		FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
	} 
	else if(mode == 2){ //灰化
		FragColor = texture(screenTexture, TexCoords);
		float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
		FragColor = vec4(average, average, average, 1.0);
	}
	else if(mode == 3){ // 框加深，使用九宮格kernel作權重
		vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
		);

		float kernel[9] = float[](
			-1, -1, -1,
			-1,  9, -1,
			-1, -1, -1
		);
    
		vec3 sampleTex[9];
		for(int i = 0; i < 9; i++)
		{
			sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
		}
		vec3 col = vec3(0.0);
		for(int i = 0; i < 9; i++)
			col += sampleTex[i] * kernel[i];
    
		FragColor = vec4(col,1.0);
		/*
		float tmp= (col.x+col.y+col.z)/3;
		FragColor = vec4(tmp,tmp,tmp, 1.0);
		*/
	}
	else if(mode == 4){ // 霧化，也是用相鄰的格子作權重
		vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
		);

		float kernel[9] = float[](
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16  
		);
    
		vec3 sampleTex[9];
		for(int i = 0; i < 9; i++)
		{
			sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
		}
		vec3 col = vec3(0.0);
		for(int i = 0; i < 9; i++)
			col += sampleTex[i] * kernel[i];
    
		FragColor = vec4(col, 1.0);
	}
	else if(mode == 5){ // 黑背彩邊，一樣用權重的方式
		vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
		);

		float kernel[9] = float[](
			1, 1,  1,
			1, -9, 1,
			1, 1,  1
		);
    
		vec3 sampleTex[9];
		for(int i = 0; i < 9; i++)
		{
			sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
		}
		vec3 col = vec3(0.0);
		for(int i = 0; i < 9; i++)
			col += sampleTex[i] * kernel[i];
    
		FragColor = vec4(col, 1.0);
	}
	else if(mode == 6){ // 圓形馬賽克

		// 取值範圍為整個螢幕的大小
		vec2 xy = vec2(TexCoords.x * texSize.x, TexCoords.y * texSize.y);
		
		// 單一馬賽克中心座標xyMosaic
		vec2 xyMosaic = vec2(floor(xy.x / mosaicSize.x) * mosaicSize.x, floor(xy.y / mosaicSize.y) * mosaicSize.y ) + 0.5*mosaicSize;
         
		// 馬賽克距離中心的長度delL      
		vec2 delXY = xyMosaic - xy;
		float delL = length(delXY);
		
		//再換回TexCoords
		vec2 uvMosaic = vec2(xyMosaic.x / texSize.x, xyMosaic.y / texSize.y);
		
		// 如果馬賽克距離中心的長度小於0.5，代表在馬賽克裡面，畫出馬賽克
		vec4 finalColor;
		if( delL < 0.5 * mosaicSize.x )
		{
			finalColor = texture2D(screenTexture, uvMosaic);
		}
		else // 如果不是，就畫出黑色部分
		{
			//finalColor = texture2D(screenTexture, TexCoords);
			finalColor = vec4(0., 0., 0., 1.);
		}
   
		FragColor = finalColor;

	}
	else if(mode == 7){ //浮雕
		vec2 tex = TexCoords;
		vec2 upLeftUV = vec2(tex.x-1.0/texSize.x, tex.y-1.0/texSize.y);
		vec4 curColor = texture2D(screenTexture, TexCoords);
		vec4 upLeftColor = texture2D(screenTexture, upLeftUV);
		vec4 delColor = curColor - upLeftColor;
		float luminance = dot(delColor.rgb, W);
		FragColor = vec4(vec3(luminance), 0.0) + bkColor;
	}

	else if(mode == 8){ // 動態三角馬賽克
		const float TR = 0.866025;  // .5*(3)^.5
		const float PI6 = 0.523599; // PI/6
   
		float x = TexCoords.x;
		float y = TexCoords.y;
   
		
		int wx = int(x/(1.5*len));
		int wy = int(y/(TR*len));
   
		vec2 v1, v2, vn;
   
		
		if(wx/2 * 2. == wx) 
		{
			if(wy/2 * 2. == wy) 
			{   // left top
				// 左上右下
				v1 = vec2(len*1.5*wx, len*TR*wy);
				v2 = vec2(len*1.5*(wx+1), len*TR*(wy+1));
			} 
			else 
			{   // left bottom
				// 左下右上
				v1 = vec2(len*1.5*wx, len*TR*(wy+1));
				v2 = vec2(len*1.5*(wx+1), len*TR*wy);
			}
		} 
		else 
		{
			if(wy/2 * 2. == wy) 
			{   // right top
				// 左下右上
				v1 = vec2(len*1.5*wx, len*TR*(wy+1));
				v2 = vec2(len*1.5*(wx+1), len*TR*wy);
			} 
			else 
			{  // right bottom
				// 左上右下
				v1 = vec2(len*1.5*wx, len*TR*wy);
				v2 = vec2(len*1.5*(wx+1), len*TR*(wy+1));
			}
		}

		float s1 = sqrt( pow(v1.x-x, 2.) + pow(v1.y-y, 2.) );
		float s2 = sqrt( pow(v2.x-x, 2.) + pow(v2.y-y, 2.) );

		if(s1 < s2)
			vn = v1;
		else
			vn = v2;
      
		vec4 mid = texture2D(screenTexture, vn);
		float a = atan((x-vn.x)/(y-vn.y));

		vec2 area1 = vec2(vn.x, vn.y-len*TR/2.);
		vec2 area2 = vec2(vn.x+len/2., vn.y-len*TR/2.);
		vec2 area3 = vec2(vn.x+len/2., vn.y+len*TR/2.);
		vec2 area4 = vec2(vn.x, vn.y+len*TR/2.);
		vec2 area5 = vec2(vn.x-len/2., vn.y+len*TR/2.);
		vec2 area6 = vec2(vn.x-len/2., vn.y-len*TR/2.);
   

		if(a >= PI6 && a < PI6*3.)
			vn = area1;
		else if(a>=PI6*3. && a<PI6*5.)
			vn = area2;
		else if((a>=PI6*5. && a<=PI6*6.) || (a<-PI6*5. && a>-PI6*6.))
			vn = area3;
		else if(a<-PI6*3. && a>=-PI6*5.)
			vn = area4;
		else if(a<=-PI6 && a>-PI6*3.)
			vn = area5;
		else if(a>-PI6 && a<PI6)
			vn = area6;
      
		//vec4  color = texture2D(screenTexture, vn);
		vec4  color = texture(screenTexture, vn);
		FragColor = color;
	}

	else if(mode == 9){ // 方形馬賽克
		vec4 color;
		//float ratio = texSize.y/texSize.x;

		/** ratio */
		vec2 xy = vec2(TexCoords.x * texSize.x , TexCoords.y * texSize.y);
		
		vec2 xyMosaic = vec2(floor(xy.x / mosaicSize.x) * mosaicSize.x, floor(xy.y / mosaicSize.y) * mosaicSize.y );
         
   
		
		vec2 xyFloor = vec2(floor(mod(xy.x, mosaicSize.x)), floor(mod(xy.y, mosaicSize.y)));
                  
		//#if 0
		if((xyFloor.x == 0 || xyFloor.y == 0))
		{
			color = vec4(0., 0., 0., 1.);
		}
		else
		//#endif
		{
			vec2 uvMosaic = vec2(xyMosaic.x / texSize.x, xyMosaic.y / texSize.y);
			color = texture2D( screenTexture, uvMosaic );
		}
   
		FragColor = color;

	}

	else if(mode == 10){ // Done At Home1 : 素描畫特效
		vec4 curColor = texture2D(screenTexture,TexCoords);
		
		float h = 0.299*curColor.x + 0.587*curColor.y + 0.114*curColor.z;
		vec4 fanshe = vec4(h,h,h,0.0);

		
		vec4 sample0,sample1,sample2,sample3;
		float h0,h1,h2,h3;
		float fstep=0.0015;
		sample0=texture2D(screenTexture,vec2(TexCoords.x-fstep,TexCoords.y-fstep));
		sample1=texture2D(screenTexture,vec2(TexCoords.x+fstep,TexCoords.y-fstep));
		sample2=texture2D(screenTexture,vec2(TexCoords.x+fstep,TexCoords.y+fstep));
		sample3=texture2D(screenTexture,vec2(TexCoords.x-fstep,TexCoords.y+fstep));
		
		h0 = 0.299*sample0.x + 0.587*sample0.y + 0.114*sample0.z;
		h1 = 0.299*sample1.x + 0.587*sample1.y + 0.114*sample1.z;
		h2 = 0.299*sample2.x + 0.587*sample2.y + 0.114*sample2.z;
		h3 = 0.299*sample3.x + 0.587*sample3.y + 0.114*sample3.z;
		
		sample0 = vec4(1.0-h0,1.0-h0,1.0-h0,0.0);
		sample1 = vec4(1.0-h1,1.0-h1,1.0-h1,0.0);
		sample2 = vec4(1.0-h2,1.0-h2,1.0-h2,0.0);
		sample3 = vec4(1.0-h3,1.0-h3,1.0-h3,0.0);
		
		vec4 color=(sample0+sample1+sample2+sample3) / 4.0;
		
		//vec3 endColor = fanshe.rgb+(fanshe.rgb*color.rgb)/(1.0-color.rgb);
		
		vec3 endColor = fanshe.rgb + ( fanshe.rgb * color.rgb )/( 1.0 - color.rgb );

		FragColor = vec4(endColor,0.0);


	}
	else if(mode == 11){ // 新增橫形水波文效果
		float x = TexCoords.x - maxW*sin((TexCoords.y*80.0 - time*7.50)); 
		x = fract(x); 
		vec3 color = texture2D(screenTexture, vec2(x, TexCoords.y)).rgb; 

		FragColor.a = 1.0; 
		FragColor.rgb = color; 
	}

	else if(mode ==12){ // 新增霧閃電特效
		
		vec2 position = (( gl_FragCoord.xy / resolution.xy ) - 0.5);
		position.x *= resolution.x / resolution.y;

		vec3 color = texture(screenTexture, TexCoords).rgb;

		for (float i = 0.; i < PI * 2.0; i += PI/20.0) {
			vec2 p = position - vec2(cos(i), sin(i)) * 0.15;
			vec3 col = hsv2rgb(vec3((i + time)/(PI*2.0), 1., 1));
			color += col * (2./512.) / length(p);
		}


		vec2 uv=(gl_FragCoord.xy*2.-resolution.xy)/min(resolution.x,resolution.y); 
		vec3 finalColor=vec3(0);
		float c=smoothstep(1.,0.3,clamp(uv.y*.3+.8,0.,.75));
		c+=snow(uv,30.)*.3;
		c+=snow(uv,20.)*.5;
		c+=snow(uv,15.)*10.8;
		c+=snow(uv,10.);
		c+=snow(uv,8.);
		c+=snow(uv,6.);
		c+=snow(uv,5.);
		finalColor=(vec3(c));
		FragColor = (vec4( color, 1.0 ) + vec4(finalColor,1)) / vec4(2, 2, 2, 1);

	}
	else if(mode == 13){ // 雪花飄
		vec2 uv = TexCoords.xy;
		float j;
		for (int i = 0; i < _SnowflakeAmount; i++){
			j = float(i);
			float speed = 0.3+ rnd(cos(j))* (0.7+ 0.5* cos(j/ (float(_SnowflakeAmount)* 0.25)));				
			vec2 center = vec2((-0.25 + uv.y) * _BlizardFactor + rnd(j)+ 0.1 * cos(time + sin(j)),mod(rnd(j)- speed* (time * 1.5 * (0.1 + _BlizardFactor)), 0.95) );       
			FragColor += vec4(0.9 * drawCircle(uv, center, 0.001 + speed * 0.012));
		}

	}
	else if(mode == 14){ // 旋轉的環

		
		float t;
		t = time * 1.0;
		vec2 r = resolution,
		o = gl_FragCoord.xy - r/2.;
		o = vec2(length(o) / r.y - .3, atan(o.y,o.x));    
		vec4 s = 0.07*cos(1.5*vec4(0,1,2,3) + t + o.y + cos(o.y) * cos(t)),
		e = s.yzwx, 
		f = max(o.x-s,e-o.x);

		FragColor = mix(FragColor ,dot(clamp(f*r.y,0.,1.), 72.*(s-e)) * (s-.1) + f, 0.2);

	}
	else if(mode == 15){ // 雷射
		vec2 p = ( gl_FragCoord.xy / resolution.xy ) -0.5;
	
		float sx = 0.3 * (p.x*p.x*87.0 - 1.7) * tan( 45.0 * p.y + 45. * pow(time/0.025, 0.06)*9.);
	
		float dy = 9./ ( 423. * abs(p.y - sx));
	
		dy += 30./ (11900. * length(p - vec2(p.x, 0.02)));
	
	
		FragColor = mix(FragColor ,vec4( (p.x + 3.56) * dy, 0.8 * dy, dy, 1.1 ), 0.2);
	}
	else if(mode == 16){ // 黃藍幻象
		vec2 p = ( gl_FragCoord.xy / resolution.xy );
		p = 2.0 * p - 1.0;
		p.x *= resolution.x / resolution.y;
		p*=0.1;
		float color = 0.0;
		float d0 = (length(p));
		vec2 q = mod(sin(p * 3.141592 * 2.0) - 0.5, 1.0) - 0.5;
		vec2 r = mod(cos(q * 3.141592 * 3.0) - 0.5, 1.0) - 0.5;
		float d = length(d0);
		float dr = length(r);
		float w1 = cos(time - 15.0 * d * 3.141592) * 2. ;
		float w2 = sin(-10.4 * dr * 3.141592*sin(d*9. - dr*w1*3.3 + w1*d0 + time*.3)) * 1. ;
	
		color = w1*1.0-w2*1.-d*d0;
		FragColor = mix(FragColor ,vec4( vec3( -color, abs(color) * 0.5, cos( color + time * 2.0 ) * 0.75 ), 1.0 ), 0.2);
	}
	else if(mode == 17){	//mid sin wave
		vec2 p = ( gl_FragCoord.xy / resolution.xy ) -0.5;
		float sx = 0.3 * (p.x*p.x*8.0 - 0.7) * cos( 123.0 * p.x - 15. * pow(time*0.3, 0.7)*9.);
		float dy = 9./ ( 423. * abs(p.y - sx));
		dy += 11./ (200. * length(p - vec2(p.x, 0.0)));
		FragColor *= vec4( (p.x + 0.2) * dy, 0.3 * dy, dy, 6.1 );

	}
	else if(mode == 18){ // 閃爍藍愛心
		vec2 p = ( gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x,resolution.y);
		vec3 destColor = vec3(0.0,0.9,1.3);
		float f = 0.0;
		float T = 3.0 * time+dot(p,p)*resolution.x*resolution.y;
		for ( float i = 0.0; i < 12.0; i++){
			T += 0.0131415926;
			float c = 16.*pow(sin(T), 3.);//sin(time*3.0 + i * 0.0031415926) * 0.8;
			float s = 13.*cos(T) - 5.*cos(2.*T) - 2.*cos(3.*T) - cos(4.0*T);
			c = 0.05*c; s = -0.05*s;
			f += 0.001/abs(length(p+vec2(c,s))-i/500000.)*(pow(i,2.0)/1000.0);
			f += 0.001/abs(length(p+vec2(-c,s))-i/500000.)*(pow(i,2.0)/1000.0);
		}
	
		FragColor = mix(FragColor, vec4(vec3(destColor*f*f*50000.),1.0), 0.2);
	}
	
	
	else if(mode == 19){ //閃耀夜店燈
		
		vec2 uv = 3.6*(2. * gl_FragCoord.xy - resolution) / resolution.y;
		vec3 color = vec3(0);
		if (uv.y < 0.0)   // horizontal mirroring
		{ 
			uv.y *= 4.0;
			color = vec3(-0.3);  // darker
		}
		vec3 rd = vec3(uv, 1.);
		
		float s = 0.5;
		for (int i = 0; i < 8; i++) {
			rd = abs(rd) / dot(rd, rd); // kali iteration!! Thanks Kali
			rd -= s;
			rd.xy *= rotate(0.1 + time *0.110);
			rd.xz *= rotate(0.2 - time *0.123);
			rd.zy *= rotate(0.3 + time *0.131);
			s *= 0.8;
			float b = 0.005;
			color.gb += .014 / max(abs(rd.x*0.8), abs(rd.y*0.8));
			color.rb += .015 / max(abs(rd.y*0.6), abs(rd.z*0.6));
			color.rg += .010 / max(abs(rd.x*0.7), abs(rd.z*0.9));
		}
		color *= 0.4;
		FragColor = mix(FragColor, vec4(color, 1.), 0.4);
	}
	else if(mode == 20){ //half circle
		vec2 r = resolution,
		o = gl_FragCoord.xy - r/3.;
		o = vec2(length(o) / r.y - .3, atan(o.y,o.y));    
		vec4 s = .1*cos(1.6*vec4(0,1,2,3) + time + o.y + sin(o.y) * sin(time)*2.),
		e = s.yzwx, 
		f = min(o.x-s,e-o.x);
		FragColor += dot(clamp(f*r.y,0.,1.), 40.*(s-e)) * (s-.1) - f;
	}
	else if(mode == 21){ //震動的圓形
		// Visual parameters (for making things look pretty)
		float initialDensity = 0.01;  	 	// Pattern starting density
		float brightness = 0.5; 		// Maximum pattern brightness 
		float blackLevel = 0.5; 		// Minimum pattern darkness 
	
		// Half the width and half the height gives the position of the center of the screen
		vec2 screenCenter = (vec2(0.5,0.5)+sin(0.02*time)*vec2(0.5,0.0))*resolution; 		
	
		// The current pixel position should be given assuming the screen center is [0,0] 
		vec2 position = gl_FragCoord.xy - screenCenter;		
	
		// Compute squares the current x and y pixel positions and call them x2 and y2
		float x2 = position.x * position.x; 	// x squared
		float y2 = position.y * position.y;	// y squared
		
		// Each pixel value is defined by sin(x2 + y2) 
		// (Multiply the inside of the sin by the current time value to make it zoom in and out)
		float pixelValue = sin(time*initialDensity*(x2 + y2)) * brightness + blackLevel;
	
		// For each pixel, set all its red, green, and blue channels with this technique (making things black and white) 
		
		FragColor += vec4(pixelValue*abs(sin(0.1*time+2.0)),pixelValue*abs(sin(0.1*time)),pixelValue*abs(sin(0.1*time+4.0)),1.0);
	}
	else if (mode == 22) { // 藍黃圓形
		vec2 position = ( gl_FragCoord.xy / resolution.xy );
		float color = 0.0;
		color += sin( position.x * cos( time / 15.0 ) * 80.0 ) + cos( position.y * cos( time / 15.0 ) * 10.0 );
		color += sin( position.y * sin( time / 10.0 ) * 40.0 ) + cos( position.x * sin( time / 25.0 ) * 40.0 );
		color += sin( position.x * sin( time / 5.0 ) * 10.0 ) + sin( position.y * sin( time / 35.0 ) * 80.0 );
		color *= sin( time / 10.0 ) * 0.5;
		FragColor += vec4( vec3( color, color * 0.5, sin( color + time / 3.0 ) * 0.75 ), 0.7);
	}

	else if (mode == 23) { // 藍射線
		vec2 uv = (gl_FragCoord.xy * 2.0 -  resolution.xy) / resolution.x;
		uv *= rotate(time * 0.2); 
		//time * 0.2
		float direction = 1.0;
		float speed = time * direction * 1.6;
		float distanceFromCenter = length(uv);
		float meteorAngle = atan(uv.y, uv.x) * (180.0 / PI);
		float flooredAngle = floor(meteorAngle);
		float randomAngle = pow(rnd(flooredAngle), 0.5);
		float t = speed + randomAngle;
		float lightsCountOffset = 0.4;
		float adist = randomAngle / distanceFromCenter * lightsCountOffset;
		float dist = t + adist;
		float meteorDirection = (direction < 0.0) ? -1.0 : 0.0;
		dist = abs(fract(dist) + meteorDirection);
		float lightLength = 100.0;
		float meteor = (5.0 / dist) * cos(sin(speed)) / lightLength;
		meteor *= distanceFromCenter * 2.0;

		vec3 color = vec3(0.);
		color.gb += meteor;

		FragColor += vec4(color, 0.0);
	}

}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 4.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float snow(vec2 uv,float scale)
{
	float w = smoothstep( 9.,0., -uv.y*(scale/10.));
	if(w<.1)return 0.;
	uv+=(time*aoinParam1)/scale;
	uv.y+=time*0./scale;
	uv.x+=sin(uv.y+time*.05) / scale;
	uv*=scale;
	vec2 s=floor(uv),f=fract(uv),p;float k=3.,d;
	p=.5+.35*sin(11.*fract(sin((s+p+scale)*mat2(7,3,6,5))*5.))-f;d=length(p);
	k=min(d,k);
	k=smoothstep(0.,k,sin(f.x+f.y)*0.01);
    return k*w;
}


float rnd(float x)
{
   return fract(sin(dot(vec2(x + 47.49, 38.2467 / (x + 2.3)),vec2(12.9898, 78.233))) * (43758.5453));
              
}
 
float drawCircle(vec2 uv, vec2 center, float radius)
{
   return 1.0 - smoothstep(0.0, radius, length(uv - center));
}

mat2 rotate(float a) 
{
	float c = cos(a),
	s = sin(a);
	return mat2(c, -s, s, c);
}