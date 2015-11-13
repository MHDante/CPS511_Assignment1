typedef struct BoundingBox
{
  VECTOR3D min;
  VECTOR3D max;
} BBox;


// Vertex positions of a standard size cube (width 2), centered at the origin
// of its own Model Coordinate System

typedef class CubeMesh
{
  const GLfloat vertices[8][3] = {
    { -1.0, -1.0,-1.0 },
    { 1.0, -1.0,-1.0 },
    { 1.0,  1.0,-1.0 },
    { -1.0,  1.0,-1.0 },
    { -1.0, -1.0, 1.0 },
    { 1.0, -1.0, 1.0 },
    { 1.0,  1.0, 1.0 },
    { -1.0,  1.0, 1.0 } };

  const GLubyte quads[4*6] = {
    0, 3, 2, 1,	// back face
    2, 3, 7, 6,	// top face
    0, 4, 7, 3,  // left face
    1, 2, 6, 5,  // right face
    4, 5, 6, 7,  // front face
    0, 1, 5, 4 };	// bottom face

  const GLfloat quadNormals[6][3] = {
    { 0.0, 0.0, -1.0 },	// Back Face 
    { 0.0, 1.0,  0.0 },	// Top Face
    { -1.0, 0.0,  0.0 },	// Left Face
    { 1.0, 0.0,  0.0 },	// Right Face
    { 0.0, 0.0,  1.0 },	// Front Face
    { 0.0,-1.0,  0.0 } };	// Bottom Face
public:
	VECTOR3D center;
	VECTOR3D dim;
	float tx, ty, tz;			// Translatation Deltas
	float sfx, sfy, sfz;		// Scale Factors
	float angle;				// Angle around y-axis of cube coordinate system
	bool selected;

	// Material properties for drawing
	float mat_ambient[4];
  float mat_specular[4];
  float mat_diffuse[4];
	float mat_shininess[1];

	// Material properties if selected
	float highlightMat_ambient[4];
  float highlightMat_specular[4];
  float highlightMat_diffuse[4];
	float highlightMat_shininess[1];
public:
  CubeMesh();
  BBox* getBBox();
  void drawCube();


} CubeMesh;

	