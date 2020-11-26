#include "Rasterizer.h"

Vertex mesh[8] = {
	{Vector4(-1.0f,-1.0f,1.0f,1.0f),Color(1.0f,0.2f,0.2f)},
	{Vector4(1.0f,-1.0f,1.0f,1.0f),Color(0.2f,1.0f,0.2f)},
	{Vector4(1.0f, 1.0f,1.0f,1.0f),Color(0.2f,0.2f,1.0f)},
	{Vector4(-1.0f, 1.0f,1.0f,1.0f),Color(1.0f,0.2f,1.0f)},
	{Vector4(-1.0f,-1.0f,-1.0f,1.0f),Color(1.0f,1.0f,0.2f)},
	{Vector4(1.0f,-1.0f,-1.0f,1.0f),Color(0.2f,1.0f,1.0f)},
	{Vector4(1.0f, 1.0f,-1.0f,1.0f),Color(1.0f,0.3f,0.3f)},
	{Vector4(-1.0f, 1.0f,-1.0f,1.0f),Color(0.2f,1.0f,0.3f)},
};

//Vertex mesh[8] = {
//   {Vector4(-1.0f,-1.0f,1.0f,1.0f),WHITH_COLOR},
//   {Vector4(1.0f,-1.0f,1.0f,1.0f),WHITH_COLOR},
//   {Vector4(1.0f, 1.0f,1.0f,1.0f),WHITH_COLOR},
//   {Vector4(-1.0f, 1.0f,1.0f,1.0f),WHITH_COLOR},
//   {Vector4(-1.0f,-1.0f,-1.0f,1.0f),WHITH_COLOR},
//   {Vector4(1.0f,-1.0f,-1.0f,1.0f),WHITH_COLOR},
//   {Vector4(1.0f, 1.0f,-1.0f,1.0f),WHITH_COLOR},
//   {Vector4(-1.0f, 1.0f,-1.0f,1.0f),WHITH_COLOR},
//};

static Vector4 MatrixVectorMul(const Matrix4& m, const Vector4& v)
{
	return Vector4(	
		m.m[0][0] * v.x	+ m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w,
		m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w,
		m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w,
		m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w);
}

static Vector4 MatrixVectorMul(const Vector4& v, const Matrix4& m)
{
	return Vector4(
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2],
		v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3]);
}

Rasterizer::Rasterizer()
{
	SetBoxNormal();
};

Rasterizer::~Rasterizer()
{

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
	float x = v.x * rhw;
	float y = v.y * rhw;
	float z = v.z * rhw;
	float w = 1.0f;
	z = (z + 1.0f) * 0.5f;
	return Vector4((x + 1.0f)*device->GetWidth() * 0.5f,
		(1.0f - y)*device->GetHeight() * 0.5f,
		z, w);
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
	
	
	/*mesh[0].SetVertexNormal(Vector4(-0.57, -0.57, -0.57,1.0f));
	mesh[1].SetVertexNormal(Vector4(-0.57, 0.57, -0.57, 1.0f));
	mesh[2].SetVertexNormal(Vector4(0.57, -0.57, -0.57, 1.0f));
	mesh[3].SetVertexNormal(Vector4(0.57, 0.57, -0.57, 1.0f));

	mesh[4].SetVertexNormal(Vector4(-0.57, -0.57, 0.57, 1.0f));
	mesh[5].SetVertexNormal(Vector4(-0.57, 0.57, 0.57, 1.0f));
	mesh[6].SetVertexNormal(Vector4(0.57, -0.57, 0.57, 1.0f));
	mesh[7].SetVertexNormal(Vector4(0.57, 0.57, 0.57, 1.0f));*/
	
}

