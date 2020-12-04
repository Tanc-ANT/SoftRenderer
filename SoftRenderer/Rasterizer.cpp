#include "Rasterizer.h"

Vertex mesh[8] = {
	{Vector4(-1.0f,-1.0f,1.0f,1.0f),Color(1.0f,0.2f,0.2f)},  //0
	{Vector4(1.0f,-1.0f,1.0f,1.0f),Color(0.2f,1.0f,0.2f)},	//1
	{Vector4(1.0f, 1.0f,1.0f,1.0f),Color(0.2f,0.2f,1.0f)},	//2
	{Vector4(-1.0f, 1.0f,1.0f,1.0f),Color(1.0f,0.2f,1.0f)},	//3
	{Vector4(-1.0f,-1.0f,-1.0f,1.0f),Color(1.0f,1.0f,0.2f)},	//4
	{Vector4(1.0f,-1.0f,-1.0f,1.0f),Color(0.2f,1.0f,1.0f)},	//5
	{Vector4(1.0f, 1.0f,-1.0f,1.0f),Color(1.0f,0.3f,0.3f)},	//6
	{Vector4(-1.0f, 1.0f,-1.0f,1.0f),Color(0.2f,1.0f,0.3f)},	//7
};

Rasterizer::Rasterizer()
{
	SetBoxNormal();
};

Rasterizer::~Rasterizer()
{

};

void Rasterizer::SetBoxNormal()
{

	Vector4 v0 = mesh[1].GetVertexPosition() - mesh[0].GetVertexPosition();
	Vector4 v1 = mesh[2].GetVertexPosition() - mesh[0].GetVertexPosition();
	Vector4 n0 = v0.Cross(v1);

	Vector4 v2 = mesh[6].GetVertexPosition() - mesh[7].GetVertexPosition();
	Vector4 v3 = mesh[5].GetVertexPosition() - mesh[7].GetVertexPosition();
	Vector4 n1 = v2.Cross(v3);

	Vector4 v4 = mesh[7].GetVertexPosition() - mesh[3].GetVertexPosition();
	Vector4 v5 = mesh[4].GetVertexPosition() - mesh[3].GetVertexPosition();
	Vector4 n2 = v4.Cross(v5);

	Vector4 v6 = mesh[5].GetVertexPosition() - mesh[1].GetVertexPosition();
	Vector4 v7 = mesh[6].GetVertexPosition() - mesh[1].GetVertexPosition();
	Vector4 n3 = v6.Cross(v7);

	Vector4 v8 = mesh[6].GetVertexPosition() - mesh[2].GetVertexPosition();
	Vector4 v9 = mesh[7].GetVertexPosition() - mesh[2].GetVertexPosition();
	Vector4 n4 = v8.Cross(v9);

	Vector4 v10 = mesh[4].GetVertexPosition() - mesh[0].GetVertexPosition();
	Vector4 v11 = mesh[5].GetVertexPosition() - mesh[0].GetVertexPosition();
	Vector4 n5 = v10.Cross(v11);

	Vector4 norm0 = n0 + n2 + n5;
	norm0.Normalize();
	mesh[0].SetVertexNormal(norm0);

	Vector4 norm1 = n0 + n3 + n5;
	norm1.Normalize();
	mesh[1].SetVertexNormal(norm1);

	Vector4 norm2 = n0 + n3 + n4;
	norm2.Normalize();
	mesh[2].SetVertexNormal(norm2);

	Vector4 norm3 = n0 + n2 + n4;
	norm3.Normalize();
	mesh[3].SetVertexNormal(norm3);

	Vector4 norm4 = n1 + n2 + n5;
	norm4.Normalize();
	mesh[4].SetVertexNormal(norm4);

	Vector4 norm5 = n1 + n3 + n5;
	norm5.Normalize();
	mesh[5].SetVertexNormal(norm5);

	Vector4 norm6 = n1 + n3 + n4;
	norm6.Normalize();
	mesh[6].SetVertexNormal(norm6);

	Vector4 norm7 = n1 + n2 + n4;
	norm7.Normalize();
	mesh[7].SetVertexNormal(norm7);
}

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

void Rasterizer::TransformCheckCVV(const Triangle& t)
{
	std::vector<Vertex> vert_list;
	std::vector<Vertex> in_list1;
	std::vector<Vertex> in_list2;

	vert_list.push_back(t.GetV0());
	vert_list.push_back(t.GetV1());
	vert_list.push_back(t.GetV2());

	float cosAh = cos(camera->GetFovY() / 2);
	float sinAh = sin(camera->GetFovY() / 2);

	ClipWithPlane(Vector4(0, 0, camera->GetNear(), 1), Vector4(0, 0, 1, 0),
		vert_list, in_list1);	//near
	ClipWithPlane(Vector4(0, 0, camera->GetFar(), 1), Vector4(0, 0, -1, 0),
		in_list1, in_list2);	//far

	int num_vertex = (int)in_list2.size() - 2;
	for (int i = 0; i < num_vertex; ++i)
	{
		int index0 = 0;
		int index1 = i + 1;
		int index2 = i + 2;
		Triangle triangle(in_list2[index0],
			in_list2[index1], in_list2[index2]);
		DrawTriangle(triangle);
	}
}

