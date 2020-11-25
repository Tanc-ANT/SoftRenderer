#include "Rasterizer.h"
#include "Color.h"

Vertex mesh[8] = {
	{Vector4(-1.0f,-1.0f,1.0f,1.0f),Color(1.0f,0.2f,0.2f)},
	{Vector4( 1.0f,-1.0f,1.0f,1.0f),Color(0.2f,1.0f,0.2f)},
	{Vector4( 1.0f, 1.0f,1.0f,1.0f),Color(0.2f,0.2f,1.0f)},
	{Vector4(-1.0f, 1.0f,1.0f,1.0f),Color(1.0f,0.2f,1.0f)},
	{Vector4(-1.0f,-1.0f,-1.0f,1.0f),Color(1.0f,1.0f,0.2f)},
	{Vector4( 1.0f,-1.0f,-1.0f,1.0f),Color(0.2f,1.0f,1.0f)},
	{Vector4( 1.0f, 1.0f,-1.0f,1.0f),Color(1.0f,0.3f,0.3f)},
	{Vector4(-1.0f, 1.0f,-1.0f,1.0f),Color(0.2f,1.0f,0.3f)},
};

void Rasterizer::SetWindow(Window *w)
{
	window = w;
}

Window* Rasterizer::GetWindow()
{
	return window;
}

void Rasterizer::SetDevice(Device* d)
{
	device = d;
}

Device* Rasterizer::GetDevice()
{
	return device;
}

void Rasterizer::SetModel(Model* m)
{
	model = m;
}

Model* Rasterizer::GetModel()
{
	return model;
}

void Rasterizer::SetCamera(Camera* c)
{
	camera = c;
}

Camera* Rasterizer::GetCamera()
{
	return camera;
}

Vector4 Rasterizer::TransformHomogenize(const Vector4& v)
{
	float rhw = 1.0f / v.w;
	return Vector4((v.x * rhw + 1.0f)*device->GetWidth() * 0.5f,
		(1.0f - v.y * rhw)*device->GetHeight() * 0.5f,
		v.z*rhw, rhw);
}

Vector4 Rasterizer::TransformApply(const Vector4& v, const Matrix4& m)
{
	float X = v.x; float Y = v.y; float Z = v.z; float W = v.w;
	Vector4 u;
	u.x = X * m.m[0][0] + Y * m.m[1][0] + Z * m.m[2][0] + W * m.m[3][0];
	u.y = X * m.m[0][1] + Y * m.m[1][1] + Z * m.m[2][1] + W * m.m[3][1];
	u.z = X * m.m[0][2] + Y * m.m[1][2] + Z * m.m[2][2] + W * m.m[3][2];
	u.w = X * m.m[0][3] + Y * m.m[1][3] + Z * m.m[2][3] + W * m.m[3][3];
	return u;
}

Color Rasterizer::ColorHomogenize(const Color& c,const float& w)
{
	// float rhw = 1 / w;
	// color divid by rhw 
	return c * w;
}

