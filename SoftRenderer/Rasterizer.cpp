#include"Rasterizer.h"

Rasterizer::Rasterizer()
{
	
}

Rasterizer::~Rasterizer()
{

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

void Rasterizer::DrawPixel(int x, int y, UINT32 color)
{
	if (((unsigned int)x < (UINT32)device->width) && (((unsigned int)y < (UINT32)device->height))
		&& (((unsigned int)x > (UINT32)0)) && (((unsigned int)y > (UINT32)0)))
	{
		y = device->height - y;
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

void Rasterizer::DrawTriangle(Vector3 t0, Vector3 t1, Vector3 t2, UINT32 color)
{
	//TODO: Change 2D trangle to 3D triangle
	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);

	if (device->render_state & RENDER_STATE_WIREFRAME)
	{
		DrawLine(t0.x, t0.y, t1.x, t1.y, GREEN_COLOR);
		DrawLine(t1.x, t1.y, t2.x, t2.y, GREEN_COLOR);
		DrawLine(t2.x, t2.y, t0.x, t0.y, RED_COLOR);
	}
	else if (device->render_state & RENDER_STATE_COLOR)
	{
		int total_height = t2.y - t0.y;
		if (total_height < 0.00001) return;
		for (int y = t0.y; y <= t1.y; ++y)
		{
			int segement_height = t1.y - t0.y + 1;//plus 1 for non zero division
			float alpha = (float)(y - t0.y) / total_height;
			float beta = (float)(y - t0.y) / segement_height;
			Vector3 A = t0 + (t2 - t0)*alpha;
			Vector3 B = t0 + (t1 - t0)*beta;
			if (A.x > B.x) std::swap(A, B);
			for (int j = A.x; j < B.x; ++j)
			{
				DrawPixel(j, y, color);
			}
		}
		for (int y = t1.y; y <= t2.y; ++y)
		{
			int segement_height = t2.y - t1.y + 1;//plus 1 for non zero division
			float alpha = (float)(y - t0.y) / total_height;
			float beta = (float)(y - t1.y) / segement_height;
			Vector3 A = t0 + (t2 - t0)*alpha;
			Vector3 B = t1 + (t2 - t1)*beta;
			if (A.x > B.x) std::swap(A, B);
			for (int j = A.x; j < B.x; ++j)
			{
				DrawPixel(j, y, color);
			}
		}
	}
}

void Rasterizer::DrawSomthing()
{
	if (device->render_state & RENDER_STATE_WIREFRAME)
	{
		for (int i = 0; i < model->nfaces(); i++) {
			std::vector<int> face = model->face(i);
			for (int j = 0; j < 3; j++) {
				Vector3 v0 = model->vert(face[j]);
				Vector3 v1 = model->vert(face[(j + 1) % 3]);
				int x0 = (v0.x + 1.)*device->width / 2.;
				int y0 = (v0.y + 1.)*device->height / 2.;
				int x1 = (v1.x + 1.)*device->width / 2.;
				int y1 = (v1.y + 1.)*device->height / 2.;
				DrawLine(x0, y0, x1, y1, WHITH_COLOR);
			}
		}
	}
	else if (device->render_state & RENDER_STATE_COLOR)
	{
		for (int i = 0; i < model->nfaces(); i++) {
			std::vector<int> face = model->face(i);
			Vector3 screen_points[3];
			Vector3 world_points[3];
			for (int j = 0; j < 3; j++) {
				Vector3 v = model->vert(face[j]);
				screen_points[j] = Vector3((v.x + 1.0f)*device->width / 2.0f,
					(v.y + 1.0f)*device->height / 2.0f,
					1.0f);
				world_points[j] = v;
			}
			Vector3 v1 = world_points[2] - world_points[1];
			Vector3 v2 = world_points[1] - world_points[0];
			Vector3 n = v1.Cross(v2);
			n.Normalize();
			Vector3 light_dir(0, 0, 1);
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
}

void Rasterizer::Update()
{
	device->Clear();
	DrawSomthing();
}
