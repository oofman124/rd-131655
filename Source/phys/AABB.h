#pragma once

class AABB {
public:
    double minX, minY, minZ;
    double maxX, maxY, maxZ;

    static constexpr double epsilon = 0.0;

    // Constructor
    AABB(double minX, double minY, double minZ, double maxX, double maxY, double maxZ)
        : minX(minX), minY(minY), minZ(minZ), maxX(maxX), maxY(maxY), maxZ(maxZ) {}

    // Clone
    AABB clone() const {
        return AABB(minX, minY, minZ, maxX, maxY, maxZ);
    }

    // Expand (one side)
    AABB expand(double x, double y, double z) const {
        double nMinX = minX, nMinY = minY, nMinZ = minZ;
        double nMaxX = maxX, nMaxY = maxY, nMaxZ = maxZ;
        if (x < 0.0) nMinX += x; else nMaxX += x;
        if (y < 0.0) nMinY += y; else nMaxY += y;
        if (z < 0.0) nMinZ += z; else nMaxZ += z;
        return AABB(nMinX, nMinY, nMinZ, nMaxX, nMaxY, nMaxZ);
    }

    // Grow (both sides)
    AABB grow(double x, double y, double z) const {
        return AABB(minX - x, minY - y, minZ - z, maxX + x, maxY + y, maxZ + z);
    }

    // Clip X collision
    double clipXCollide(const AABB& other, double x) const {
        if (other.maxY <= minY || other.minY >= maxY) return x;
        if (other.maxZ <= minZ || other.minZ >= maxZ) return x;
        if (x > 0.0 && other.maxX <= minX) {
            double max = minX - other.maxX - epsilon;
            if (max < x) x = max;
        }
        if (x < 0.0 && other.minX >= maxX) {
            double max = maxX - other.minX + epsilon;
            if (max > x) x = max;
        }
        return x;
    }

    // Clip Y collision
    double clipYCollide(const AABB& other, double y) const {
        if (other.maxX <= minX || other.minX >= maxX) return y;
        if (other.maxZ <= minZ || other.minZ >= maxZ) return y;
        if (y > 0.0 && other.maxY <= minY) {
            double max = minY - other.maxY - epsilon;
            if (max < y) y = max;
        }
        if (y < 0.0 && other.minY >= maxY) {
            double max = maxY - other.minY + epsilon;
            if (max > y) y = max;
        }
        return y;
    }

    // Clip Z collision
    double clipZCollide(const AABB& other, double z) const {
        if (other.maxX <= minX || other.minX >= maxX) return z;
        if (other.maxY <= minY || other.minY >= maxY) return z;
        if (z > 0.0 && other.maxZ <= minZ) {
            double max = minZ - other.maxZ - epsilon;
            if (max < z) z = max;
        }
        if (z < 0.0 && other.minZ >= maxZ) {
            double max = maxZ - other.minZ + epsilon;
            if (max > z) z = max;
        }
        return z;
    }

    // Intersection test
    bool intersects(const AABB& other) const {
        if (other.maxX <= minX || other.minX >= maxX) return false;
        if (other.maxY <= minY || other.minY >= maxY) return false;
        if (other.maxZ <= minZ || other.minZ >= maxZ) return false;
        return true;
    }

    // Move in place
    void move(double x, double y, double z) {
        minX += x; minY += y; minZ += z;
        maxX += x; maxY += y; maxZ += z;
    }

    // Offset (returns new AABB)
    AABB offset(double x, double y, double z) const {
        return AABB(minX + x, minY + y, minZ + z, maxX + x, maxY + y, maxZ + z);
    }
};