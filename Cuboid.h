#ifndef H_CUBOID
#define H_CUBOID
#include <glm/glm.hpp>
#include "SceneObject.h"
#include "Plane.h"

class Cuboid : public SceneObject
{

private:
    glm::vec3 center = glm::vec3(0);
    float height = 1;
    float width = 1;
    float depth = 1;

	Plane *front = new Plane (
						center + glm::vec3(0.5 * width, 	0.5 * height, 	0.5 * depth),
						center + glm::vec3(-0.5 * width,	0.5 * height, 	0.5 * depth),
						center + glm::vec3(-0.5 * width, 	-0.5 * height, 	0.5 * depth),
						center + glm::vec3(0.5 * width, 	-0.5 * height, 	0.5 * depth)
					);

	Plane *back = new Plane (
						center + glm::vec3(0.5 * width, 	-0.5 * height, 	-0.5 * depth),
						center + glm::vec3(0.5 * width, 	0.5 * height, 	-0.5 * depth),
						center + glm::vec3(-0.5 * width, 	0.5 * height, 	-0.5 * depth),
						center + glm::vec3(-0.5 * width, 	-0.5 * height, 	-0.5 * depth)
					);

    Plane *left = new Plane (
						center + glm::vec3(0.5 * width, 	-0.5 * height, 	0.5 * depth),
						center + glm::vec3(0.5 * width, 	0.5 * height, 	0.5 * depth),
						center + glm::vec3(0.5 * width, 	0.5 * height, 	-0.5 * depth),
						center + glm::vec3(0.5 * width, 	-0.5 * height, 	-0.5 * depth)
					);

    Plane *right = new Plane (
						center + glm::vec3(-0.5 * width, 	-0.5 * height, 	0.5 * depth),
						center + glm::vec3(-0.5 * width, 	-0.5 * height, 	-0.5 * depth),
						center + glm::vec3(-0.5 * width, 	0.5 * height, 	-0.5 * depth),
						center + glm::vec3(-0.5 * width, 	0.5 * height, 	0.5 * depth)
					);

    Plane *top = new Plane (
						center + glm::vec3(0.5 * width, 	0.5 * height, 	-0.5 * depth),
						center + glm::vec3(0.5 * width, 	0.5 * height, 	0.5 * depth),
						center + glm::vec3(-0.5 * width, 	0.5 * height, 	0.5 * depth),
						center + glm::vec3(-0.5 * width, 	0.5 * height, 	-0.5 * depth)
					);

    Plane *bottom = new Plane (
						center + glm::vec3(0.5 * width, 	-0.5 * height, 	-0.5 * depth),
						center + glm::vec3(0.5 * width, 	-0.5 * height, 	0.5 * depth),
						center + glm::vec3(-0.5 * width, 	-0.5 * height, 	0.5 * depth),
						center + glm::vec3(-0.5 * width, 	-0.5 * height, 	-0.5 * depth)
					);

public:
    Cuboid() {};

    Cuboid(glm::vec3 c, float h, float w, float d);

    float intersect(glm::vec3 p0, glm::vec3 dir);

    glm::vec3 normal(glm::vec3 p);

};

#endif //!H_CUBOID
