#ifndef LIYT_H
#define LIYT_H

// all of these functions have 0 safety features
// additionally they permenantly modify the arrays passed to them

void liyt_transVerts(float* verts, int tricount, float x, float y, float z) {
	for (int i = 0; i < tricount * 3; i++) {
		verts[(i*3)  ] += x;
		verts[(i*3)+1] += y;
		verts[(i*3)+2] += z;
	}
}

void liyt_scaleVerts(float* verts, int tricount, float x, float y, float z) {
	for (int i = 0; i < tricount * 3; i++) {
		verts[(i*3)  ] *= x;
		verts[(i*3)+1] *= y;
		verts[(i*3)+2] *= z;
	}
}

void liyt_rotVertsZyx(float* verts, int tricount, float x, float y, float z) {

	float sinrx = sinf(x); float cosrx = cosf(x);
	float sinry = sinf(y); float cosry = cosf(y);
	float sinrz = sinf(z); float cosrz = cosf(z);

	float *v = verts;

	int count = tricount * 3;

	for (int i = 0; i < count; i++, v += 3) {
		float nx, ny, nz;
		float vx = v[0], vy = v[1], vz = v[2];

		nx = (vx * cosrz) - (vy * sinrz);
		ny = (vx * sinrz) + (vy * cosrz);
		nz = (vz * cosry) - (nx * sinry);
		v[0] = (vz * sinry) + (nx * cosry);
		v[1] = (ny * cosrx) - (nz * sinrx);
		v[2] = (ny * sinrx) + (nz * cosrx);
	}
}

void liyt_rotVertsMtxZyx(float* verts, int tricount, float x, float y, float z) {
	Mtx rotX, rotY, rotZ, rotFinal;
	guMtxRotAxisRad(rotX, &xAxis, x);
	guMtxRotAxisRad(rotY, &yAxis, y);
	guMtxRotAxisRad(rotZ, &zAxis, z);
	guMtxConcat(rotX, rotY, rotFinal);
	guMtxConcat(rotZ, rotFinal, rotFinal);

	float *v = verts;

	int count = tricount * 3;

	for (int i = 0; i < count; i++, v += 3) {
		guVector vv; vv.x = v[0]; vv.y = v[1]; vv.z = v[2];

		guVecMultiply(rotFinal, &vv, &vv);
		v[0] = vv.x;
		v[1] = vv.y;
		v[2] = vv.z;
	}	
}

void liyt_genMtxPosRot(Mtx out, float xpos, float ypos, float zpos, float xrot, float yrot, float zrot) {
	Mtx xrotmtx, yrotmtx, zrotmtx;
	guMtxRotAxisRad(xrotmtx, &xAxis, xrot);
	guMtxRotAxisRad(yrotmtx, &yAxis, yrot);
	guMtxRotAxisRad(zrotmtx, &zAxis, zrot);
	guMtxConcat(xrotmtx, yrotmtx, yrotmtx);
	guMtxConcat(yrotmtx, zrotmtx, zrotmtx);
	guMtxTransApply(zrotmtx, out, xpos, ypos, zpos);
	return;
}

void liyt_genMtxPosRotZyx(Mtx out, float xpos, float ypos, float zpos, float xrot, float yrot, float zrot) {
	Mtx xrotmtx, yrotmtx, zrotmtx;
	guMtxRotAxisRad(xrotmtx, &xAxis, xrot);
	guMtxRotAxisRad(yrotmtx, &yAxis, yrot);
	guMtxRotAxisRad(zrotmtx, &zAxis, zrot);
	guMtxConcat(zrotmtx, yrotmtx, yrotmtx);
	guMtxConcat(yrotmtx, xrotmtx, xrotmtx);
	guMtxTransApply(xrotmtx, out, xpos, ypos, zpos);
	return;
}

#endif