struct MeshVertex
{
	Vector3	position;
	Vector3    normal;
};



struct MeshQuad
{
	// pointers to vertices of each quad
	MeshVertex *vertices[4];	
};

class QuadMesh
{
private:
	
	int maxMeshSize;
	int minMeshSize;
	float meshDim;

	int numVertices;
	MeshVertex *vertices;

	int numQuads;
	MeshQuad *quads;

	int numFacesDrawn;
	
	GLfloat mat_ambient[4];
    GLfloat mat_specular[4];
    GLfloat mat_diffuse[4];
	GLfloat mat_shininess[1];

	
private:
	bool CreateMemory();
	void FreeMemory();

public:

	typedef std::pair<int, int> MaxMeshDim;

	QuadMesh(int maxMeshSize = 40, float meshDim = 1.0f);
	
	~QuadMesh()
	{
		FreeMemory();
	}

	MaxMeshDim GetMaxMeshDimentions()
	{
		return MaxMeshDim(minMeshSize, maxMeshSize);
	}
	
	bool InitMesh(int meshSize, Vector3 origin, double meshLength, double meshWidth,Vector3 dir1, Vector3 dir2);
	void DrawMesh(int meshSize);
	void UpdateMesh();
	void QuadMesh::SetMaterial(Vector3 ambient, Vector3 diffuse, Vector3 specular, double shininess);
	void ComputeNormals();
	
	
};

