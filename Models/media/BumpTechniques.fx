//--------------------------------------------------------------------------------------
// File: BumpTechniques.fx
// Autor: Bruno Evangelista (www.brunoevangelista.com)
//
// This shader implements many techniques to simulate detailed surfaces
//
// The technique currently implemented are:
//		- Mip-Mapping
//		- Texture Mapping
// 		- Normal Mapping
//		- Parallax Offset Mapping
//		- Relief Mapping
//		- Parallax Occlusion Mapping
//		- Cone Step Mapping
//		- Multi-Layer Cone Step Mapping
//		- Sphere Mapping
//
//--------------------------------------------------------------------------------------

/**
 - Theres a problem comparing the surfaces depth because of the tile
*/

// DON'T UNCOMMENT THE LINES BELOW
//#define NUM_ITERATIONS 16
//#define NUM_ITERATIONS_RELIEF1 11
//#define NUM_ITERATIONS_RELIEF2 5

bool DEPTH_BIAS = false;
bool BORDER_CLAMP = false;

float3 ambientLightColor = {0.15, 0.15, 0.15};
float3 lightPos;
float3 lightColor = {1.0f, 1.0f, 1.0f};

float3 Kd = {0.8, 0.8, 0.8};
float3 Ks = {0.3, 0.3, 0.3};
float specularPower = 64.0;
float tile = 1.0;

float depth = 0.1;

// Parallax Offset Mapping
float parallaxScale = 0.06;
float parallaxBias = 0.015;

// POM Configurations
int LODThreshold = 3;
int minSamples = (int)(NUM_ITERATIONS * 0.5);
int maxSamples = (int)(NUM_ITERATIONS * 2);

// Texture dimensions
int TEXTURE_WIDTH;
int TEXTURE_HEIGHT;
int TEXTURE_DEPTH;

texture color_map_tex;
texture displacement_map_tex;
texture cone_map_tex;
texture cone_map3D_tex;
texture cone_map3Dv_tex;
texture sphere_map_tex;

