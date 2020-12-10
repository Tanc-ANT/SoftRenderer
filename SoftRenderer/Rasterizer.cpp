#include "Rasterizer.h"

Rasterizer::Rasterizer()
{

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
	z = (z + 1.0f) * 0.5f;
	return Vector4((x + 1.0f)*canvas->GetWidth() * 0.5f,
		(1.0f - y)*canvas->GetHeight() * 0.5f,
		z, w);
}

Vector4 Rasterizer::InvTransformHomogenize(const Vector4& v)
{

	float x = (v.x * 2 / canvas->GetWidth() - 1.0f) / v.w;
	float y = (1.0f - v.y * 2 / canvas->GetHeight()) / v.w;
	float z = (v.z * 2 - 1.0f) / v.w;
	float w = 1 / v.w;
	return Vector4(x, y, z, w);
}

void Rasterizer::ClipWithPlane(const Vector4& ponint, const Vector4& normal,
	std::vector<Vertex>& vert_list, std::vector<Vertex>& in_list)
{
	int num_vert = (int)vert_list.size();
	int previous_index, current_index;
	for (int i = 0; i < num_vert; ++i)
	{
		current_index = i;
		previous_index = (i - 1 + num_vert) % num_vert;
		Vertex pre_vertex = vert_list[previous_index];
		Vertex current_vertex = vert_list[current_index];

		Vector4 Q1P = pre_vertex.GetVertexPosition() - ponint;
		Vector4 Q2P = current_vertex.GetVertexPosition() - ponint;
		
		float d1 = Q1P.Dot(normal);
		float d2 = Q2P.Dot(normal);

		if (d1*d2 < 0)
		{
			float t = d1 / (d1 - d2);
			Vertex I = Vertex::Lerp(pre_vertex, current_vertex, t);
			in_list.push_back(I);
		}
		if (d2 > 0)
		{
			in_list.push_back(current_vertex);
		}
	}
}

void Rasterizer::ClipCVV(const Triangle& cam_tri, const Triangle& lig_tri)
{
	if (renderPass == 1)
	{
		DrawTriangleDepth(lig_tri);
	}
	else if (renderPass == 0)
	{
		std::vector<Vertex> vert_list;
		std::vector<Vertex> in_list1;
		//std::vector<Vertex> in_list2;

		vert_list.push_back(cam_tri.GetV0());
		vert_list.push_back(cam_tri.GetV1());
		vert_list.push_back(cam_tri.GetV2());

		float cosAh = cos(camera->GetFovY() / 2);
		float sinAh = sin(camera->GetFovY() / 2);

		ClipWithPlane(Vector4(0, 0, camera->GetNear(), 1), Vector4(0, 0, 1, 0),
			vert_list, in_list1);	//near
		//ClipWithPlane(Vector4(0, 0, camera->GetFar(), 1), Vector4(0, 0, -1, 0),
		//	in_list1, in_list2);	//far

		int num_vertex = (int)in_list1.size() - 2;

		Vector4 world_pos[3];

		for (int i = 0; i < num_vertex; ++i)
		{
			int index0 = 0;
			int index1 = i + 1;
			int index2 = i + 2;
			Triangle cam_triangle(in_list1[index0],
				in_list1[index1], in_list1[index2]);

			Triangle light_triangle = CameraTriangleToLightTriangle(cam_triangle);

			DrawTriangleColor(cam_triangle);
		}
	}
}

Triangle Rasterizer::CameraTriangleTransfrom(const Triangle& triangle)
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
		Vector3 t = vertex_points[j].GetVertexTexcoord();
		Vector4 n = vertex_points[j].GetVertexNormal();
		// M transform
		world_points[j] = v * M;
		vertex_points[j].SetVertexPosition(world_points[j]);

		// normal set
		n = n * M;
		vertex_points[j].SetVertexNormal(n);
		
		// light caculation
		if (scnManager->GetRenderState() & RENDER_STATE_LIGHT)
		{
			scnManager->GetCurrentLight()->LightColorCalculaiton(
				Vector4(camera->GetPostion(), 1.0f),
				vertex_points[j]);
		}
		// VP transform
		screen_points[j] = world_points[j] * V;
		screen_points[j] = screen_points[j] * P;
		vertex_points[j].SetVertexPosition(screen_points[j]);
	}
	return Triangle(vertex_points[0], vertex_points[1], vertex_points[2]);
}

