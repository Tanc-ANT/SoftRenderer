#include"Rasterizer.h"

Vector4 mesh[8] = {
	{-1.0f,-1.0f,1.0f,1.0f},
	{ 1.0f,-1.0f,1.0f,1.0f},
	{ 1.0f, 1.0f,1.0f,1.0f},
	{-1.0f, 1.0f,1.0f,1.0f},
	{-1.0f,-1.0f,-1.0f,1.0f},
	{ 1.0f,-1.0f,-1.0f,1.0f},
	{ 1.0f, 1.0f,-1.0f,1.0f},
	{-1.0f, 1.0f,-1.0f,1.0f},
};

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

void Rasterizer::DrawPixel(int x, int y, UINT32 color)
{
	if (((unsigned int)x < (UINT32)device->width) && (((unsigned int)y < (UINT32)device->height))
		&& (((unsigned int)x > (UINT32)0)) && (((unsigned int)y > (UINT32)0)))
		
	{
		//y = device->height - y;
		device->framebuffer[y][x] = color;
	}
}

void Rasterizer::DrawLine(int x0, int y0, int x1, int y1, UINT32 color)
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

void Rasterizer::DrawTriangle(Vector4 t0, Vector4 t1, Vector4 t2, const UINT32& color)
{
	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);

	int total_height = t2.y - t0.y;
	if (total_height < 0.00001) return;
	for (int y = t0.y; y <= t1.y; ++y)
	{
		// Because of up set down Y. Add a checkpoint here.
		if (y >= device->height) break;
		int segement_height = t1.y - t0.y + 1;//plus 1 for non zero division
		float alpha = (float)(y - t0.y) / total_height;
		float beta = (float)(y - t0.y) / segement_height;
		Vector4 A = t0 + (t2 - t0)*alpha;
		Vector4 B = t0 + (t1 - t0)*beta;
		if (A.x > B.x) std::swap(A, B); //draw line from left to right

		float scanline_depth = B.w - A.w; //for zbuffer caculation
		float scanline_width = B.x - A.x;
		float ratio = scanline_depth / scanline_width;
		float *z_line_buffer = device->zbuffer[y];
		for (int j = A.x; j < B.x; ++j)
		{
			float z = (j - A.x) * ratio + A.w;
			if (z_line_buffer[j] <= z)
			{
				z_line_buffer[j] = z;
				DrawPixel(j, y, color);
			}
		}
	}
	for (int y = t1.y; y <= t2.y; ++y)
	{
		// Because of up set down Y. Add a checkpoint here.
		if (y >= device->height) break;
		int segement_height = t2.y - t1.y + 1;//plus 1 for non zero division
		float alpha = (float)(y - t0.y) / total_height;
		float beta = (float)(y - t1.y) / segement_height;
		Vector4 A = t0 + (t2 - t0)*alpha;
		Vector4 B = t1 + (t2 - t1)*beta;
		if (A.x > B.x) std::swap(A, B);//draw line from left to right

		float scanline_depth = B.w - A.w; //for zbuffer caculation
		float scanline_width = B.x - A.x;
		float ratio = scanline_depth / scanline_width;
		float *z_line_buffer = device->zbuffer[y];
		for (int j = A.x; j < B.x; ++j)
		{
			float z = (j - A.x) * ratio + A.w;
			if (z_line_buffer[j] <= z)
			{
				z_line_buffer[j] = z;
				DrawPixel(j, y, color);
			}
		}
	}
}

void Rasterizer::DrawPlane(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d)
{
	if (device->render_state == RENDER_BOX_WIREFRAME)
	{
		DrawLine(a.x, a.y, b.x, b.y, WHITH_COLOR);
		DrawLine(b.x, b.y, c.x, c.y, WHITH_COLOR);
		DrawLine(c.x, c.y, a.x, a.y, WHITH_COLOR);

		DrawLine(c.x, c.y, d.x, d.y, WHITH_COLOR);
		DrawLine(d.x, d.y, a.x, a.y, WHITH_COLOR);
		DrawLine(a.x, a.y, c.x, c.y, WHITH_COLOR);
	}
	else if (device->render_state == RENDER_BOX_COLOR)
	{
		DrawTriangle(a, b, c, WHITH_COLOR);
		DrawTriangle(c, d, a, WHITH_COLOR);
	}
}

void Rasterizer::DrawBox(const Vector4 points[],int n)
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
	Matrix4 world;
	world.SetIdentity();
	world.Rotation(Vector3(0, 1, 0), 90);
	Matrix4 view = camera->GetViewMatrix();
	Matrix4 proj = camera->GetProjectionMatrix();
	Matrix4 t = world * view;
	transform = t * proj;
	
	if (device->render_state == RENDER_MODEL_WIREFRAME)
	{
		Vector4 screen_points[2];
		Vector4 world_points[2];
		for (int i = 0; i < model->nfaces(); i++) {
			std::vector<int> face = model->face(i);
			for (int j = 0; j < 3; j++) {
				Vector4 v0 = Vector4(model->vert(face[j]), 1.0f);
				Vector4 v1 = Vector4(model->vert(face[(j + 1) % 3]), 1.0f);
				world_points[0] = TransformApply(v0, transform);
				screen_points[0] = TransformHomogenize(world_points[0]);
				world_points[1] = TransformApply(v1, transform);
				screen_points[1] = TransformHomogenize(world_points[1]);

				DrawLine(screen_points[0].x, screen_points[0].y,
					screen_points[1].x, screen_points[1].y, WHITH_COLOR);
			}
		}
	}
	else if (device->render_state == RENDER_MODEL_COLOR)
	{
		for (int i = 0; i < model->nfaces(); i++) {
			std::vector<int> face = model->face(i);
			Vector4 screen_points[3];
			Vector4 world_points[3];
			for (int j = 0; j < 3; j++) {
				Vector4 v = Vector4(model->vert(face[j]), 1.0f);
				world_points[j] = TransformApply(v,transform);
				screen_points[j] = TransformHomogenize(world_points[j]);
				
			}
			Vector4 v1 = world_points[2] - world_points[1];
			Vector4 v2 = world_points[1] - world_points[0];
			Vector4 n = v1.Cross(v2);
			n.Normalize();
			Vector4 light_dir(0, 0, 1, 1);
			float intenstiy = n.Dot(light_dir);
			UINT32 color = intenstiy * 255;
			color = (color) | (color << 8) | (color << 16);
			if (intenstiy > 0)
			{
				DrawTriangle(screen_points[0], screen_points[1],
					screen_points[2], color);
			}
		}
	}
	else
	{
		Vector4 world_points[8];
		Vector4 screen_points[8];
		for (int i = 0; i < 8; ++i)
		{
			world_points[i] = TransformApply(mesh[i], transform);
			screen_points[i] = TransformHomogenize(world_points[i]);
		}
		DrawBox(screen_points, 8);
	}
	
}

void Rasterizer::Update()
{
	device->Clear();
	DrawSomthing();
}

Vector4 Rasterizer::TransformHomogenize(const Vector4& v)
{
	float rhw = 1.0f / v.w;
	return Vector4((v.x * rhw + 1.0f)*device->width * 0.5f,
		(1.0f - v.y * rhw)*device->height * 0.5f,
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
