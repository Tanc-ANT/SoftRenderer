#include "Rasterizer.h"

// default axis viewport transform value
const int axis_ox = 0;
const int axis_oy = 500;
const int axis_w = 100;
const int axis_h = 100;

Vector4 axis_array[4] = {
	{0.0f,0.0f,0.0f,1.0f},		//origin
	{5.0f,0.0f,0.0f,1.0f},		//x-axis	red line
	{0.0f,5.0f,0.0f,1.0f},		//y-axis	green line
	{0.0f,0.0f,5.0f,1.0f}		//z-axis	blue line
};	

Rasterizer::Rasterizer()
{
	for (int i = 0; i < 3; ++i)
	{
		unifrom[i] = std::make_shared<Uniform>();
	}
};

Rasterizer::~Rasterizer()
{

};

Vector4 Rasterizer::TransformHomogenize(const Vector4& v)
{
	float rhw = 1.0f / v.w;
	float x = v.x * rhw;
	float y = v.y * rhw;
	float z = v.z * rhw;
	float w = rhw;

	x = (x + 1.0f)*canvas->GetWidth() * 0.5f;
	y = (1.0f - y)*canvas->GetHeight() * 0.5f;
	z = (z + 1.0f) * 0.5f;

	return Vector4(x, y, z, w);
}

Vector4 Rasterizer::InvTransformHomogenize(const Vector4& v)
{
	float x = v.x;
	float y = v.y;

	x = (x * 2 / canvas->GetWidth() - 1.0f) / v.w;
	y = (1.0f - y * 2 / canvas->GetHeight()) / v.w;

	float z = (v.z * 2 - 1.0f) / v.w;
	float w = 1 / v.w;
	return Vector4(x, y, z, w);
}

void Rasterizer::TransformViewPort(int& x, int& y, int oX, int oY, int w, int h)
{
	float w_ratio = (float)w / (float)canvas->GetWidth();
	float h_ratio = (float)h / (float)canvas->GetHeight();
	
	x = (int)(x * w_ratio + oX);
	y = (int)(y * h_ratio + oY);
}

void Rasterizer::TransformViewPort(float& x, float& y, int oX, int oY, int w, int h)
{
	float w_ratio = (float)w / (float)canvas->GetWidth();
	float h_ratio = (float)h / (float)canvas->GetHeight();

	x = x * w_ratio + oX;
	y = y * h_ratio + oY;
}

void Rasterizer::ClipWithPlane(const Vector4& ponint, const Vector4& normal,
	std::vector<std::shared_ptr<Uniform>>& vert_list, 
	std::vector<std::shared_ptr<Uniform>>& in_list)
{
	int num_vert = (int)vert_list.size();
	int previous_index, current_index;
	for (int i = 0; i < num_vert; ++i)
	{
		current_index = i;
		previous_index = (i - 1 + num_vert) % num_vert;
		auto pre_vertex = vert_list[previous_index];
		auto current_vertex = vert_list[current_index];

		Vector4 Q1P = pre_vertex->cameraScreenPos - ponint;
		Vector4 Q2P = current_vertex->cameraScreenPos - ponint;
		
		float d1 = Q1P.Dot(normal);
		float d2 = Q2P.Dot(normal);

		if (d1*d2 < 0)
		{
			float t = d1 / (d1 - d2);
			Uniform u = Uniform::Lerp(pre_vertex, current_vertex, t);
			auto u_ptr = std::make_shared<Uniform>(u);
			in_list.push_back(u_ptr);
		}
		if (d2 > 0)
		{
			in_list.push_back(current_vertex);
		}
	}
}

