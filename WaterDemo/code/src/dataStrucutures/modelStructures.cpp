struct Point {
	float x;
	float y;
	float z;
	float w;
};

struct UV {
	float u;
	float v;
};

struct Normal {
	float x;
	float y;
	float z;
};

struct Face {
	Point* points;
	UV* uvs;
	Normal* normals;
};