void Rasterizer::LightCalculaiton(Vertex& v)
{
	if (canvas->GetRenderState() & RENDER_STATE_LIGHT)
	{
		float ambient = 0.1f;
		float diffuse = 0.0f;
		float specular = 0.0f;

		//diffuse caculation
		Vector4 n = v.GetVertexNormal();
		Vector4 light_dir =  light->GetPosition() - v.GetVertexPosition();
		light_dir.Normalize();
		diffuse = n.Dot(light_dir);

		//specular caculaiton
		Vector4 view_dir = Vector4(camera->GetPostion(),1.0f) - v.GetVertexPosition();
		view_dir.Normalize();
		Vector4 halfway_dir = light_dir + view_dir;
		halfway_dir.Normalize();
		specular = std::powf(std::fmaxf(n.Dot(halfway_dir), 0.0), 32);

		//blinn-phone
		Color color = v.GetVertexColor();
		color = (color * (diffuse + ambient + specular))*light->GetColor();
		v.SetVertexColor(color);
	}
}

void Rasterizer::DrawPixel(int x, int y, UINT32 color)
{
	if (((unsigned int)x < (UINT32)canvas->GetWidth()) && (((unsigned int)y < (UINT32)canvas->GetHeight()))
		&& (((unsigned int)x >= (UINT32)0)) && (((unsigned int)y >= (UINT32)0)))
		
	{
		//y = device->height - y;
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

void Rasterizer::DrawTriangle(const Triangle& t)
{
	Vector4 t0 = t.GetV0().GetVertexPosition();
	Vector4 t1 = t.GetV1().GetVertexPosition();
	Vector4 t2 = t.GetV2().GetVertexPosition();

	t0 = TransformHomogenize(t0);
	t1 = TransformHomogenize(t1);
	t2 = TransformHomogenize(t2);

	if (FaceCulling(t0,t1,t2))
		return;

	nTriangle += 1;

	Color c0 = t.GetV0().GetVertexColor();
	Color c1 = t.GetV1().GetVertexColor();
	Color c2 = t.GetV2().GetVertexColor();

	Vector3 uv0 = t.GetV0().GetVertexTexcoord();
	Vector3 uv1 = t.GetV1().GetVertexTexcoord();
	Vector3 uv2 = t.GetV2().GetVertexTexcoord();

	if (canvas->GetRenderState() & RENDER_STATE_WIREFRAME)
	{
		DrawLine((int)t0.x, (int)t0.y, (int)t1.x, (int)t1.y, Color::WHITH_COLOR);
		DrawLine((int)t1.x, (int)t1.y, (int)t2.x, (int)t2.y, Color::WHITH_COLOR);
		DrawLine((int)t2.x, (int)t2.y, (int)t0.x, (int)t0.y, Color::WHITH_COLOR);
	}
	
	else if(canvas->GetRenderState() & (RENDER_STATE_COLOR | RENDER_STATE_TEXTURE))
	{
		
		if (canvas->GetRenderState() & RENDER_STATE_PERSPECTIVE)
		{
			c0 = c0 * t0.w;
			c1 = c1 * t1.w;
			c2 = c2 * t2.w;

			uv0 = uv0 * t0.w;
			uv1 = uv1 * t1.w;
			uv2 = uv2 * t2.w;
		}
		
		if (t0.y > t1.y) { std::swap(t0, t1); std::swap(c0, c1); std::swap(uv0, uv1); }
		if (t0.y > t2.y) { std::swap(t0, t2); std::swap(c0, c2); std::swap(uv0, uv2); }
		if (t1.y > t2.y) { std::swap(t1, t2); std::swap(c1, c2); std::swap(uv1, uv2); }

		float total_height = t2.y - t0.y;
		// plus 0.5 for rounding
		for (int y = (int)(t0.y + 0.5f); y <= (int)(t1.y + 0.5f); y++)
		{
			// Because of up set down Y. Add a checkpoint here.
			if (y >= canvas->GetHeight() || y < 0) break;
			// This check  additional pixel 
			//if(std::abs((float)y - t0.y)<0.5f) continue;
			float segement_height = t1.y - t0.y + EPSILON;
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
			//draw line from left to right
			if (A.x > B.x)
			{
				std::swap(A, B);
				std::swap(C, D);
				std::swap(E, F);
			}

			//zbuffer caculation
			float scanline_depth = B.z - A.z;
			float scanline_width = B.x - A.x;
			float depth_ratio = scanline_depth / scanline_width;
			float *z_line_buffer = canvas->GetZBuffer()[y];

			//w caculation
			float w_ratio = 1.0f;
			if (canvas->GetRenderState() & RENDER_STATE_PERSPECTIVE)
			{
				float w_diff = B.w - A.w;
				w_ratio = w_diff / scanline_width;
			}

			//color caculation
			Color color_diff = D - C;
			Color color_ratio = color_diff / scanline_width;

			//UV caculation
			Vector3 uv_diff = F - E;
			Vector3 uv_ratio = uv_diff / scanline_width;

			for (int j = (int)(A.x + 0.5f); j < (int)(B.x + 0.5f); ++j)
			{
				float step = (float)j - A.x + 0.5f;
				if (j >= canvas->GetWidth() || j < 0) break;
				float z = depth_ratio * step + A.z;
				
				if (z_line_buffer[j] >= z)
				{
					Color color;
					color = (color_ratio * step + C);
					if (canvas->GetRenderState() & RENDER_STATE_PERSPECTIVE)
					{
						float w = w_ratio * step + A.w;
						color = color / w;
					}
					if (canvas->GetRenderState() & RENDER_STATE_TEXTURE)
					{
						Color texcolor;
						Vector3 uv = (uv_ratio * step + E);
						if (canvas->GetRenderState() & RENDER_STATE_PERSPECTIVE)
						{
							float w = w_ratio * step + A.w;
							uv = uv / w;
						}
						if (canvas->GetRenderState() & RENDER_STATE_BOX)
							texcolor = textures->GetColor(uv, TEXTURE_INDEX_0);
						else if (canvas->GetRenderState() & RENDER_STATE_MODEL)
							texcolor = textures->GetColor(uv, TEXTURE_INDEX_1);
						color = color * texcolor;
					}					
					z_line_buffer[j] = z;
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
			//color lerp
			Color C = Color::ClampLerp(c0, c2, alpha);
			Color D = Color::ClampLerp(c1, c2, beta);
			//texcoord lerp
			Vector3 E = Vector3::ClampLerp(uv0, uv2, alpha);
			Vector3 F = Vector3::ClampLerp(uv1, uv2, beta);
			//draw line from left to right
			if (A.x > B.x)
			{
				std::swap(A, B);
				std::swap(C, D);
				std::swap(E, F);
			}

			//for zbuffer caculation
			float scanline_depth = B.z - A.z; 
			float scanline_width = B.x - A.x;
			float depth_ratio = scanline_depth / scanline_width;
			float *z_line_buffer = canvas->GetZBuffer()[y];

			//w caculation
			float w_ratio = 1.0f;
			if (canvas->GetRenderState() & RENDER_STATE_PERSPECTIVE)
			{
				float w_diff = B.w - A.w;
				w_ratio = w_diff / scanline_width;
			}

			//color caculation
			Color color_diff = D - C;
			Color color_ratio = color_diff / scanline_width;

			//UV caculation
			Vector3 uv_diff = F - E;
			Vector3 uv_ratio = uv_diff / scanline_width;

			for (int j = (int)(A.x + 0.5f); j < (int)(B.x + 0.5f); ++j)
			{
				float step = (float)j - A.x + 0.5f;
				if (j >= canvas->GetWidth() || j < 0) break;
				float z = depth_ratio * step + A.z;
				float w = w_ratio * step + A.w;
				if (z_line_buffer[(int)j] >= z)
				{
					Color color;
					color = (color_ratio * step + C);
					if (canvas->GetRenderState() & RENDER_STATE_PERSPECTIVE)
					{
						float w = w_ratio * step + A.w;
						color = color / w;
					}
					if (canvas->GetRenderState() & RENDER_STATE_TEXTURE)
					{
						Color texcolor;
						Vector3 uv = (uv_ratio * step + E);
						if (canvas->GetRenderState() & RENDER_STATE_PERSPECTIVE)
						{
							float w = w_ratio * step + A.w;
							uv = uv / w;
						}
						if (canvas->GetRenderState() & RENDER_STATE_BOX)
							texcolor = textures->GetColor(uv, TEXTURE_INDEX_0);
						else if (canvas->GetRenderState() & RENDER_STATE_MODEL)
							texcolor = textures->GetColor(uv, TEXTURE_INDEX_1);
						color = color * texcolor;
					}
					z_line_buffer[j] = z;
					DrawPixel(j, y, color.GetIntensity());
				}
			}
		}
	}
}

void Rasterizer::DrawPlane(Vertex& a, Vertex& b, Vertex& c, Vertex& d)
{
	// Set texcoord
	a.SetVertexTexcoord({ 0,1,0 });
	b.SetVertexTexcoord({ 0,0,0 });
	c.SetVertexTexcoord({ 1,0,0 });
	d.SetVertexTexcoord({ 1,1,0 });

	TransformCheckCVV(Triangle(a, b, c));
	TransformCheckCVV(Triangle(c, d, a));
}

void Rasterizer::DrawBox(Vertex points[],int n)
{
	DrawPlane(points[0], points[1], points[2], points[3]); // front
	DrawPlane(points[7], points[6], points[5], points[4]); // back
	DrawPlane(points[0], points[4], points[5], points[1]); // bottom
	DrawPlane(points[1], points[5], points[6], points[2]); // right
	DrawPlane(points[2], points[6], points[7], points[3]); // up
	DrawPlane(points[3], points[7], points[4], points[0]); // left
}

void Rasterizer::DrawSomthing()
{
	nTriangle = 0;
	if (canvas->GetRenderState() & RENDER_STATE_MODEL)
	{
		for (int i = 0; i < model->Nfaces(); i++) {
			Triangle t = model->GetFaceIndex(i);
			Vertex vertex_points[3] = { t.GetV0(),t.GetV1(),t.GetV2() };
			Vector4 world_points[3];
			Vector4 screen_points[3];
			for (int j = 0; j < 3; j++) {
				Vector4 v = vertex_points[j].GetVertexPosition();
				Vector3 t = vertex_points[j].GetVertexTexcoord();
				Vector4 n = vertex_points[j].GetVertexNormal();
				// Set color
				vertex_points[j].SetVertexColor(Color::WHITH_COLOR);
				// M transform
				world_points[j] = v * camera->GetModelMatrix();
				vertex_points[j].SetVertexPosition(world_points[j]);
				// normal set
				n = n * camera->GetModelMatrix();
				vertex_points[j].SetVertexNormal(n);
				// light caculation
				LightCalculaiton(vertex_points[j]);
				
				// VP transform
				screen_points[j] = world_points[j] * camera->GetViewMatrix();
				screen_points[j] = screen_points[j] * camera->GetProjectionMatrix();
				vertex_points[j].SetVertexPosition(screen_points[j]);
			}
			t = { vertex_points[0],vertex_points[1],vertex_points[2] };
			TransformCheckCVV(t);
		}
	}
	else if(canvas->GetRenderState() & RENDER_STATE_BOX)
	{
		Vertex vert[8];
		Vector4 world_points[8];
		Vector4 screen_points[8];
		for (int i = 0; i < 8; ++i)
		{
			// M transform
			world_points[i] = mesh[i].GetVertexPosition() * camera->GetModelMatrix();
			vert[i].SetVertexPosition(world_points[i]);
			// Normal Calculation
			Vector4 n = mesh[i].GetVertexNormal();		
			n = n * camera->GetModelMatrix();
			vert[i].SetVertexNormal(n);
			// Color Set
			if (canvas->GetRenderState() & RENDER_STATE_COLOR)
				vert[i].SetVertexColor(mesh[i].GetVertexColor());
			else if (canvas->GetRenderState() & RENDER_STATE_TEXTURE)
				vert[i].SetVertexColor(Color::WHITH_COLOR);
			// Texcoord Set
			vert[i].SetVertexTexcoord(mesh[i].GetVertexTexcoord());
			// Light Calculation
			LightCalculaiton(vert[i]);
			// VP transform
			screen_points[i] = world_points[i] * camera->GetViewMatrix();
			screen_points[i] = screen_points[i] * camera->GetProjectionMatrix();
			vert[i].SetVertexPosition(screen_points[i]);
		}
		DrawBox(vert, 8);
	}
	window->SetNtri(nTriangle);
}

void Rasterizer::Update()
{
	canvas->Clear();
	camera->Update();
	DrawSomthing();
	window->Update();
	ProcessWindowKeyInput();
	ProcessWindowMouseInput();
}

bool Rasterizer::FaceCulling(const Vector4& t0, const Vector4 t1, const Vector4 t2) const
{
	if (canvas->GetRenderState() & RENDER_STATE_BACKCULL)
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
		if (!change_state)
		{
			change_state = true;
			int m = canvas->GetRenderMode();
			m = (m + 1) % MODE;
			canvas->SetRenderState(m);
		}
	}
	else
		change_state = false;

	if (canvas->GetRenderMode() == 0)	light->SetColor(Color(1.0f, 1.0f, 1.0f));
	else if (canvas->GetRenderMode() == 6)	light->SetColor(Color(1.0f, 1.0f, 1.0f));
	else if (canvas->GetRenderMode() == 7)	light->SetColor(Color(1.0f, 1.0f, 1.0f));
	else if (canvas->GetRenderMode() == 8)	light->SetColor(Color(0.8f, 0.5f, 0.5f));
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