void Rasterizer::ClipSpace()
{
	if (renderPass == 1)
	{
		if(scnManager->GetCurrentModels()->GetModel(currModelIndex)->GetCastShadow())
			DrawTriangleDepth();
	}
	else if (renderPass == 0)
	{
		std::vector<std::shared_ptr<Uniform>> vert_list{ unifrom[0], unifrom[1], unifrom[2] };
		std::vector<std::shared_ptr<Uniform>> in_list1;

		ClipWithPlane(Vector4(0, 0, camera->GetNear(), 1), Vector4(0, 0, 1, 0),
			vert_list, in_list1);	//near
		int num_vertex = (int)in_list1.size() - 2;

		for (int i = 0; i < num_vertex; ++i)
		{
			int index0 = 0;
			int index1 = i + 1;
			int index2 = i + 2;

			unifrom[0] = in_list1[index0]->Clone();
			unifrom[1] = in_list1[index1]->Clone();
			unifrom[2] = in_list1[index2]->Clone();

			DrawTriangleColor();
		}
	}
}

void Rasterizer::CameraTriangleTransfrom(const Triangle& triangle)
{
	Vertex vertex_points[3] = { triangle.GetV0(),triangle.GetV1(),triangle.GetV2() };
	Vector4 world_points[3];
	Vector4 screen_points[3];

	Matrix4 M; Matrix4 V; Matrix4 P;

	M = camera->GetModelMatrix();
	V = camera->GetViewMatrix();
	P = camera->GetProjectionMatrix();

	for (int j = 0; j < 3; j++) {
		Vector4 v = vertex_points[j].GetVertexPosition();
		Vector4 n = vertex_points[j].GetVertexNormal();
		// M transform
		world_points[j] = v * M;
		vertex_points[j].SetVertexPosition(world_points[j]);

		// normal set
		n = n * M;
		vertex_points[j].SetVertexNormal(n);

		// VP transform
		screen_points[j] = world_points[j] * V;
		screen_points[j] = screen_points[j] * P;

		unifrom[j]->worldPos = world_points[j];
		unifrom[j]->cameraScreenPos = screen_points[j];
		unifrom[j]->normal = n;
		unifrom[j]->texcoord = vertex_points[j].GetVertexTexcoord();
		unifrom[j]->color = vertex_points[j].GetVertexColor();
	}
}

void Rasterizer::LightTriangleTransfrom(const Triangle& triangle)
{
	Vertex vertex_points[3] = { triangle.GetV0(),triangle.GetV1(),triangle.GetV2() };
	Vector4 world_points[3];
	Vector4 screen_points[3];

	for (int j = 0; j < 3; j++) {
		Vector4 v = vertex_points[j].GetVertexPosition();
		// M transform
		world_points[j] = v * camera->GetModelMatrix();

		// VP transform
		screen_points[j] = world_points[j] * lightView;
		screen_points[j] = screen_points[j] * lightOrth;

		unifrom[j]->lightScreenPos = screen_points[j];
	}
}

Vector4 Rasterizer::LightVertexTransfrom(const Vector4& vert)
{
	Vector4 world_points;
	Vector4 screen_points;

	Vector4 v = vert;
	// M transform
	world_points = v * camera->GetModelMatrix();

	// VP transform
	auto light = scnManager->GetCurrentLight();
	screen_points = world_points * lightView;
	screen_points = screen_points * lightOrth;

	return screen_points;
}

void Rasterizer::DrawPixel(int x, int y, UINT32 color)
{
	if (((unsigned int)x < (UINT32)canvas->GetWidth()) && (((unsigned int)y < (UINT32)canvas->GetHeight()))
		&& (((unsigned int)x >= (UINT32)0)) && (((unsigned int)y >= (UINT32)0)))
	{
		canvas->GetFrameBuffer()[y][x] = color;
	}
}