sampler2D color_map = sampler_state
{
	Texture = <color_map_tex>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

sampler2D displacement_map = sampler_state
{
	Texture = <displacement_map_tex>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = None;
};

sampler2D cone_map = sampler_state
{
	Texture = <cone_map_tex>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

sampler3D cone_map3D = sampler_state
{
	Texture = <cone_map3D_tex>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

sampler3D cone_map3Dv = sampler_state
{
	Texture = <cone_map3Dv_tex>;
	MinFilter = None;
	MagFilter = None;
	MipFilter = None;
};

sampler3D sphere_map = sampler_state
{
	Texture = <sphere_map_tex>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

// Matrix
// ---------------------------------------------
float4x4 matWVP : WorldViewProjection;
float4x4 matW 	: World;
float4x4 matVI 	: ViewInverse;
//float4x4 matWIT : WorldInverseTranspose;

// Structs
// ---------------------------------------------
struct vertexInput
{
    float4 pos       : POSITION;
    float3 normal    : NORMAL;
    float2 uv0		 : TEXCOORD0;
    float3 tangent   : TANGENT0;
    float3 binormal  : BINORMAL0;
};

struct pixelInput
{
	float4 hpos		: POSITION;
	float2 uv0 		: TEXCOORD0;
	float3 eyeVec	: TEXCOORD1;
	float3 lightVec	: TEXCOORD2;
	float3 normalVec : TEXCOORD3;
};

pixelInput simple_vs(vertexInput IN) 
{
    pixelInput OUT;

    // Processing
    OUT.hpos = mul(IN.pos, matWVP);
    OUT.uv0 = IN.uv0 * tile;
    
   	// World vertex
	float3 vertexPos = mul(IN.pos, matW);
	
	// Normal vector (Only used by POM)
	OUT.normalVec = mul(IN.normal, matW);
	
	// View vector
	OUT.eyeVec = matVI[3].xyz - vertexPos;
	
	// Light vector
	OUT.lightVec = lightPos - vertexPos;
	
    return OUT;
}

pixelInput bump_vs(vertexInput IN) 
{
    pixelInput OUT;

    // Vertex in clip space    
    OUT.hpos = mul(IN.pos, matWVP);
    
    // Copy texcoord
    OUT.uv0 = IN.uv0 * tile;
    
	// World vertex
	float3 vertexPos = mul(IN.pos, matW);
	
	// Normal vector (Used by Texture Mapping and POM)
	OUT.normalVec = mul(IN.normal, matW);

    // Tangent space (Tangent, binormal, normal)
    float3x3 tangentMap = float3x3(IN.tangent, IN.binormal, IN.normal);
    tangentMap = mul(tangentMap, matW);
	
	// View vector
	float3 eyeVec = vertexPos - matVI[3].xyz;	
	OUT.eyeVec = mul(tangentMap, eyeVec);
	
	// Light vector
	float3 lightVec = lightPos - vertexPos;
	OUT.lightVec = mul(tangentMap, lightVec);
	
    return OUT;
}

pixelInput displacement_vs(vertexInput IN) 
{
    pixelInput OUT;

    // Copy texcoord
    OUT.uv0 = IN.uv0 * tile;

    // Read height
    float4 tex = tex2Dlod(displacement_map, float4(OUT.uv0, 0.0f, 0.0f));
	float height = tex.w;

	// Vertex in clip space
	float scaleDepth = 	100.0f;
    IN.pos = float4(IN.pos.xyz - IN.normal * height * depth * scaleDepth, 1.0f);
    OUT.hpos = mul(IN.pos, matWVP);
    
	// World vertex
	float3 vertexPos = mul(IN.pos, matW);
	
	// Normal
	float3 normal;
	normal.xy = tex.xy * 2.0f - 1.0f;
	normal.y = -normal.y;
	normal.z = sqrt(1.0 - dot(tex.xy, tex.xy));
    
    float3x3 tangentMap = float3x3(IN.tangent, IN.binormal, IN.normal);	
	
	// Transform the normal by the tangent space instead of putting
	// the eye vector and light vector in the tangent space
	normal = mul(normal, tangentMap);
	OUT.normalVec = mul(normal, matW);
	
	// View vector
	OUT.eyeVec = vertexPos - matVI[3].xyz;	
	
	// Light vector
	OUT.lightVec = lightPos - vertexPos;

	return OUT;
}

float4 phongShading(float3 normal, float3 lightVec, float3 eyeVec, float3 materialColor)
{
	float3 halfVec = normalize(lightVec + eyeVec);
	
	float diffuseInt = saturate(dot(lightVec, normal));
	float specularInt = saturate(dot(halfVec, normal));
	specularInt = pow(specularInt, specularPower);
	
	float4 finalColor;
	finalColor.xyz = materialColor * (ambientLightColor + lightColor*diffuseInt*Kd) +
		specularInt*Ks;
	finalColor.w = 1.0f;
	
	return finalColor;
}

float4 displacement_ps(pixelInput IN) : COLOR
{
	// View and light vectors
	float3 v = normalize(IN.eyeVec);
	float3 l1 = normalize(IN.lightVec);
	float3 n = normalize(IN.normalVec);
	
	// Diffuse texture color
	float3 color = tex2D(color_map, IN.uv0).xyz;
	
	return phongShading(n, l1, -v, color);
}

float4 bump_ps(float2 uv0, float3 eyeVec, float3 lightVec) : COLOR
{
	if (BORDER_CLAMP)
	{
		if (uv0.x < 0.0) discard;
		if (uv0.x > tile) discard;
		if (uv0.y < 0.0) discard;
		if (uv0.y > tile) discard;
	}

	// View and light vectors
	float3 v = normalize(eyeVec);
	float3 l1 = normalize(lightVec);
	
	// Diffuse texture color
	float3 color = tex2D(color_map, uv0).xyz;

	// Bump map
	float3 n = tex2D(cone_map, uv0);
	n.xy = n.xy * 2.0 - 1.0;
	n.y = -n.y;
	n.z = sqrt(1.0 - dot(n.xy, n.xy));
	
	return phongShading(n, l1, -v, color);
}

float4 texture_ps(pixelInput IN) : COLOR
{
	// View and light vectors
	float3 n = normalize(IN.normalVec);
	float3 v = normalize(IN.eyeVec);
	float3 l1 = normalize(IN.lightVec);
	
	// Diffuse texture color
	float3 color = tex2D(color_map, IN.uv0).xyz;

	return phongShading(n, l1, v, color);
}

void setup_ray_ps(pixelInput IN, out float3 rayPos, out float3 rayVec)
{
	rayPos = float3(IN.uv0, 0.0);
	
	rayVec = normalize(IN.eyeVec);
	rayVec.z = abs(rayVec.z);

	// Depth bias used by Fabio Policarpo
	if (DEPTH_BIAS)
	{
		float db = 1.0 - rayVec.z;
		db *= db;
		db = 1.0 - db*db;
		rayVec.xy *= db;
	}
	
	rayVec.xy *= depth;
}

void ray_intersect_sphere(inout float3 p,inout float3 v)
{
	for(int i = 0; i < NUM_ITERATIONS; i++)
	{
		float stepDist = tex3D(sphere_map, p).x;
		p += v * stepDist;
	}
}

void ray_intersect_cone3D_fixed(inout float3 p,inout float3 v)
{
	for( int i=0;i<NUM_ITERATIONS;i++ )
	{
		float stepDist = tex3D(cone_map3D, p);
		p += v * stepDist;
	}

	/*
	// Lonesock
	float iz = max(abs(v.x),abs(v.y));
  
	for( int i=0;i<NUM_ITERATIONS; i++ )
	{
		float4 tex = tex2D(cone_map, p.xy);
		
		float radio = tex.z*tex.z;
		float height = tex.w - p.z;
		float dist = 1.2f * height/(iz/(radio) + 1.0);
		p += v*dist;
	}
	*/
}

void ray_intersect_cone(inout float3 p,inout float3 v)
{
	float dist = length(v.xy);
	
	for( int i=0;i<NUM_ITERATIONS; i++ )
	{
		float4 tex = tex2D(cone_map, p.xy);
		float height = saturate(tex.w - p.z);

		float cone_ratio = tex.z*tex.z;
		
		//float stepDist = height * cone_ratio / (v.z*cone_ratio + dist);
		float stepDist = height * cone_ratio / (cone_ratio + dist);
		p += v * stepDist;
		
		/*
		float4 tex = tex2D(cone_map, p.xy);
		float height = max(0, tex.w - p.z);
		float stepDist = height * 0.2;
		p += v * stepDist;
		*/
		//float stepDist = height * cone_ratio / (cone_ratio + 0.1);
		/*
		p += v * height * cone_ratio / 
			(v.z*cone_ratio + dist);
		*/	
	}
}

void ray_intersect_pom(inout float3 p, inout float3 v, float dx, float dy, int numSteps)
{
	v /= v.z*numSteps;
	
	int i;
	for(i=0; i < numSteps; i++)
	{
		float4 tex = tex2Dgrad(cone_map, p.xy, dx, dy);
		if (p.z < tex.w)
			p+=v;
	}
}

void ray_intersect_relief(inout float3 p,inout float3 v)
{
	// Discretize the space
	v /= v.z * NUM_ITERATIONS_RELIEF1;
	
	// Linear serach
	for(int i = 0; i < NUM_ITERATIONS_RELIEF1; i++ )
	{
		float4 tex = tex2D(cone_map, p.xy);
		if (p.z < tex.w)
			p+=v;
	}
	
	// Binary seracg
	for(int i = 0; i < NUM_ITERATIONS_RELIEF2; i++)
	{
		v *= 0.5;
		float4 tex = tex2D(cone_map, p.xy);
		if (p.z < tex.w)
			p += v;
		else
			p -= v;
	}
}

float4 sphere_ps(pixelInput IN) : COLOR
{
	float3 p,v;
	
	p = float3(IN.uv0, 0);

	// Texture width MUST be equal to texture height
	IN.eyeVec.xy *= depth * 15.0f/256.0f * 
		TEXTURE_WIDTH;

	v = normalize(IN.eyeVec);
	v.z = abs(v.z);

	if (DEPTH_BIAS)
	{
		float db = 1.0 - v.z;
		db *= db;
		db = 1.0 - db*db;
		v.xy *= db;
	}
	
	v.xy *= TEXTURE_DEPTH/(float)TEXTURE_WIDTH;

	ray_intersect_sphere(p,v);

	return bump_ps(p.xy, IN.eyeVec, IN.lightVec);
}


float4 cone3D_fixed_ps(pixelInput IN) : COLOR
{
	float3 p,v;
	setup_ray_ps(IN, p, v);
	
	ray_intersect_cone3D_fixed(p,v);

	return bump_ps(p.xy, IN.eyeVec, IN.lightVec);
}

float4 cone_ps(pixelInput IN) : COLOR
{
	float3 p,v;
	setup_ray_ps(IN,p,v);

	ray_intersect_cone(p,v);

	return bump_ps(p.xy, IN.eyeVec, IN.lightVec);
}

float4 pom_ps(pixelInput IN) : COLOR
{
	float2 absoluteTexcoord = IN.uv0 * float2(TEXTURE_WIDTH, TEXTURE_HEIGHT);
	float2 dxAbsolute = ddx(absoluteTexcoord);
	float2 dyAbsolute = ddy(absoluteTexcoord);
	
	// Calculate mipmap
	// mipmap = max( sqrt((Du/Dx)^2 + (Dv/Dx)^2), sqrt((Du/Dy)^2 + (Dv/Dy)^2) )
	float2 dxdySquared = dxAbsolute * dxAbsolute + dyAbsolute * dyAbsolute;
	float texcoordDelta = max(dxdySquared.x, dxdySquared.y);
	
	//float mipmap = max(0.0, 0.5 * log2(texcoordDelta));
	float mipmapLevel = max(0.0f, sqrt(texcoordDelta));
	
	float2 dx = ddx(IN.uv0);
	float2 dy = ddy(IN.uv0);
	
	float3 rayPos = float3(IN.uv0, 0.0);
	if (mipmapLevel <= (float)LODThreshold)
	{
		float3 rayVec = normalize(IN.eyeVec);
		int numSteps = (int)lerp(maxSamples, minSamples, dot(rayVec, normalize(IN.normalVec)));
		
		rayVec.z = abs(rayVec.z);
		if (DEPTH_BIAS)
		{
			float db = 1.0 - rayVec.z;
			db *= db;
			db = 1.0 - db*db;
			rayVec.xy *= db;
		}
		rayVec.xy *= depth;
		
		ray_intersect_pom(rayPos.xyz, rayVec, dx, dy, numSteps);
	}
	
	return bump_ps(rayPos.xy, IN.eyeVec, IN.lightVec);
}

float4 relief_ps(pixelInput IN) : COLOR
{
	float3 p,v;
	setup_ray_ps(IN,p,v);

	ray_intersect_relief(p.xyz, v);
	
	return bump_ps(p.xy, IN.eyeVec, IN.lightVec);
}

float4 parallax_ps(pixelInput IN) : COLOR
{
	float offset = tex2D(cone_map, IN.uv0).w
		* parallaxScale - parallaxBias;
	IN.uv0 += normalize(IN.eyeVec) * offset;
	
	return bump_ps(IN.uv0, IN.eyeVec, IN.lightVec);
}

float4 normal_ps(pixelInput IN) : COLOR
{
	return bump_ps(IN.uv0, IN.eyeVec, IN.lightVec);
}

float4 mipmap_ps(pixelInput IN) : COLOR
{
	float2 absoluteTexcoord = IN.uv0 * float2(TEXTURE_WIDTH, TEXTURE_HEIGHT);
	float2 dxAbsolute = ddx(absoluteTexcoord);
	float2 dyAbsolute = ddy(absoluteTexcoord);
	
	// Calculate mipmap
	// mipmap = max( sqrt((Du/Dx)^2 + (Dv/Dx)^2), sqrt((Du/Dy)^2 + (Dv/Dy)^2) )
	float2 dxdySquared = dxAbsolute * dxAbsolute + dyAbsolute * dyAbsolute;
	float texcoordDelta = max(dxdySquared.x, dxdySquared.y);
	float mipmapLevel = max(0.0, sqrt(texcoordDelta));
	
	// Change colors according with the mipmap
	float4 resultColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	if (mipmapLevel >= 0.0 && mipmapLevel < 1.0)
		resultColor = float4(1.0f, 0.0f, 0.0f, 1.0f);
	if (mipmapLevel >= 1.0 && mipmapLevel < 2.0)
		resultColor = float4(0.0f, 1.0f, 0.0f, 1.0f);
	if (mipmapLevel >= 2.0 && mipmapLevel < 3.0)
		resultColor = float4(0.0f, 0.0f, 1.0f, 1.0f);
	if (mipmapLevel >= 3.0 && mipmapLevel < 4.0)
		resultColor = float4(1.0f, 1.0f, 0.0f, 1.0f);
	if (mipmapLevel >= 4.0 && mipmapLevel < 5.0)
		resultColor = float4(0.0f, 1.0f, 1.0f, 1.0f);
	if (mipmapLevel >= 5.0 && mipmapLevel < 6.0)
		resultColor = float4(1.0f, 0.0f, 1.0f, 1.0f);
	if (mipmapLevel >= 6.0 && mipmapLevel < 7.0)
		resultColor = float4(0.5f, 0.5f, 0.5f, 1.0f);
	if (mipmapLevel > 7.0 && mipmapLevel <= 8.0)
		resultColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
		
	return resultColor;
}

technique sphere_mapping
{
    pass p0 
    {
		VertexShader = compile vs_1_1 bump_vs();
		PixelShader  = compile ps_2_a sphere_ps();
    }
}

technique cone3D_fixed
{
    pass p0 
    {
		VertexShader = compile vs_1_1 bump_vs();
		PixelShader  = compile ps_2_a cone3D_fixed_ps();
    }
}

technique cone_mapping
{
    pass p0 
    {
		VertexShader = compile vs_1_1 bump_vs();
		PixelShader  = compile ps_2_a cone_ps();
    }
}

technique parallax_occlusion_mapping
{
    pass P0
    {          
        VertexShader = compile vs_1_1 bump_vs();
        PixelShader  = compile ps_3_0 pom_ps(); 
    }
}

technique relief_mapping
{
    pass p0 
    {
		VertexShader = compile vs_1_1 bump_vs();
		PixelShader  = compile ps_2_a relief_ps();
    }
}

technique parallax_mapping
{
    pass p0 
    {
		VertexShader = compile vs_1_1 bump_vs();
		PixelShader  = compile ps_2_a parallax_ps();
    }
}

technique normal_mapping
{
    pass p0 
    {
		VertexShader = compile vs_1_1 bump_vs();
		PixelShader  = compile ps_2_a normal_ps();
    }
}

technique displacement_mapping
{
    pass P0
    {          
        CullMode =ccw;
        
        VertexShader = compile vs_3_0 displacement_vs();
        PixelShader  = compile ps_2_a displacement_ps(); 
    }
}

technique mip_mapping
{
    pass P0
    {          
        VertexShader = compile vs_1_1 bump_vs();
        PixelShader  = compile ps_2_a mipmap_ps(); 
    }
}

technique texture_mapping
{
    pass P0
    {          
        VertexShader = compile vs_1_1 simple_vs();
        PixelShader  = compile ps_3_0 texture_ps(); 
    }
}