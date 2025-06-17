#include <glad/glad.h>
#include "Frustum.h"

static Frustum frustum;



Frustum& Frustum::getFrustum() {
    frustum.calculateFrustum();
    return frustum;
}