void Rasterizer::DrawLine(int x0, int y0, int x1, int y1, Color color)
{
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = y0;
	for (int x = x0; x <= x1; ++x) {
		if (steep) {
			DrawPixel(y, x, color.GetIntensity());
		}
		else {
			DrawPixel(x, y, color.GetIntensity());
		}
		error2 += derror2;
		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

void Rasterizer::DrawScanline(const Uniform& A, const Uniform& B, int y)
{
	Vector4 worldpos_A = A.worldPos;
	Vector4 worldpos_B = B.worldPos;

	Vector4 campos_A = A.cameraScreenPos;
	Vector4 campos_B = B.cameraScreenPos;

	Color color_A = A.color;
	Color color_B = B.color;

	Vector3 texture_A = A.texcoord;
	Vector3 texture_B = B.texcoord;

	Vector4 normal_A = A.normal;
	Vector4 normal_B = B.normal;

	//zbuffer caculation
	float scanline_depth = campos_B.z - campos_A.z;
	float scanline_width = campos_B.x - campos_A.x;
	float depth_ratio = scanline_depth / scanline_width;
	float *z_line_buffer = canvas->GetZBuffer()[y];

	//w caculation
	float w_diff = campos_B.w - campos_A.w;
	float w_ratio = w_diff / scanline_width;

	//world pos caculation
	Vector4 world_diff = worldpos_B - worldpos_A;
	Vector4 worldpos_ratio = world_diff / scanline_width;

	//color caculation
	Color color_diff = color_B - color_A;
	Color color_ratio = color_diff / scanline_width;

	//UV caculation
	Vector3 uv_diff = texture_B - texture_A;
	Vector3 uv_ratio = uv_diff / scanline_width;

	//normal caculation
	Vector4 norm_diff = normal_B - normal_A;
	Vector4 norm_ratio = norm_diff / scanline_width;

	for (int j = (int)(campos_A.x + 0.5f); j < (int)(campos_B.x + 0.5f); ++j)
	{
		float step = (float)j - campos_A.x + 0.5f;
		if (j >= canvas->GetWidth() || j < 0) continue;
		float z = depth_ratio * step + campos_A.z;
		float w = w_ratio * step + campos_A.w;

		if (z_line_buffer[j] > z)
		{
			Color color;
			color = (color_ratio * step + color_A);
			color = color / w;

			Vector4 normal;
			normal = (norm_ratio * step + normal_A);
			normal = normal / w;

			Vector4 world = (worldpos_ratio * step + worldpos_A);
			world = world / w;

			Vector3 uv;
			uv = (uv_ratio * step + texture_A);
			uv = uv / w;

			// light caculation
			CaculateLightColor(world, normal,uv, color);

			z_line_buffer[j] = z;
			if (scnManager->GetRenderState() & RENDER_STATE_SHADOW)
			{
				if (TestVertexInShadow(world, normal))
				{
					float shadow = scnManager->GetCurrentLight()->GetAmbient();
					color = color * Color(shadow, shadow, shadow);
				}
			}
			DrawPixel(j, y, color.GetIntensity());
		}
	}
}

void Rasterizer::DrawTriangleDepth()
{
	Vector4 t0 = TransformHomogenize(unifrom[0]->lightScreenPos);
	Vector4 t1 = TransformHomogenize(unifrom[1]->lightScreenPos);
	Vector4 t2 = TransformHomogenize(unifrom[2]->lightScreenPos);

	if (t0.y > t1.y) { std::swap(t0, t1); }
	if (t0.y > t2.y) { std::swap(t0, t2); }
	if (t1.y > t2.y) { std::swap(t1, t2); }

	float total_height = t2.y - t0.y;
	// plus 0.5 for rounding
	for (int y = (int)(t0.y + 0.5f); y <= (int)(t1.y + 0.5f); y++)
	{
		// Because of up set down Y. Add a checkpoint here.
		if (y >= canvas->GetHeight() || y < 0) continue;
		// This check  additional pixel 
		//if(std::abs((float)y - t0.y)<0.5f) continue;
		float segement_height = t1.y - t0.y + EPSILON;
		//if(segement_height < 1.0f) continue;
		float alpha = (float)(y - t0.y) / total_height;
		float beta = (float)(y - t0.y) / segement_height;
		//position lerp
		Vector4 A = Vector4::ClampLerp(t0, t2, alpha);
		Vector4 B = Vector4::ClampLerp(t0, t1, beta);

		//draw line from left to right
		if (A.x > B.x)
		{
			std::swap(A, B);
		}

		//zbuffer caculation
		float scanline_depth = B.z - A.z;
		float scanline_width = B.x - A.x;
		float depth_ratio = scanline_depth / scanline_width;

		//w caculation
		float w_ratio = 1.0f;
		float w_diff = B.w - A.w;
		w_ratio = w_diff / scanline_width;

		for (int j = (int)(A.x + 0.5f); j < (int)(B.x + 0.5f); ++j)
		{
			float step = (float)j - A.x + 0.5f;
			if (j >= canvas->GetWidth() || j < 0) continue;
			float z = depth_ratio * step + A.z;

			if (GetShadowDepth(j,y) > z)
			{
				DrawShadowDepth(j, y, z);
				Color color(z, z, z);
				DrawPixel(j, y, color.GetIntensity());
			}
		}
	}
	// plus 0.5 for rounding
	for (int y = (int)(t1.y + 0.5f); y <= (int)(t2.y + 0.5f); y++)
	{
		// Because of up set down Y. Add a checkpoint here.
		if (y >= canvas->GetHeight() || y < 0) break;
		// This check  additional pixel 
		//if (std::abs((float)y - t1.y) < 0.5f) continue;
		float segement_height = t2.y - t1.y + EPSILON;
		//if (segement_height < 1.0f) continue;
		float alpha = (float)(y - t0.y) / total_height;
		float beta = (float)(y - t1.y) / segement_height;
		//position lerp
		Vector4 A = Vector4::ClampLerp(t0, t2, alpha);
		Vector4 B = Vector4::ClampLerp(t1, t2, beta);

		//draw line from left to right
		if (A.x > B.x)
		{
			std::swap(A, B);
		}

		//for zbuffer caculation
		float scanline_depth = B.z - A.z;
		float scanline_width = B.x - A.x;
		float depth_ratio = scanline_depth / scanline_width;

		//w caculation
		float w_ratio = 1.0f;
		float w_diff = B.w - A.w;
		w_ratio = w_diff / scanline_width;

		for (int j = (int)(A.x + 0.5f); j < (int)(B.x + 0.5f); ++j)
		{
			float step = (float)j - A.x + 0.5f;
			if (j >= canvas->GetWidth() || j < 0) break;
			float z = depth_ratio * step + A.z;
			float w = w_ratio * step + A.w;
			if (GetShadowDepth(j, y) > z)
			{
				DrawShadowDepth(j, y, z);
				Color color(z, z, z);
				DrawPixel(j, y, color.GetIntensity());
			}
		}
	}
}

void Rasterizer::DrawTriangleColor()
{

	Vector4 t0 = TransformHomogenize(unifrom[0]->cameraScreenPos);
	Vector4 t1 = TransformHomogenize(unifrom[1]->cameraScreenPos);
	Vector4 t2 = TransformHomogenize(unifrom[2]->cameraScreenPos);

	unifrom[0]->cameraScreenPos = t0;
	unifrom[1]->cameraScreenPos = t1;
	unifrom[2]->cameraScreenPos = t2;

	if (BackFaceCulling(t0, t1, t2))
		return;
	else
		nTriangle++;

	if (renderPass == 0 && (scnManager->GetRenderState() & RENDER_STATE_WIREFRAME))
	{
		DrawLine((int)t0.x, (int)t0.y, (int)t1.x, (int)t1.y, Color::WHITH_COLOR);
		DrawLine((int)t1.x, (int)t1.y, (int)t2.x, (int)t2.y, Color::WHITH_COLOR);
		DrawLine((int)t2.x, (int)t2.y, (int)t0.x, (int)t0.y, Color::WHITH_COLOR);
	}
	
	else if(scnManager->GetRenderState() & (RENDER_STATE_COLOR | RENDER_STATE_TEXTURE))
	{
		// Perspective correction
		unifrom[0]->worldPos = unifrom[0]->worldPos * t0.w;
		unifrom[1]->worldPos = unifrom[1]->worldPos * t1.w;
		unifrom[2]->worldPos = unifrom[2]->worldPos * t2.w;

		unifrom[0]->color = unifrom[0]->color * t0.w;
		unifrom[1]->color = unifrom[1]->color * t1.w;
		unifrom[2]->color = unifrom[2]->color * t2.w;

		unifrom[0]->texcoord = unifrom[0]->texcoord * t0.w;
		unifrom[1]->texcoord = unifrom[1]->texcoord * t1.w;
		unifrom[2]->texcoord = unifrom[2]->texcoord * t2.w;

		unifrom[0]->normal = unifrom[0]->normal * t0.w;
		unifrom[1]->normal = unifrom[1]->normal * t1.w;
		unifrom[2]->normal = unifrom[2]->normal * t2.w;

		
		if (unifrom[0]->cameraScreenPos.y > unifrom[1]->cameraScreenPos.y) { std::swap(unifrom[0], unifrom[1]); }
		if (unifrom[0]->cameraScreenPos.y > unifrom[2]->cameraScreenPos.y) { std::swap(unifrom[0], unifrom[2]); }
		if (unifrom[1]->cameraScreenPos.y > unifrom[2]->cameraScreenPos.y) { std::swap(unifrom[1], unifrom[2]); }

		t0 = unifrom[0]->cameraScreenPos;
		t1 = unifrom[1]->cameraScreenPos;
		t2 = unifrom[2]->cameraScreenPos;

		float total_height = t2.y - t0.y;
		// plus 0.5 for rounding
		for (int y = (int)(t0.y + 0.5f); y <= (int)(t1.y + 0.5f); y++)
		{
			// Because of up set down Y. Add a checkpoint here.
			if (y >= canvas->GetHeight() || y < 0) continue;

			float segement_height = t1.y - t0.y + EPSILON;
			//if(segement_height < 1.0f) continue;
			float alpha = (float)(y - t0.y) / total_height;
			float beta = (float)(y - t0.y) / segement_height;
			//vertex lerp
			Uniform A = Uniform::ClampLerp(unifrom[0], unifrom[2], alpha);
			Uniform B = Uniform::ClampLerp(unifrom[0], unifrom[1], beta);
			
			//draw line from left to right
			if (A.cameraScreenPos.x > B.cameraScreenPos.x)
			{
				std::swap(A, B);
			}

			DrawScanline(A, B, y);
		}
		// plus 0.5 for rounding
		for (int y = (int)(t1.y + 0.5f); y <= (int)(t2.y + 0.5f); y++)
		{
			// Because of up set down Y. Add a checkpoint here.
			if (y >= canvas->GetHeight() || y < 0) continue;

			float segement_height = t2.y - t1.y + EPSILON;
			//if (segement_height < 1.0f) continue;
			float alpha = (float)(y - t0.y) / total_height;
			float beta = (float)(y - t1.y) / segement_height;
			//vertex lerp
			Uniform A = Uniform::ClampLerp(unifrom[0], unifrom[2], alpha);
			Uniform B = Uniform::ClampLerp(unifrom[1], unifrom[2], beta);
			//draw line from left to right
			if (A.cameraScreenPos.x > B.cameraScreenPos.x)
			{
				std::swap(A, B);
			}

			DrawScanline(A, B, y);
		}
	}
}

void Rasterizer::DrawShadowDepth(int x, int y, float z)
{
	float x_ratio = (float)shadowMap->GetWidth() / (float)canvas->GetWidth();
	float y_ratio = (float)shadowMap->GetHeight() / (float)canvas->GetHeight();

	int u = (int)(x * x_ratio + 0.5f);
	int v = (int)(y * y_ratio + 0.5f);
	x = CLAMP(u, 0, shadowMap->GetWidth() - 1);
	y = CLAMP(v, 0, shadowMap->GetHeight() - 1);
	shadowMap->SetDepth(x, y, z);
}

float Rasterizer::GetShadowDepth(int x, int y)
{
	float x_ratio = (float)shadowMap->GetWidth() / (float)canvas->GetWidth();
	float y_ratio = (float)shadowMap->GetHeight() / (float)canvas->GetHeight();

	int u = (int)(x * x_ratio + 0.5f);
	int v = (int)(y * y_ratio + 0.5f);
	x = CLAMP(u, 0, shadowMap->GetWidth() - 1);
	y = CLAMP(v, 0, shadowMap->GetHeight() - 1);
	return shadowMap->GetDepth(x, y);
}

bool Rasterizer::TestVertexInShadow(const Vector4& world_point, const Vector4& normal)
{
	if ((scnManager->GetRenderState() & RENDER_STATE_SHADOW) && 
		scnManager->GetCurrentModels()->GetModel(currModelIndex)->GetReceiveShadow())
	{
		Vector4 screen_point = world_point * lightView;
		screen_point = screen_point * lightOrth;

		screen_point = TransformHomogenize(screen_point);

		int x = (int)screen_point.x, y = (int)screen_point.y;
		float z = screen_point.z;

		if (y >= canvas->GetHeight() || x > canvas->GetWidth() || y <= 0 || x < 0)
			return false;

		//caculate current depth in light screen
		auto light = std::dynamic_pointer_cast<DirectLight>(scnManager->GetCurrentLight());
		Vector4 light_dir = -light->GetDirection();
		light_dir.Normalize();
		float dot = normal.Dot(light_dir);
		float bias = std::fmaxf(0.025f * (1.0f - dot), 0.010f);
		float currentDepth = screen_point.z - bias;

		float closestDepth = GetShadowDepth(x, y);
		return closestDepth < currentDepth ? true : false;
	}
	else
		return false;
}

void Rasterizer::DrawSomthing()
{
	nTriangle = 0;
	std::shared_ptr<Model> model;
	int max_models = (int)scnManager->GetCurrentModels()->GetSize();
	
	SortRenderArray();

	for (currModelIndex = 0; currModelIndex < max_models; ++currModelIndex)
	{
		model = scnManager->GetCurrentModels()->GetModel(currModelIndex);

		for (int i = 0; i < model->Nfaces(); i++) {
			if ((scnManager->GetRenderState() & RENDER_STATE_SHADOW) &&
				scnManager->GetCurrentModels()->GetModel(currModelIndex)->GetCastShadow())
			{
				LightTriangleTransfrom(model->GetFaceIndex(i));
			}
			CameraTriangleTransfrom(model->GetFaceIndex(i));
			ClipSpace();
		}
	}
	window->SetNtri(nTriangle);
}

void Rasterizer::DrawAxis()
{
	Vector4 screen_pos[4];
	Matrix4 M = camera->GetModelMatrix();
	Matrix4 V = camera->GetStaticViewMatrix();
	Matrix4 P = camera->GetProjectionMatrix();
	for (int i = 0; i < 4; ++i)
	{
		screen_pos[i] = axis_array[i];
		screen_pos[i] = screen_pos[i] * M * V * P;
		screen_pos[i] = TransformHomogenize(screen_pos[i]);
		TransformViewPort(screen_pos[i].x, screen_pos[i].y, axis_ox, axis_oy, axis_w, axis_h);
	}
	DrawLine((int)screen_pos[0].x, (int)screen_pos[0].y, 
		(int)screen_pos[1].x, (int)screen_pos[1].y,Color::RED_COLOR);
	DrawLine((int)screen_pos[0].x, (int)screen_pos[0].y,
		(int)screen_pos[2].x, (int)screen_pos[2].y, Color::GREEN_COLOR);
	DrawLine((int)screen_pos[0].x, (int)screen_pos[0].y,
		(int)screen_pos[3].x, (int)screen_pos[3].y, Color::BLUE_COLOR);
}

void Rasterizer::Update()
{
	renderPass = scnManager->GetRenderPass();
	//renderPass = 1;
	shadowMap->ClearTextureDepth();
	while (renderPass--)
	{
		canvas->ClearFrameBuffer(window->GetFrameBuffer());
		canvas->ClearZBuffer();
		camera->Update();
		UpdateLightMatirx();
		DrawSomthing();
		DrawAxis();
	}
	window->Update();
	ProcessWindowKeyInput();
	ProcessWindowMouseInput();
}

bool Rasterizer::BackFaceCulling(const Vector4& t0, const Vector4 t1, const Vector4 t2) const
{
	if (scnManager->GetRenderState() & RENDER_STATE_BACKCULL)
	{
		Vector3 v1 = Vector3(t1.x, t1.y, t1.z) - Vector3(t0.x, t0.y, t0.z);
		Vector3 v2 = Vector3(t2.x, t2.y, t2.z) - Vector3(t0.x, t0.y, t0.z);
		Vector3 n = v1.Cross(v2);
		return n.z <= 0;
	}
	else
	{
		return false;
	}
}

void Rasterizer::CaculateLightColor(const Vector4& world_pos, const Vector4& normal, const Vector3& tex, Color& pixel_color)
{
	if (scnManager->GetRenderState() & RENDER_STATE_LIGHT)
	{
		if (scnManager->GetRenderState() & RENDER_STATE_TEXTURE)
		{
			auto material = scnManager->GetCurrentModels()->
				GetModel(currModelIndex)->GetMaterial();

			scnManager->GetCurrentLight()->LightTextureCalculaiton(
				camera->GetPostion(), world_pos, normal,tex, material, pixel_color);
		}
		else if (scnManager->GetRenderState() & RENDER_STATE_COLOR)
		{
			scnManager->GetCurrentLight()->LightColorCalculaiton(
				camera->GetPostion(), world_pos, normal, pixel_color);
		}
	}
}

void Rasterizer::UpdateLightMatirx()
{
	if (scnManager->GetRenderState()&RENDER_STATE_SHADOW)
	{
		UpdateLightViewMatrix();
		UpdateLightOrthographicMatrix();
	}
}

void Rasterizer::UpdateLightViewMatrix()
{
	auto light = std::dynamic_pointer_cast<DirectLight>(scnManager->GetCurrentLight());

	Vector4 p = -light->GetDirection();
	Vector3 position = Vector3(p.x, p.y, p.z);
	Vector3 target = { 0, 0, 0 };
	Vector3 up = { 0,1,0 };

	Vector3 z_axis = target - position;
	z_axis.Normalize();
	Vector3 x_axis = up.Cross(z_axis);
	x_axis.Normalize();
	Vector3 y_axis = z_axis.Cross(x_axis);

	lightView.m[0][0] = x_axis.x;
	lightView.m[1][0] = x_axis.y;
	lightView.m[2][0] = x_axis.z;
	lightView.m[3][0] = -x_axis.Dot(position);

	lightView.m[0][1] = y_axis.x;
	lightView.m[1][1] = y_axis.y;
	lightView.m[2][1] = y_axis.z;
	lightView.m[3][1] = -y_axis.Dot(position);

	lightView.m[0][2] = z_axis.x;
	lightView.m[1][2] = z_axis.y;
	lightView.m[2][2] = z_axis.z;
	lightView.m[3][2] = -z_axis.Dot(position);

	lightView.m[0][3] = lightView.m[1][3] = lightView.m[2][3] = 0.0f;
	lightView.m[3][3] = 1.0f;
}

void Rasterizer::UpdateLightOrthographicMatrix()
{
	const float Near = 0.0f;
	const float Far = 20.0f;
	const float Right = 10.0f;
	const float Left = -10.0f;
	const float Top = 10.0f;
	const float Bottom = -10.0f;

	lightOrth.m[0][0] = 2 / (Right - Left);
	lightOrth.m[1][1] = 2 / (Top - Bottom);
	lightOrth.m[2][2] = 2 / (Far - Near);
	lightOrth.m[3][0] = (Left + Right) / (Left - Right);
	lightOrth.m[3][1] = (Bottom + Top) / (Bottom - Top);
	lightOrth.m[3][2] = (Near + Far) / (Near - Far);
	lightOrth.m[3][3] = 1.0f;
}

void Rasterizer::SortRenderArray()
{
	auto model_array = scnManager->GetCurrentModels()->GetArrayRef();

	Vector4 model_camera = camera->GetPostion() * camera->GetInvModelMatrix();
	
	auto cmp1 = [model_camera](std::shared_ptr<Model> a, std::shared_ptr<Model> b)
	{
		return (((int)a->GetTransparent() < (int)b->GetTransparent()));
	};
	
	sort(model_array.begin(), model_array.end(), cmp1);

	auto iter = model_array.begin();
	for (; iter < model_array.end(); ++iter)
	{
		if((*iter)->GetTransparent()) break;
	}

	auto cmp2 = [model_camera](std::shared_ptr<Model> a, std::shared_ptr<Model> b)
	{
		Vector4 dist1 = model_camera - a->GetCenter();
		Vector4 dist2 = model_camera - b->GetCenter();

		float lenght1 = dist1.Lenght();
		float lenght2 = dist2.Lenght();

		return (lenght1 > lenght2);
	};

	sort(iter, model_array.end(), cmp2);

}

void Rasterizer::ProcessWindowKeyInput()
{
	if (window->GetKey()['W'])
	{
		Vector3 trans = camera->GetTrans();
		trans.z -= 0.01f;
		camera->SetTrans(trans);
	}

	if (window->GetKey()['S'])
	{
		Vector3 trans = camera->GetTrans();
		trans.z += 0.01f;
		camera->SetTrans(trans);
	}
	if (window->GetKey()['A'])
	{
		Vector3 trans = camera->GetTrans();
		trans.x += 0.01f;
		camera->SetTrans(trans);
	}
	if (window->GetKey()['D'])
	{
		Vector3 trans = camera->GetTrans();
		trans.x -= 0.01f;
		camera->SetTrans(trans);
	}
	if (window->GetKey()['Q'])
	{
		Vector3 trans = camera->GetTrans();
		trans.y += 0.01f;
		camera->SetTrans(trans);
	}
	if (window->GetKey()['E'])
	{
		Vector3 trans = camera->GetTrans();
		trans.y -= 0.01f;
		camera->SetTrans(trans);
	}
	if (window->GetKey()[VK_ESCAPE])
	{
		window->SetCloseState(true);
	}
	if (window->GetKey()[VK_SPACE])
	{
		if (!changeState)
		{
			changeState = true;
			scnManager->SwitchNextScene();
		}
	}
	else
		changeState = false;
}

void Rasterizer::ProcessWindowMouseInput()
{
	float dx, dy;
	Vector3 pos = window->GetMousePos();
	dx = (float)(pos.x - originX);
	dy = (float)(pos.y - originY);

	if (window->GetRightButtonState())
	{
		viewRotLength.x -= dy / 5.0f;
		viewRotLength.y -= dx / 5.0f;
		Vector3 viewRot = camera->GetViewRot();
		viewRot.x += (viewRotLength.x - viewRot.x) * 0.1f;
		viewRot.y += (viewRotLength.y - viewRot.y) * 0.1f;
		camera->SetViewRot(viewRot);
	}

	if (window->GetLeftButtonState())
	{
		modelRotLength.x -= dy / 5.0f;
		modelRotLength.y -= dx / 5.0f;
		Vector3 modelRot = camera->GetModelRot();
		modelRot.x += (modelRotLength.x - modelRot.x) * 0.1f;
		modelRot.y += (modelRotLength.y - modelRot.y) * 0.1f;
		camera->SetModelRot(modelRot);
	}
	originX = pos.x; originY = pos.y;
}