Triangle Rasterizer::LightTriangleTransfrom(const Triangle& triangle)
{
	Vertex vertex_points[3] = { triangle.GetV0(),triangle.GetV1(),triangle.GetV2() };
	Vector4 world_points[3];
	Vector4 screen_points[3];

	Matrix4 M; Matrix4 V; Matrix4 P;

	M = camera->GetModelMatrix();
	V = scnManager->GetCurrentLight()->GetViewMatrix();
	P = scnManager->GetCurrentLight()->GetProjectionMatrix();

	for (int j = 0; j < 3; j++) {
		Vector4 v = vertex_points[j].GetVertexPosition();
		// M transform
		world_points[j] = v * M;
		vertex_points[j].SetVertexPosition(world_points[j]);

		// VP transform
		screen_points[j] = world_points[j] * V;
		screen_points[j] = screen_points[j] * P;
		vertex_points[j].SetVertexPosition(screen_points[j]);
	}
	return Triangle(vertex_points[0], vertex_points[1], vertex_points[2]);
}

Vector4 Rasterizer::LightVertexTransfrom(const Vector4& vert)
{
	Vector4 world_points;
	Vector4 screen_points;

	Vector4 v = vert;
	// M transform
	world_points = v * camera->GetModelMatrix();

	// VP transform
	screen_points = world_points * scnManager->GetCurrentLight()->GetViewMatrix();
	screen_points = screen_points * scnManager->GetCurrentLight()->GetProjectionMatrix();

	return screen_points;
}

Triangle Rasterizer::CameraTriangleToLightTriangle(const Triangle& triangle)
{
	Triangle light_triangle = triangle;
	Vector4 world_pos[3];

	world_pos[0] = light_triangle.GetV0().GetVertexPosition() * camera->GetInvProjectionMatrix();
	world_pos[1] = light_triangle.GetV1().GetVertexPosition() * camera->GetInvProjectionMatrix();
	world_pos[2] = light_triangle.GetV2().GetVertexPosition() * camera->GetInvProjectionMatrix();

	world_pos[0] = world_pos[0] * camera->GetInvViewMatrix();
	world_pos[1] = world_pos[1] * camera->GetInvViewMatrix();
	world_pos[2] = world_pos[2] * camera->GetInvViewMatrix();

	light_triangle.GetV0().SetVertexPosition(world_pos[0]);
	light_triangle.GetV1().SetVertexPosition(world_pos[1]);
	light_triangle.GetV2().SetVertexPosition(world_pos[2]);
	
	light_triangle = LightTriangleTransfrom(light_triangle);
	return light_triangle;
}

void Rasterizer::DrawPixel(int x, int y, UINT32 color)
{
	if (((unsigned int)x < (UINT32)canvas->GetWidth()) && (((unsigned int)y < (UINT32)canvas->GetHeight()))
		&& (((unsigned int)x >= (UINT32)0)) && (((unsigned int)y >= (UINT32)0)))
	{
		canvas->GetFrameBuffer()[y][x] = color;
	}
}