void Rasterizer::CalculateVertexColor(Vertex& v)
{
	Vector4 n = v.GetVertexNormal();
	Vector4 light_driection = light->GetDirection();
	float light_ratio = n.Dot(light_driection);
	Color color = v.GetVertexColor();
	color = color * light_ratio;
	v.SetVertexColor(color);
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
		Color c0 = t.GetV0().GetVertexColor();
		Color c1 = t.GetV1().GetVertexColor();
		Color c2 = t.GetV2().GetVertexColor();

		if (t0.y > t1.y) { std::swap(t0, t1); std::swap(c0, c1); }
		if (t0.y > t2.y) { std::swap(t0, t2); std::swap(c0, c2); }
		if (t1.y > t2.y) { std::swap(t1, t2); std::swap(c1, c2); }

		float total_height = t2.y - t0.y + EPSILON;
		// plus 0.5 for rounding
		for (int y = (int)(t0.y + 0.5); y <= (int)(t1.y + 0.5); y++)
		{
			// Because of up set down Y. Add a checkpoint here.
			if (y >= device->GetHeight() || y < 0) break;
			// This check  additional pixel 
			//if(std::abs((float)y - t0.y)<0.5f) continue;
			float segement_height = t1.y - t0.y + EPSILON;
			//if(segement_height < 1.0f) continue;
			float alpha = (float)(y - t0.y) / total_height;
			float beta = (float)(y - t0.y) / segement_height;
			Vector4 A = Vector4::Lerp(t0, t2, alpha);
			Vector4 B = Vector4::Lerp(t0, t1, beta);
			Color C = Color::Lerp(c0, c2, alpha);
			Color D = Color::Lerp(c0, c1, beta);
			//draw line from left to right
			if (A.x > B.x) { std::swap(A, B); std::swap(C, D); }

			//zbuffer caculation
			float scanline_depth = B.z - A.z;
			float scanline_width = B.x - A.x;
			float depth_ratio = scanline_depth / scanline_width;
			float *z_line_buffer = device->GetZBuffer()[y];

			//color caculation
			Color color_diff = D - C;
			Color color_ratio = color_diff / scanline_width;

			for (int j = (int)(A.x + 0.5); j < (int)(B.x + 0.5); ++j)
			{
				float step = (float)j - A.x + 0.5;
				if (j >= device->GetWidth() || j < 0) break;
				float z = depth_ratio * step + A.z;
				if (z_line_buffer[j] <= z)
				{
					Color c = (color_ratio * step + C);
					z_line_buffer[j] = z;
					DrawPixel(j, y, c);
				}
			}
		}
		// plus 0.5 for rounding
		for (int y = (int)(t1.y + 0.5); y <= (int)(t2.y + 0.5); y++)
		{
			// Because of up set down Y. Add a checkpoint here.
			if (y >= device->GetHeight() || y < 0) break;
			// This check  additional pixel 
			//if (std::abs((float)y - t1.y) < 0.5f) continue;
			float segement_height = t2.y - t1.y + EPSILON;
			//if (segement_height < 1.0f) continue;
			float alpha = (float)(y - t0.y) / total_height;
			float beta = (float)(y - t1.y) / segement_height;
			Vector4 A = Vector4::Lerp(t0, t2, alpha);
			Vector4 B = Vector4::Lerp(t1, t2, beta);
			Color C = Color::Lerp(c0, c2, alpha);
			Color D = Color::Lerp(c1, c2, beta);
			//draw line from left to right
			if (A.x > B.x) { std::swap(A, B); std::swap(C, D); }

			//for zbuffer caculation
			float scanline_depth = B.z - A.z; 
			float scanline_width = B.x - A.x;
			float depth_ratio = scanline_depth / scanline_width;
			float *z_line_buffer = device->GetZBuffer()[y];

			//color caculation
			Color color_diff = D - C;
			Color color_ratio = color_diff / scanline_width;

			for (int j = (int)(A.x + 0.5); j < (int)(B.x + 0.5); ++j)
			{
				float step = (float)j - A.x + 0.5;
				if (j >= device->GetWidth() || j < 0) break;
				float z = depth_ratio * step + A.z;
				if (z_line_buffer[(int)j] <= z)
				{
					Color c = (color_ratio * step + C);
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
	DrawPlane(points[0], points[1], points[2], points[3]); // front
	DrawPlane(points[7], points[6], points[5], points[4]); // back
	DrawPlane(points[0], points[4], points[5], points[1]); // down
	DrawPlane(points[1], points[5], points[6], points[2]); // right
	DrawPlane(points[2], points[6], points[7], points[3]); // up
	DrawPlane(points[3], points[7], points[4], points[0]); // left
}

void Rasterizer::DrawSomthing()
{	
	if (device->GetRenderState() & RENDER_STATE_MODEL)
	{
		for (int i = 0; i < model->Nfaces(); i++) {
			std::vector<int> face = model->GetFace(i);
			Vertex screen_points[3];
			Vector4 world_points[3];
			for (int j = 0; j < 3; j++) {
				Vector4 v = Vector4(model->GetVert(face[j]), 1.0f);
				Vector4 n = Vector4(model->GetNorm(face[j]), 0.0f);
				world_points[j] = TransformApply(v, camera->GetTranformation());
				screen_points[j].SetVertexPosition(TransformHomogenize(world_points[j]));
				screen_points[j].SetVertexColor(WHITH_COLOR);
				n = MatrixVectorMul(n, camera->GetModelMatrix());
				screen_points[j].SetVertexNormal(n);
				CalculateVertexColor(screen_points[j]);
			}
			Vector4 v1 = world_points[2] - world_points[1];
			Vector4 v2 = world_points[1] - world_points[0];
			Vector4 n = v1.Cross(v2);
			n.Normalize();
			Triangle t(screen_points[0], screen_points[1], screen_points[2]);
			DrawTriangle(t);

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
			Vector4 n = mesh[i].GetVertexNormal();
			
			n = MatrixVectorMul(n, camera->GetModelMatrix());

			vert[i].SetVertexNormal(n);
			vert[i].SetVertexColor(mesh[i].GetVertexColor());
			CalculateVertexColor(vert[i]);
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
