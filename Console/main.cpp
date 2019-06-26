#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <map>

struct OMDHeader
{
	char fileFormat;	//'t' for text, 'b' for binary
	char extension[3];	//"OMD"
	unsigned modelType;
	unsigned vertexCount;
	unsigned indexCount;
	unsigned groupCount;
	unsigned materialCount;
	unsigned boundingVolumePrimitive;
	unsigned hitboxTriangleCount;
	unsigned boneCount;
	unsigned animationCount;

	void Print()
	{
		std::wcout << fileFormat;
		std::wcout << extension[0];
		std::wcout << extension[1];
		std::wcout << extension[2];
		std::wcout << '\n';
		std::wcout << L"Model type: " << modelType << '\n';
		std::wcout << L"Vertex count: " << vertexCount << '\n';
		std::wcout << L"Index count: " << indexCount << '\n';
		std::wcout << L"Group count: " << groupCount << '\n';
		std::wcout << L"Material count: " << materialCount << '\n';
		std::wcout << L"Bounding volume primitive: " << boundingVolumePrimitive << '\n';
		std::wcout << L"Hitbox triangle count: " << hitboxTriangleCount << '\n';
		std::wcout << L"Bone count: " << boneCount << '\n';
		std::wcout << L"Animation count: " << animationCount << '\n';
	}
};

OMDHeader header;
bool ScanText(std::wistream& instr, const wchar_t text[])
{
	std::wstringstream ss(text);
	std::wstring b1, b2;
	do {
		if (instr.eof())
			return false;
		ss >> b1;
		instr >> b2;
		if (b1 != b2)
			return false;
	} while (!ss.eof());
	return true;
}

void ReadHeaderText(std::wifstream& infile)
{
	std::wstring buffer;
	infile >> buffer;
	header.fileFormat = (char)buffer[0];
	header.extension[0] = (char)buffer[1];
	header.extension[1] = (char)buffer[2];
	header.extension[2] = (char)buffer[3];
	if (ScanText(infile, L"Model type:"))
		infile >> header.modelType;
	if (ScanText(infile, L"Vertex count:"))
		infile >> header.vertexCount;
	if (ScanText(infile, L"Index count:"))
		infile >> header.indexCount;
	if (ScanText(infile, L"Group count:"))
		infile >> header.groupCount;
	if (ScanText(infile, L"Material count:"))
		infile >> header.materialCount;
	if (ScanText(infile, L"Bounding volume primitive:"))
		infile >> header.boundingVolumePrimitive;
	if (ScanText(infile, L"Hitbox triangle count:"))
		infile >> header.hitboxTriangleCount;
	if (ScanText(infile, L"Bone count:"))
		infile >> header.boneCount;
	if (ScanText(infile, L"Animation count:"))
		infile >> header.animationCount;
	header.Print();
}
void ReadVerticesText(std::wifstream& infile)
{
	if (ScanText(infile, L"Vertices:"))
	{
		std::wcout << L"\nVertices:\n";
		unsigned vertexSize = 8;
		std::vector<float> vertices(vertexSize * header.vertexCount);
		for (float& f : vertices)
			infile >> f;
		for (size_t i = 0; i < header.vertexCount; i++)
		{
			for (size_t j = 0; j < vertexSize; j++)
				std::wcout << vertices[i * 8 + j] << ' ';
			std::wcout << '\n';
		}
	}
}
void ReadIndicesText(std::wifstream& infile)
{
	if (ScanText(infile, L"Indices:"))
	{
		std::wcout << L"\nIndices:\n";
		std::vector<unsigned> indices(header.indexCount);
		for (unsigned& i : indices)
			infile >> i;
		for (size_t i = 0; i < header.indexCount; i += 3)
			std::wcout << indices[i + 0] << ' ' << indices[i + 1] << ' ' << indices[i + 2] << ' ' << '\n';
	}
}
void ReadGroupsText(std::wifstream& infile)
{
	struct Group
	{
		unsigned startIndex;
		unsigned indexCount;
		unsigned materialIndex;
	};
	if (ScanText(infile, L"Groups:"))
	{
		std::vector<Group> groups(header.groupCount);
		for (Group& g : groups)
		{
			if (ScanText(infile, L"New group"))
			{
				if (ScanText(infile, L"Start index:"))
					infile >> g.startIndex;
				if (ScanText(infile, L"Index count:"))
					infile >> g.indexCount;
				if (ScanText(infile, L"Material index:"))
					infile >> g.materialIndex;
			}
		}
		std::wcout << L"\nGroups:\n";
		for (Group& g : groups)
		{
			std::wcout << L"New group\n";
			std::wcout << L"\tStart index: " << g.startIndex << '\n';
			std::wcout << L"\tIndex count: " << g.indexCount << '\n';
			std::wcout << L"\tMaterial index: " << g.materialIndex << '\n';
		}
	}
}
void ReadMaterialsText(std::wifstream& infile)
{
	struct Material
	{
		std::wstring textureName;
		std::wstring normalmapName;
	};
	if (ScanText(infile, L"Materials:"))
	{
		std::vector<Material> materials(header.materialCount);
		wchar_t buffer[256];
		for (Material& m : materials)
		{
			if (ScanText(infile, L"New material"))
			{
				if (ScanText(infile, L"Texture name:"))
				{
					infile.getline(buffer, 256);
					size_t offset;
					for (offset = 0; isspace(buffer[offset]); offset++);
					m.textureName = buffer + offset;
				}
				if (ScanText(infile, L"Normalmap name:"))
				{
					infile.getline(buffer, 256);
					size_t offset;
					for (offset = 0; isspace(buffer[offset]); offset++);
					m.normalmapName = buffer + offset;
				}
			}
		}
		std::wcout << L"\nMaterials:\n";
		for (Material& m : materials)
		{
			std::wcout << L"New material\n";
			std::wcout << L"\tTexture name: " << m.textureName << '\n';
			std::wcout << L"\tNormalmap name: " << m.normalmapName << '\n';
		}
	}
}
void ReadHitboxText(std::wifstream& infile)
{
	struct Triangle
	{
		float vertices[3][3];
		float normal[3];
		float distance;
	};
	std::vector<Triangle> hitbox;
	float x = 0, y = 0, z = 0;
	float w = 0, h = 0, d = 0, r = 0;
	if (ScanText(infile, L"Bounding volume:"))
	{
		if (header.boundingVolumePrimitive == 1)
		{
			infile >> x >> y >> z;
			infile >> w >> h >> d;
		}
		else if (header.boundingVolumePrimitive == 2)
		{
			infile >> x >> y >> z;
			infile >> r;
		}
	}
	if (ScanText(infile, L"Hitbox:"))
	{
		hitbox.resize(header.hitboxTriangleCount);
		for (auto& t : hitbox)
		{
			infile >> t.vertices[0][0];
			infile >> t.vertices[0][1];
			infile >> t.vertices[0][2];
			infile >> t.vertices[1][0];
			infile >> t.vertices[1][1];
			infile >> t.vertices[1][2];
			infile >> t.vertices[2][0];
			infile >> t.vertices[2][1];
			infile >> t.vertices[2][2];
			infile >> t.normal[0];
			infile >> t.normal[1];
			infile >> t.normal[2];
			infile >> t.distance;
		}
	}
	std::wcout << "\nBounding volume:\n";
	if (header.boundingVolumePrimitive == 1)
		std::wcout << x << ' ' << y << ' ' << z << ' ' << w << ' ' << h << ' ' << d << '\n';
	if (header.boundingVolumePrimitive == 2)
		std::wcout << x << ' ' << y << ' ' << z << ' ' << r << '\n';

	std::wcout << "\nHitbox:\n";
	for (auto& t : hitbox)
	{
		std::wcout << t.vertices[0][0] << ' ';
		std::wcout << t.vertices[0][1] << ' ';
		std::wcout << t.vertices[0][2] << ' ';
		std::wcout << t.vertices[1][0] << ' ';
		std::wcout << t.vertices[1][1] << ' ';
		std::wcout << t.vertices[1][2] << ' ';
		std::wcout << t.vertices[2][0] << ' ';
		std::wcout << t.vertices[2][1] << ' ';
		std::wcout << t.vertices[2][2] << ' ';
		std::wcout << t.normal[0] << ' ';
		std::wcout << t.normal[1] << ' ';
		std::wcout << t.normal[2] << ' ';
		std::wcout << t.distance << ' ';
		std::wcout << '\n';
	}
}
void ReadBonesText(std::wifstream& infile)
{
	if (ScanText(infile, L"Bones:"))
	{

	}
	std::wcout << "\nBones:\n";
}
void ReadAnimationsText(std::wifstream& infile)
{
	if (ScanText(infile, L"Animations:"))
	{

	}
	std::wcout << "\nAnimations:\n";
}

