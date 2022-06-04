#include "Cuboid.h"
#include <math.h>
#include "Ray.h"
#include <vector>
#include "SceneObject.h"

#include <string>
#include <iostream>
using namespace std;



Cuboid::Cuboid (glm::vec3 c, float h, float w, float d)
{
	center = c;
	height = h;
	width = w;
	depth = d;

	front = new Plane (
						center + glm::vec3(0.5 * width, 	0.5 * height, 	0.5 * depth),
						center + glm::vec3(-0.5 * width,	0.5 * height, 	0.5 * depth),
						center + glm::vec3(-0.5 * width, 	-0.5 * height, 	0.5 * depth),
						center + glm::vec3(0.5 * width, 	-0.5 * height, 	0.5 * depth)
					);

	back = new Plane (
						center + glm::vec3(-0.5 * width, 	-0.5 * height, 	-0.5 * depth),
						center + glm::vec3(-0.5 * width, 	0.5 * height, 	-0.5 * depth),
						center + glm::vec3(0.5 * width, 	0.5 * height, 	-0.5 * depth),
						center + glm::vec3(0.5 * width, 	-0.5 * height, 	-0.5 * depth)
					);

    left = new Plane (
						center + glm::vec3(0.5 * width, 	-0.5 * height, 	-0.5 * depth),
						center + glm::vec3(0.5 * width, 	0.5 * height, 	-0.5 * depth),
						center + glm::vec3(0.5 * width, 	0.5 * height, 	0.5 * depth),
						center + glm::vec3(0.5 * width, 	-0.5 * height, 	0.5 * depth)
					);

    right = new Plane (
						center + glm::vec3(-0.5 * width, 	0.5 * height, 	0.5 * depth),
						center + glm::vec3(-0.5 * width, 	0.5 * height, 	-0.5 * depth),
						center + glm::vec3(-0.5 * width, 	-0.5 * height, 	-0.5 * depth),
						center + glm::vec3(-0.5 * width, 	-0.5 * height, 	0.5 * depth)
					);

    top = new Plane (
						center + glm::vec3(-0.5 * width, 	0.5 * height, 	-0.5 * depth),
						center + glm::vec3(-0.5 * width, 	0.5 * height, 	0.5 * depth),
						center + glm::vec3(0.5 * width, 	0.5 * height, 	0.5 * depth),
						center + glm::vec3(0.5 * width, 	0.5 * height, 	-0.5 * depth)
					);

    bottom = new Plane (
						center + glm::vec3(-0.5 * width, 	-0.5 * height, 	-0.5 * depth),
						center + glm::vec3(-0.5 * width, 	-0.5 * height, 	0.5 * depth),
						center + glm::vec3(0.5 * width, 	-0.5 * height, 	0.5 * depth),
						center + glm::vec3(0.5 * width, 	-0.5 * height, 	-0.5 * depth)
					);
}

float Cuboid::intersect(glm::vec3 p0, glm::vec3 dir)
{
	vector<SceneObject*> sceneObjects;
	sceneObjects.push_back(front);
	sceneObjects.push_back(back);
	sceneObjects.push_back(left);
	sceneObjects.push_back(right);
	sceneObjects.push_back(top);
	sceneObjects.push_back(bottom);

	Ray ray = Ray(p0, dir);
	ray.closestPt(sceneObjects);
	if (ray.index == -1) return -1.0;
	return ray.dist;
}

glm::vec3 Cuboid::normal(glm::vec3 p)
{
	vector<Plane*> sceneObjects;
	sceneObjects.push_back(front);
	sceneObjects.push_back(back);
	sceneObjects.push_back(left);
	sceneObjects.push_back(right);
	sceneObjects.push_back(top);
	sceneObjects.push_back(bottom);

	for (Plane* plane: sceneObjects) {
		if (plane->isInside(p)) {
			return plane->normal(p);
		}
	}

	return glm::vec3(0);
}