void Rasterizer::DrawDepth(int x, int y, float z)
{
	canvas->GetShadowBuffer()[y][x] = z;
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

void Rasterizer::DrawTriangleDepth(const Triangle& lig_t)
{
	Triangle triangle = lig_t;

	Vector4 t0 = lig_t.GetV0().GetVertexPosition();
	Vector4 t1 = lig_t.GetV1().GetVertexPosition();
	Vector4 t2 = lig_t.GetV2().GetVertexPosition();

	t0 = TransformHomogenize(t0);
	t1 = TransformHomogenize(t1);
	t2 = TransformHomogenize(t2);

	if (t0.y > t1.y) { std::swap(t0, t1); }
	if (t0.y > t2.y) { std::swap(t0, t2); }
	if (t1.y > t2.y) { std::swap(t1, t2); }

	float total_height = t2.y - t0.y;
	// plus 0.5 for rounding
	for (int y = (int)(t0.y + 0.5f); y <= (int)(t1.y + 0.5f); y++)
	{
		// Because of up set down Y. Add a checkpoint here.
		if (y >= canvas->GetHeight() || y < 0) break;
		// This check  additional pixel 
		//if(std::abs((float)y - t0.y)<0.5f) continue;
		float segement_height = t1.y - t0.y;
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
		float *shadow_line_buffer = canvas->GetShadowBuffer()[y];

		//w caculation
		float w_ratio = 1.0f;
		float w_diff = B.w - A.w;
		w_ratio = w_diff / scanline_width;

		for (int j = (int)(A.x + 0.5f); j < (int)(B.x + 0.5f); ++j)
		{
			float step = (float)j - A.x + 0.5f;
			if (j >= canvas->GetWidth() || j < 0) break;
			float z = depth_ratio * step + A.z;

			if (shadow_line_buffer[j] > z)
			{
				shadow_line_buffer[j] = z;
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
		float segement_height = t2.y - t1.y;
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
		float *shadow_line_buffer = canvas->GetShadowBuffer()[y];

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
			if (shadow_line_buffer[j] > z)
			{
				shadow_line_buffer[j] = z;
			}
		}
	}
}

void Rasterizer::DrawTriangleColor(const Triangle& cam_t)
{
	Triangle triangle = cam_t;

	Vector4 t0 = cam_t.GetV0().GetVertexPosition();
	Vector4 t1 = cam_t.GetV1().GetVertexPosition();
	Vector4 t2 = cam_t.GetV2().GetVertexPosition();

	t0 = TransformHomogenize(t0);
	t1 = TransformHomogenize(t1);
	t2 = TransformHomogenize(t2);
	
	if (BackFaceCulling(t0, t1, t2))
		return;
	else
		nTriangle++;

	Color c0 = cam_t.GetV0().GetVertexColor();
	Color c1 = cam_t.GetV1().GetVertexColor();
	Color c2 = cam_t.GetV2().GetVertexColor();

	Vector3 uv0 = cam_t.GetV0().GetVertexTexcoord();
	Vector3 uv1 = cam_t.GetV1().GetVertexTexcoord();
	Vector3 uv2 = cam_t.GetV2().GetVertexTexcoord();

	Vector4 n0 = cam_t.GetV0().GetVertexNormal();
	Vector4 n1 = cam_t.GetV1().GetVertexNormal();
	Vector4 n2 = cam_t.GetV2().GetVertexNormal();

	if (renderPass == 0 && (scnManager->GetRenderState() & RENDER_STATE_WIREFRAME))
	{
		DrawLine((int)t0.x, (int)t0.y, (int)t1.x, (int)t1.y, Color::WHITH_COLOR);
		DrawLine((int)t1.x, (int)t1.y, (int)t2.x, (int)t2.y, Color::WHITH_COLOR);
		DrawLine((int)t2.x, (int)t2.y, (int)t0.x, (int)t0.y, Color::WHITH_COLOR);
	}
	
	else if(scnManager->GetRenderState() & (RENDER_STATE_COLOR | RENDER_STATE_TEXTURE))
	{
		// Perspective correction
		c0 = c0 * t0.w;
		c1 = c1 * t1.w;
		c2 = c2 * t2.w;
		
		uv0 = uv0 * t0.w;
		uv1 = uv1 * t1.w;
		uv2 = uv2 * t2.w;

		n0 = n0 * t0.w;
		n1 = n1 * t1.w;
		n2 = n2 * n2.w;
		
		if (t0.y > t1.y) { std::swap(t0, t1); std::swap(c0, c1); std::swap(uv0, uv1); std::swap(n0, n1); }
		if (t0.y > t2.y) { std::swap(t0, t2); std::swap(c0, c2); std::swap(uv0, uv2); std::swap(n0, n2); }
		if (t1.y > t2.y) { std::swap(t1, t2); std::swap(c1, c2); std::swap(uv1, uv2); std::swap(n1, n2); }

		float total_height = t2.y - t0.y;
		// plus 0.5 for rounding
		for (int y = (int)(t0.y + 0.5f); y <= (int)(t1.y + 0.5f); y++)
		{
			// Because of up set down Y. Add a checkpoint here.
			if (y >= canvas->GetHeight() || y < 0) break;
			// This check  additional pixel 
			//if(std::abs((float)y - t0.y)<0.5f) continue;
			float segement_height = t1.y - t0.y;
			//if(segement_height < 1.0f) continue;
			float alpha = (float)(y - t0.y) / total_height;
			float beta = (float)(y - t0.y) / segement_height;
			//position lerp
			Vector4 A = Vector4::ClampLerp(t0, t2, alpha);
			Vector4 B = Vector4::ClampLerp(t0, t1, beta);
			//color lerp
			Color C = Color::ClampLerp(c0, c2, alpha);
			Color D = Color::ClampLerp(c0, c1, beta);
			//texcoord lerp
			Vector3 E = Vector3::ClampLerp(uv0, uv2, alpha);
			Vector3 F = Vector3::ClampLerp(uv0, uv1, beta);
			//normal lerp
			Vector4 G = Vector4::ClampLerp(n0, n2, alpha);
			Vector4 H = Vector4::ClampLerp(n0, n1, alpha);

			//draw line from left to right
			if (A.x > B.x)
			{
				std::swap(A, B);
				std::swap(C, D);
				std::swap(E, F);
				std::swap(G, H);
			}

			//zbuffer caculation
			float scanline_depth = B.z - A.z;
			float scanline_width = B.x - A.x;
			float depth_ratio = scanline_depth / scanline_width;
			float *z_line_buffer = canvas->GetZBuffer()[y];

			//w caculation
			float w_diff = B.w - A.w;
			float w_ratio = w_diff / scanline_width;
			

			//color caculation
			Color color_diff = D - C;
			Color color_ratio = color_diff / scanline_width;

			//UV caculation
			Vector3 uv_diff = F - E;
			Vector3 uv_ratio = uv_diff / scanline_width;

			//normal caculation
			Vector4 norm_diff = H - G;
			Vector4 norm_ratio = norm_diff / scanline_width;

			for (int j = (int)(A.x + 0.5f); j < (int)(B.x + 0.5f); ++j)
			{
				float step = (float)j - A.x + 0.5f;
				if (j >= canvas->GetWidth() || j < 0) break;
				float z = depth_ratio * step + A.z;
				float w = w_ratio * step + A.w;

				Vector4 normal;
				normal = (norm_ratio * step + G);
				normal = normal / w;

				if (z_line_buffer[j] > z)
				{
					Color color;
					Vector3 uv;
					color = (color_ratio * step + C);
					color = color / w;

					if (scnManager->GetRenderState() & RENDER_STATE_TEXTURE)
					{
						Color texcolor;
						uv = (uv_ratio * step + E);
						float w = w_ratio * step + A.w;
						// Perspective correction
						uv = uv / w;
						texcolor = scnManager->GetCurrentTextures()->GetColor(uv, currModelIndex);
						color = color * texcolor;
					}					
					z_line_buffer[j] = z;
					if (TestVertexInShadow(Vector4(j, y, z, w), normal))
						color = color * Color(0.15f, 0.15f, 0.15f);
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
			float segement_height = t2.y - t1.y;
			//if (segement_height < 1.0f) continue;
			float alpha = (float)(y - t0.y) / total_height;
			float beta = (float)(y - t1.y) / segement_height;
			//position lerp
			Vector4 A = Vector4::ClampLerp(t0, t2, alpha);
			Vector4 B = Vector4::ClampLerp(t1, t2, beta);
			//color lerp
			Color C = Color::ClampLerp(c0, c2, alpha);
			Color D = Color::ClampLerp(c1, c2, beta);
			//texcoord lerp
			Vector3 E = Vector3::ClampLerp(uv0, uv2, alpha);
			Vector3 F = Vector3::ClampLerp(uv1, uv2, beta);
			//normal lerp
			Vector4 G = Vector4::ClampLerp(n0, n2, alpha);
			Vector4 H = Vector4::ClampLerp(n0, n1, alpha);
			//draw line from left to right
			if (A.x > B.x)
			{
				std::swap(A, B);
				std::swap(C, D);
				std::swap(E, F);
				std::swap(G, H);
			}

			//for zbuffer caculation
			float scanline_depth = B.z - A.z; 
			float scanline_width = B.x - A.x;
			float depth_ratio = scanline_depth / scanline_width;
			float *z_line_buffer = canvas->GetZBuffer()[y];

			//w caculation
			float w_diff = B.w - A.w;
			float w_ratio = w_diff / scanline_width;

			//color caculation
			Color color_diff = D - C;
			Color color_ratio = color_diff / scanline_width;

			//UV caculation
			Vector3 uv_diff = F - E;
			Vector3 uv_ratio = uv_diff / scanline_width;

			//normal caculation
			Vector4 norm_diff = H - G;
			Vector4 norm_ratio = norm_diff / scanline_width;

			for (int j = (int)(A.x + 0.5f); j < (int)(B.x + 0.5f); ++j)
			{
				float step = (float)j - A.x + 0.5f;
				if (j >= canvas->GetWidth() || j < 0) break;
				float z = depth_ratio * step + A.z;
				float w = w_ratio * step + A.w;

				Vector4 normal;
				normal = (norm_ratio * step + G);
				normal = normal / w;

				if (z_line_buffer[j] > z)
				{
					Color color;
					Vector3 uv;

					color = (color_ratio * step + C);
					float w = w_ratio * step + A.w;
					color = color / w;

					if (scnManager->GetRenderState() & RENDER_STATE_TEXTURE)
					{
						Color texcolor;
						uv = (uv_ratio * step + E);
						float w = w_ratio * step + A.w;
						// Perspective correction
						uv = uv / w;
						texcolor = scnManager->GetCurrentTextures()->GetColor(uv, currModelIndex);
						color = color * texcolor;
					}
					z_line_buffer[j] = z;
					if (TestVertexInShadow(Vector4(j, y, z, w), normal))
						color = color * Color(0.15f, 0.15f, 0.15f);
					DrawPixel(j, y, color.GetIntensity());
				}
			}
		}
	}
}

bool Rasterizer::TestVertexInShadow(const Vector4& vert, const Vector4& normal)
{
	
	Vector4 screen_point = InvTransformHomogenize(vert);
	screen_point = screen_point * camera->GetInvProjectionMatrix();

	Vector4 world_point = screen_point * camera->GetInvViewMatrix();

	screen_point = world_point * scnManager->GetCurrentLight()->GetViewMatrix();
	screen_point = screen_point * scnManager->GetCurrentLight()->GetProjectionMatrix();

	screen_point = TransformHomogenize(screen_point);

	int x = (int)screen_point.x, y = (int)screen_point.y;
	float z = screen_point.z;

	if (y >= canvas->GetHeight() || x > canvas->GetWidth() || y<= 0|| x<0)
		return false;

	DirectLight* light = dynamic_cast<DirectLight*>(scnManager->GetCurrentLight());
	Vector4 light_dir = -light->GetDirection();
	// we have bise here. we don't need front culling anymore.
	float dot = normal.Dot(light_dir);
	//dot = std::clamp(dot, 0.0f, 1.0f);
	float bias = std::fmaxf(0.5f * (1.0 - dot), 0.15);
	float closestDepth = canvas->GetShadowBuffer()[y][x];
	float currentDepth = z - bias;
	if (closestDepth < currentDepth)
		return true;
	else
		return false;
}

void Rasterizer::DrawSomthing()
{
	nTriangle = 0;
	Model *model;

	int max_models = (int)scnManager->GetCurrentModels()->GetSize();
	for (currModelIndex = 0; currModelIndex < max_models; ++currModelIndex)
	{
		model = scnManager->GetCurrentModels()->GetModel(currModelIndex);

		for (int i = 0; i < model->Nfaces(); i++) {
			Triangle camera_tri,light_tri;
			light_tri = LightTriangleTransfrom(model->GetFaceIndex(i));
			camera_tri = CameraTriangleTransfrom(model->GetFaceIndex(i));
			ClipCVV(camera_tri,light_tri);
		}
	}
	window->SetNtri(nTriangle);
}

void Rasterizer::Update()
{
	renderPass = scnManager->GetRenderPass();
	canvas->ClearShadowBuffer();
	while (renderPass--)
	{
		canvas->ClearFrameBuffer();
		canvas->ClearZBuffer();
		camera->Update();
		scnManager->GetCurrentLight()->Update();
		DrawSomthing();
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