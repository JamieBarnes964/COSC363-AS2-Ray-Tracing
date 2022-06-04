#include "Cylinder.h"
#include <math.h>
#include <iostream>
using namespace std;



float Cylinder::intersect(glm::vec3 p0, glm::vec3 dir)
{
	float a = pow(dir.x, 2) + pow(dir.z, 2);
	float b = 2.0 * (float)(
		(dir.x * (float)(p0.x - center.x)) +
		(dir.z * (float)(p0.z - center.z))
	);
	float c =
		pow(p0.x - center.x, 2) +
		pow(p0.z - center.z, 2) -
		pow(radius, 2)
	;

	float delta = b*b - 4 * a * c;

	if (delta < 0) return -1;

	float t1 = (- b - sqrt(delta)) / (2 * a);
	float t2 = (- b + sqrt(delta)) / (2 * a);

	if (t1 < 0 && t2 < 0) return -1;

	if (min(t1, t2) < 0) {
		if (
			(p0 + min(t1, t2) * dir).y < center.y + height &&
			(p0 + min(t1, t2) * dir).y > center.y &&
			((p0 + max(t1, t2) * dir).y > center.y + height ||
			(p0 + max(t1, t2) * dir).y < center.y)
		) return -1;
	}

	if ((p0 + min(t1, t2) * dir).y > (center.y + height)) {
		if ((p0 + max(t1, t2) * dir).y > (center.y + height)) return -1;
		else {
			return ((center.y + height) - p0.y) / dir.y;
		}
	}

	if ((p0 + min(t1, t2) * dir).y < center.y) {
		if ((p0 + max(t1, t2) * dir).y < center.y) return -1;
		else {
			return (center.y - p0.y) / dir.y;
		}
	}



	// if (p0.y + t1 * dir.y > -10 && p0.y + t2 * dir.y > -10) cout << p0.y + t1 * dir.y << " " << p0.y + t2 * dir.y << "\n";



	if (t1 < 0) {
		// cout << t1 << ' ' << t2 << "\n";
		return (t2 > 0) ? t2 : -1;
	}
	else
	{
		// cout << t1 << ' ' << t2 << "\n";
		return t1;
	}
}


glm::vec3 Cylinder::normal(glm::vec3 p)
{
	if (p.y >= center.y + height - 0.01) {
		glm::vec3 n = glm::vec3(0,1,0);
		// glm::vec3 n = glm::vec3(0, p.y - (center.y + height / 2), 0);
		n = glm::normalize(n);
		return n;
	}
	if (p.y <= center.y + 0.01) {
		glm::vec3 n = glm::vec3(0,-1,0);
		// glm::vec3 n = glm::vec3(0, p.y - (center.y + height / 2), 0);
		n = glm::normalize(n);
		return n;
	}
	glm::vec3 n = glm::vec3(p.x - center.x, 0, p.z - center.z);
	n = glm::normalize(n);
	return n;

}
