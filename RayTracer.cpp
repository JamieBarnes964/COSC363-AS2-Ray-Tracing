/*==================================================================================
* COSC 363  Computer Graphics (2022)
* Department of Computer Science and Software Engineering, University of Canterbury.
*
* A basic ray tracer
* See Lab06.pdf   for details.
*===================================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include "Plane.h"
#include "Cuboid.h"
#include "Cylinder.h"
#include "TextureBMP.h"
#include <GL/freeglut.h>
using namespace std;

const float EDIST = 40.0;
const int NUMDIV = 700;
const int MAX_STEPS = 10;
const float XMIN = -10.0;
const float XMAX = 10.0;
const float YMIN = -10.0;
const float YMAX = 10.0;

vector<SceneObject*> sceneObjects;
TextureBMP texture;


//---The most important function in a ray tracer! ----------------------------------
//   Computes the colour value obtained by tracing a ray and finding its
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
	glm::vec3 backgroundCol(1);						//Background colour = (0,0,0)
	glm::vec3 lightPos(10, 40, -3);					//Light's position
	glm::vec3 color(0);
	SceneObject* obj;

    ray.closestPt(sceneObjects);					//Compare the ray with all objects in the scene
    if(ray.index == -1) return backgroundCol;		//no intersection
	obj = sceneObjects[ray.index];					//object on which the closest point of intersection is found

	float airMedium = 1.0;




	if (ray.index == 0)
	{
		//Stripe pattern
		int stripeWidth = 5;
		int iz = abs(ray.hit.z) / stripeWidth;
		int ix = abs(ray.hit.x) / stripeWidth;
		color = glm::vec3(0);

		if (ray.hit.x > 0) {
			if (abs(iz % 2) == abs(ix % 2)) color = glm::vec3(1, 1, 1);
			else color = glm::vec3(0.1, 0.1, 0.1);
		}
		if (ray.hit.x <= 0) {
			if (abs(iz % 2) != abs((ix) % 2)) color = glm::vec3(1, 1, 1);
			else color = glm::vec3(0.1, 0.1, 0.1);
		}

		obj->setColor(color);

		float texcoords = (ray.hit.x + 15) / 20;
		float texcoordt = 1 - (ray.hit.z + 90) / 30;
		if(texcoords > 0 && texcoords < 1 && texcoordt > 0 && texcoordt < 1)
		{
			color=texture.getColorAt(texcoords, texcoordt);
			// obj->setColor(color);
		}
	}

	// color = obj->getColor();						//Object's colour
	color = obj->lighting(lightPos, -ray.dir, ray.hit);						//Object's colour


	glm::vec3 lightVec = lightPos - ray.hit;
	Ray shadowRay(ray.hit, lightVec);

	shadowRay.closestPt(sceneObjects);
	int lightDist = glm::length(lightVec);

	if (shadowRay.index > -1 && shadowRay.dist < lightDist) {
		SceneObject* shadowObj = sceneObjects[shadowRay.index];
		if (shadowObj->isTransparent() || shadowObj->isRefractive()) {
			Ray transmittedShadowRay(shadowRay.hit, lightVec);
			// color = color * shadowObj->getTransparencyCoeff() * shadowObj->lighting(lightPos, shadowRay.dir, shadowRay.hit);
			float transCoef = shadowObj->getTransparencyCoeff() * 0.8 + 0.2;
			color = color * transCoef * shadowObj->getColor();
		} else {
			color = 0.2f * obj->getColor(); //0.2 = ambient scale factor
		}
	}

	if (obj->isReflective() && step < MAX_STEPS)
	{
		float rho = obj->getReflectionCoeff();
		glm::vec3 normalVec = obj->normal(ray.hit);
		glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVec);
		Ray reflectedRay(ray.hit, reflectedDir);
		glm::vec3 reflectedColor = trace(reflectedRay, step + 1);
		color = color + (rho * reflectedColor);
	}

	if (obj->isTransparent() && step < MAX_STEPS)
	{
		float tco = obj->getTransparencyCoeff();
		Ray transmittedRay(ray.hit, ray.dir);
		glm::vec3 transmittedColor = trace(transmittedRay, step + 1);
		color = (color + tco * transmittedColor);
	}


	if (obj->isRefractive() && step << MAX_STEPS)
	{
		glm::vec3 normalVec = obj->normal(ray.hit);
		glm::vec3 refractedVec(0);
		if (glm::dot(ray.dir, normalVec) > 0) {
			float eta = obj->getRefractiveIndex() / airMedium;
			refractedVec = glm::refract(ray.dir, -normalVec, eta);
		} else {
			float eta = airMedium / obj->getRefractiveIndex();
			refractedVec = glm::refract(ray.dir, normalVec, eta);
		}
		Ray refractedRay(ray.hit, refractedVec);
		glm::vec3 transmittedColor = trace(refractedRay, step + 1);
		color = color + transmittedColor * obj->getRefractionCoeff();
	}
	// color = color + glm::vec3(0.003) * ray.dist; // FOG??

	return color;
}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
	float xp, yp;  //grid point
	float cellX = (XMAX - XMIN) / NUMDIV;  //cell width
	float cellY = (YMAX - YMIN) / NUMDIV;  //cell height
	glm::vec3 eye(0., 0., 0.);

	glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glBegin(GL_QUADS);  //Each cell is a tiny quad.

	for (int i = 0; i < NUMDIV; i++)	//Scan every cell of the image plane
	{
		xp = XMIN + i * cellX;
		for (int j = 0; j < NUMDIV; j++)
		{
			yp = YMIN + j * cellY;

			glm::vec3 dir(xp + 0.5 * cellX, yp + 0.5 * cellY, -EDIST);	//direction of the primary ray

			Ray ray = Ray(eye, dir);

			glm::vec3 col = trace(ray, 1); //Trace the primary ray and get the colour value
			glColor3f(col.r, col.g, col.b);
			glVertex2f(xp, yp);				//Draw each cell with its color value
			glVertex2f(xp + cellX, yp);
			glVertex2f(xp + cellX, yp + cellY);
			glVertex2f(xp, yp + cellY);
		}
	}

    glEnd();
    glFlush();
}



//---This function initializes the scene -------------------------------------------
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL 2D orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);

    glClearColor(0, 0, 0, 1);

	texture = TextureBMP("Butterfly.bmp");

	Plane *plane = new Plane (	glm::vec3(-20., -15, -40), //Point A
								glm::vec3(20., -15, -40), //Point B
								glm::vec3(20., -15, -200), //Point C
								glm::vec3(-20., -15, -200)); //Point D
	plane->setColor(glm::vec3(0.8,0.8,0));
	// plane->setReflectivity(true, 0.2);
	plane->setSpecularity(false);
	sceneObjects.push_back(plane);


	Plane *banner = new Plane (	glm::vec3(-20., -5, -90), //Point A
								glm::vec3(-20., 5, -90), //Point B
								glm::vec3(20., 5, -90), //Point C
								glm::vec3(20., -5, -90)); //Point D
	banner->setColor(glm::vec3(0.8,0.8,0));
	// plane->setReflectivity(true, 0.2);
	banner->setSpecularity(false);
	sceneObjects.push_back(banner);

	Sphere *sphere1 = new Sphere(glm::vec3(-5.0, -10.0, -70.0), 5.0);
	sphere1->setColor(glm::vec3(0.1, 0.1, 1.0));   //Set colour to blue
	// sphere1->setSpecularity(false);
	// sphere1->setReflectivity(true, 0.4);
	sphere1->setTransparency(true, 1.);
	sceneObjects.push_back(sphere1);		 //Add sphere to scene objects

	Sphere *sphere2 = new Sphere(glm::vec3(-5.0, -1.0, -70.0), 4.0);
	sphere2->setColor(glm::vec3(1, 1, 1));   //Set colour to blue
	// sphere2->setShininess(5);
	// sphere2->setTransparency(true, 0.8);
	sphere2->setRefractivity(true, 1.0, 1.5);
	sceneObjects.push_back(sphere2);		 //Add sphere to scene objects

	Sphere *sphere3 = new Sphere(glm::vec3(-5.0, 6.0, -70.0), 3.0);
	sphere3->setColor(glm::vec3(1, 0, 0));   //Set colour to blue
	sphere3->setReflectivity(true, 0.6);
	sceneObjects.push_back(sphere3);		 //Add sphere to scene objects

	// Sphere *sphere4 = new Sphere(glm::vec3(5.0, -10.0, -60.0), 5.0);
	// sphere4->setColor(glm::vec3(0.1, 0.4, 0.1));   //Set colour to blue
	// sphere4->setReflectivity(false, 0);
	// sphere4->setTransparency(true, 0.8);
	// sceneObjects.push_back(sphere4);		 //Add sphere to scene objects

	Cuboid *cuboid1 = new Cuboid(glm::vec3(8, -10, -70.0), 9.9, 5, 5);
	cuboid1->setColor(glm::vec3(1, 0, 1));
	cuboid1->setReflectivity(false, 0.2);
	cuboid1->setTransparency(true, 0.9);
	// cuboid1->setSpecularity(false);
	sceneObjects.push_back(cuboid1);

	// Cuboid *cuboid2 = new Cuboid(glm::vec3(-4, -10, -90.0), 5, 5, 5);
	// cuboid2->setColor(glm::vec3(1, 0, 1));
	// // cuboid2->setReflectivity(false, 0.2);
	// // cuboid2->setTransparency(true, 0.9);
	// // cuboid1->setSpecularity(false);
	// sceneObjects.push_back(cuboid2);

Cylinder *cylinder1 = new Cylinder(glm::vec3(8, -5, -70), 2.5, 8);
	cylinder1->setColor(glm::vec3(1,1,0));
	sceneObjects.push_back(cylinder1);

}


int main(int argc, char *argv[]) {
	try {

		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
		glutInitWindowSize(1000, 1000);
		glutInitWindowPosition(0, 0);
		glutCreateWindow("Raytracing");

		glutDisplayFunc(display);
		initialize();

		glutMainLoop();
		return 0;
	}
	catch (std::exception& e) {
		cerr << "error: " << e.what() << "\n";
	}
}