void LoadOMDText(const wchar_t* filename)
{
	std::wifstream infile(filename);
	ReadHeaderText(infile);
	ReadVerticesText(infile);
	ReadIndicesText(infile);
	ReadGroupsText(infile);
	ReadMaterialsText(infile);
	ReadHitboxText(infile);
	ReadBonesText(infile);
	ReadAnimationsText(infile);
}

int OMDLoadingTest()
{
	LoadOMDText(L"../Octdoc/Media/cube.omd");
	std::cin.get();
	return 0;
}

float Random()
{
	return float(2 * rand() - RAND_MAX) / float(RAND_MAX);
}
struct float3
{
	float x, y, z;
	float3() :x(0.0f), y(0.0f), z(0.0f) {}
	float3(float x, float y, float z) :x(x), y(y), z(z) {}
	static float3 RandomVector() { return float3(Random(), Random(), Random()); }
	float LengthSquare() { return x * x + y * y + z * z; }
	float Dot(float3 v) { return x * v.x + y * v.y + z * v.z; }
	float3 operator-(float3 v) { return float3(x - v.x, y - v.y, z - v.z); }
};
float LPD1(float3 linePoint, float3 lineDirection, float3 point)
{
	//return lineDirection.Cross(point - linePoint).LengthSquare() / lineDirection.LengthSquare();
	return (point - linePoint).LengthSquare() - (lineDirection.Dot(point - linePoint)) * (lineDirection.Dot(point - linePoint)) / lineDirection.LengthSquare();
}
float LPD2(float3 linePoint, float3 lineDirection, float3 point)
{
	//return lineDirection.Cross(point - linePoint).LengthSquare() / lineDirection.LengthSquare();
	return (point - linePoint).LengthSquare() - (lineDirection.Dot(point - linePoint)) * (lineDirection.Dot(point - linePoint)) / lineDirection.LengthSquare();
}

int LinePointDistanceTest()
{
	for (int i = 0; i < 213; i++)
	{
		float3 p = float3::RandomVector();
		float3 v = float3::RandomVector();
		float3 q = float3::RandomVector();
		std::cout << LPD1(p, v, q) - LPD2(p, v, q) << std::endl;
	}
	std::cin.get();
	return 0;
}

class Class
{
	static int c;
public:
	Class()
	{
		std::cout << c++ << std::endl;
	}
};
int Class::c = 0;
int main()
{
	std::vector<Class> cs;
	cs.resize(5);
	std::cin.get();
	return OMDLoadingTest();
}