void Rasterizer::DrawPixel(int x, int y, Color color)
{
	if (((unsigned int)x < (UINT32)device->GetWidth()) && (((unsigned int)y < (UINT32)device->GetHeight()))
		&& (((unsigned int)x > (UINT32)0)) && (((unsigned int)y > (UINT32)0)))
		
	{
		//y = device->height - y;
		device->GetFrameBuffer()[y][x] = color.GetIntensity();
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
			DrawPixel(y, x, color);
		}
		else {
			DrawPixel(x, y, color);
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
	if (FaceCulling(t))
		return;

	Vector4 t0 = t.GetV0().GetVertexPosition();
	Vector4 t1 = t.GetV1().GetVertexPosition();
	Vector4 t2 = t.GetV2().GetVertexPosition();

	if (device->GetRenderState() & RENDER_STATE_WIREFRAME)
	{
		DrawLine(t0.x, t0.y, t1.x, t1.y, WHITH_COLOR);
		DrawLine(t1.x, t1.y, t2.x, t2.y, WHITH_COLOR);
		DrawLine(t2.x, t2.y, t0.x, t0.y, WHITH_COLOR);
	}
	
	else if(device->GetRenderState() & RENDER_STATE_COLOR)
	{
		Color c0 = ColorHomogenize(t.GetV0().GetVertexColor(), t0.w);
		Color c1 = ColorHomogenize(t.GetV1().GetVertexColor(), t1.w);
		Color c2 = ColorHomogenize(t.GetV2().GetVertexColor(), t2.w);

		if (t0.y > t1.y) { std::swap(t0, t1); std::swap(c0, c1); }
		if (t0.y > t2.y) { std::swap(t0, t2); std::swap(c0, c2); }
		if (t1.y > t2.y) { std::swap(t1, t2); std::swap(c1, c2); }

		float total_height = t2.y - t0.y;
		if (total_height <= 1.0f) return;
		for (float y = t0.y; y <= t1.y; y+=1.0f)
		{
			// Because of up set down Y. Add a checkpoint here.
			if (y >= device->GetHeight() || y<0) break;
			float segement_height = t1.y - t0.y + EPSILON;//plus EPSILON for non zero division
			float alpha = (float)(y - t0.y) / total_height;
			float beta = (float)(y - t0.y) / segement_height;
			Vector4 A = t0 + (t2 - t0)*alpha;
			Vector4 B = t0 + (t1 - t0)*beta;
			Color C = c0 + (c2 - c0)*alpha;
			Color D = c0 + (c1 - c0)*beta;
			//draw line from left to right
			if (A.x > B.x) { std::swap(A, B); std::swap(C, D); }

			//zbuffer caculation
			float scanline_depth = B.w - A.w; 
			float scanline_width = B.x - A.x;
			float depth_ratio = scanline_depth / scanline_width;
			float *z_line_buffer = device->GetZBuffer()[(int)y];

			//color caculation
			Color color_diff = D - C;
			Color color_ratio = color_diff / scanline_width;
			
			for (float j = A.x; j < B.x; j+=1.0f)
			{
				if (j >= device->GetWidth() || j< 0) break;
				float z = depth_ratio * (j - A.x) + A.w;
				if (z_line_buffer[(int)j] <= z)
				{
					Color c = (color_ratio * (j - A.x) + C) / z;
					z_line_buffer[(int)j] = z;
					DrawPixel(j, y, c);
				}
			}
		}
		for (float y = t1.y; y <= t2.y; y+=1.0f)
		{
			// Because of up set down Y. Add a checkpoint here.
			if (y >= device->GetHeight() || y < 0) break;
			float segement_height = t2.y - t1.y + EPSILON;//plus EPSILON for non zero division
			float alpha = (float)(y - t0.y) / total_height;
			float beta = (float)(y - t1.y) / segement_height;
			Vector4 A = t0 + (t2 - t0)*alpha;
			Vector4 B = t1 + (t2 - t1)*beta;
			Color C = c0 + (c2 - c0)*alpha;
			Color D = c1 + (c2 - c1)*beta;
			//draw line from left to right
			if (A.x > B.x) { std::swap(A, B); std::swap(C, D); }

			//for zbuffer caculation
			float scanline_depth = B.w - A.w; 
			float scanline_width = B.x - A.x;
			float depth_ratio = scanline_depth / scanline_width;
			float *z_line_buffer = device->GetZBuffer()[(int)y];

			//color caculation
			Color color_diff = D - C;
			Color color_ratio = color_diff / scanline_width;

			for (float j = A.x; j < B.x; j+=1.0f)
			{
				if (j >= device->GetWidth() || j < 0) break;
				float z = depth_ratio * (j - A.x) + A.w;
				if (z_line_buffer[(int)j] <= z)
				{
					Color c = (color_ratio * (j - A.x) + C) / z;
					z_line_buffer[(int)j] = z;
					DrawPixel(j, y, c);
				}
			}
		}
	}
}

void Rasterizer::DrawPlane(const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d)
{
	DrawTriangle(Triangle(a, b, c));
	DrawTriangle(Triangle(c, d, a));
}

void Rasterizer::DrawBox(const Vertex points[],int n)
{
	DrawPlane(points[0], points[1], points[2], points[3]);
	DrawPlane(points[7], points[6], points[5], points[4]);
	DrawPlane(points[0], points[4], points[5], points[1]);
	DrawPlane(points[1], points[5], points[6], points[2]);
	DrawPlane(points[2], points[6], points[7], points[3]);
	DrawPlane(points[3], points[7], points[4], points[0]);
}

void Rasterizer::DrawSomthing()
{	
	if (device->GetRenderState() & RENDER_STATE_MODEL)
	{
		for (int i = 0; i < model->nfaces(); i++) {
			std::vector<int> face = model->face(i);
			Vector4 screen_points[3];
			Vector4 world_points[3];
			for (int j = 0; j < 3; j++) {
				Vector4 v = Vector4(model->vert(face[j]), 1.0f);
				world_points[j] = TransformApply(v, camera->GetTranformation());
				screen_points[j] = TransformHomogenize(world_points[j]);

			}
			Vector4 v1 = world_points[2] - world_points[1];
			Vector4 v2 = world_points[1] - world_points[0];
			Vector4 n = v1.Cross(v2);
			n.Normalize();
			Vector4 light_dir(0, 0, 1, 1);
			float light_color = n.Dot(light_dir);
			Color color(light_color, light_color, light_color);
			Vertex a(screen_points[0], color);
			Vertex b(screen_points[1], color);
			Vertex c(screen_points[2], color);
			Triangle t(a, b, c);
			if (light_color > 0)
			{
				DrawTriangle(t);
			}
		}
	}
	else if(device->GetRenderState() & RENDER_STATE_BOX)
	{
		Vertex vert[8];
		Vector4 world_points[8];
		Vector4 screen_points[8];
		for (int i = 0; i < 8; ++i)
		{
			world_points[i] = TransformApply(mesh[i].GetVertexPosition(), camera->GetTranformation());
			screen_points[i] = TransformHomogenize(world_points[i]);
			vert[i].SetVertexPosition(screen_points[i]);
			vert[i].SetVertexColor(mesh[i].GetVertexColor());
		}
		DrawBox(vert, 8);
	}
}

void Rasterizer::Update()
{
	device->Clear();
	camera->Update();
	DrawSomthing();
	window->Update();
	InputKeysEvent();
}

bool Rasterizer::FaceCulling(const Triangle& t) const
{
	if (device->GetRenderState() & RENDER_STATE_BACKCULL)
	{
		Vector4 t0 = t.GetV0().GetVertexPosition();
		Vector4 t1 = t.GetV1().GetVertexPosition();
		Vector4 t2 = t.GetV2().GetVertexPosition();

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

void Rasterizer::InputKeysEvent()
{
	if (window->GetKey()[VK_ESCAPE]) window->SetCloseState(true);
	if (window->GetKey()[VK_UP]) camera->TranslateFront();
	if (window->GetKey()[VK_DOWN]) camera->TranslateBack();
	if (window->GetKey()[VK_LEFT]) camera->RotateLeft();
	if (window->GetKey()[VK_RIGHT]) camera->RotateRight();
	if (window->GetKey()[VK_SPACE])
	{
		if (!change_state)
		{
			change_state = true;
			int m = device->GetRenderMode();
			m = (m + 1) % MODE;
			device->SetRenderState(m);
		}
	}
	else
		change_state = false;
}
