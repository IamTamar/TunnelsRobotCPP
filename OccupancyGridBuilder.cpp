#include "OccupancyGridBuilder.h"
#include <iostream>
#include <limits>   // עבור numeric_limits
#include <queue>    // עבור BFS ב-inflateObstacles
#include <vector>

using namespace std;
OccupancyGridBuilder::OccupancyGridBuilder(double octreeResolution,
    double grid2DResolution,
    double minYObstacle,
    double maxYObstacle,
    double robotRadiusM)
    : octreeResolution_(octreeResolution),
    grid2DResolution_(grid2DResolution),
    minYObstacle_(minYObstacle),
    maxYObstacle_(maxYObstacle)
{
    // חישוב רדיוס הניפוח ביחידות של תאי גריד עי חלוקת הרדיוס חלקי הרזולוציה של הגריד
    inflationRadiusCells_ = static_cast<int>(robotRadiusM / grid2DResolution_) + 1;

    // אתחול גבולות המפה לערכים שאינם חוקיים (או מינימום/מקסימום)
    mapBounds_ = {
        numeric_limits<double>::max(), numeric_limits<double>::lowest(),
        numeric_limits<double>::max(), numeric_limits<double>::lowest(),
        numeric_limits<double>::max(), numeric_limits<double>::lowest()
    };
}

// פונקציה לחיפוש גבולות ענן הנקודות
OccupancyGridBuilder::MapBounds OccupancyGridBuilder::findPointCloudBounds(const pcl::PointCloud<PointT>::Ptr& cloud) {
    MapBounds bounds;
    bounds.minX = numeric_limits<double>::max();
    bounds.maxX = numeric_limits<double>::lowest();
    bounds.minY = numeric_limits<double>::max();
    bounds.maxY = numeric_limits<double>::lowest();
    bounds.minZ = numeric_limits<double>::max();
    bounds.maxZ = numeric_limits<double>::lowest();

    for (const auto& point : *cloud) {
        if (point.x < bounds.minX) bounds.minX = point.x;
        if (point.x > bounds.maxX) bounds.maxX = point.x;
        if (point.y < bounds.minY) bounds.minY = point.y;
        if (point.y > bounds.maxY) bounds.maxY = point.y;
        if (point.z < bounds.minZ) bounds.minZ = point.z;
        if (point.z > bounds.maxZ) bounds.maxZ = point.z;
    }
    return bounds;
}

// פונקציה לניפוח מכשולים על גבי הגריד הדו-ממדי BFS)
void OccupancyGridBuilder::inflateObstacles(OccupancyGrid& grid, int inflationRadius) {
    int gridWidth = grid.size();
    if (gridWidth == 0) return;
    int gridHeight = grid[0].size();

    OccupancyGrid inflatedGrid = grid; // מעתיק את הגריד המקורי
    std::queue<std::pair<int, int>> q;

    // אתחול התור עם כל תאי המכשול המקוריים
    for (int i = 0; i < gridWidth; ++i) {
        for (int j = 0; j < gridHeight; ++j) {
            if (grid[i][j] == 1) { // אם זהו מכשול מקורי
                q.push({ i, j });
                // אם כבר סומן כחסימה או ניפוח, אל תחזיר אחורה
                if (inflatedGrid[i][j] != 1 && inflatedGrid[i][j] != 2) {
                    inflatedGrid[i][j] = 1; // מודא שהוא מסומן כמכשול
                }
            }
        }
    }

    // BFS לניפוח
    // ההבדל מ-BFS רגיל: כאן אנחנו "סופרים" את רדיוס הניפוח
    // נוצרת כאן מפת "מרחקים" מהמכשולים
    vector<vector<int>> dist(gridWidth, vector<int>(gridHeight, -1));

    while (!q.empty()) {
        std::pair<int, int> current = q.front();
        q.pop();
        int cx = current.first;
        int cz = current.second;

        for (int dx = -1; dx <= 1; ++dx) {
            for (int dz = -1; dz <= 1; ++dz) {
                if (dx == 0 && dz == 0) continue; // לא לבדוק את התא הנוכחי

                int nx = cx + dx;
                int nz = cz + dz;

                // לודא שהשכן בתוך גבולות הגריד
                if (nx >= 0 && nx < gridWidth && nz >= 0 && nz < gridHeight) {
                    // אם עדיין לא ביקרנו בתא זה (dist[nx][nz] == -1)
                    if (dist[nx][nz] == -1) {
                        dist[nx][nz] = dist[cx][cz] + 1;
                        if (dist[nx][nz] <= inflationRadius) {
                            inflatedGrid[nx][nz] = 2; // מסמן כתא מנופח/חסימה
                            q.push({ nx, nz }); 
                        }
                    }
                }
            }
        }
    }
    grid = inflatedGrid; // מעדכן את הגריד המקורי עם הניפוח
}

// פונקציה לבניית מפת התפוסה
OccupancyGridBuilder::OccupancyGrid OccupancyGridBuilder::buildOccupancyGrid(const pcl::PointCloud<PointT>::Ptr& cloud) {
    if (!cloud || cloud->empty()) {
        cerr << "Input cloud is empty or null." << std::endl;
        return {}; // מחזיר גריד ריק
    }

    // 1. מציאת גבולות ענן הנקודות
    mapBounds_ = findPointCloudBounds(cloud);
    cout << "Cloud bounds: X[" << mapBounds_.minX << ", " << mapBounds_.maxX
        << "] Y[" << mapBounds_.minY << ", " << mapBounds_.maxY
        << "] Z[" << mapBounds_.minZ << ", " << mapBounds_.maxZ << "]" << std::endl;

    // 2. יצירת Octree והוספת נקודות
    pcl::octree::OctreePointCloudOccupancy<PointT> octree(octreeResolution_);
    octree.setInputCloud(cloud);
    octree.addPointsFromInputCloud();
    cout << "Octree created with resolution: " << octreeResolution_ << "m" << std::endl;

    // 3. יצירת מפת תפוסה דו-ממדית (מישור XZ)
    int gridWidth = static_cast<int>((mapBounds_.maxX - mapBounds_.minX) / grid2DResolution_) + 1;
    int gridHeight = static_cast<int>((mapBounds_.maxZ - mapBounds_.minZ) / grid2DResolution_) + 1;

    OccupancyGrid occupancyGrid2D(gridWidth, std::vector<int>(gridHeight, 0)); // 0 = פנוי

    cout << "Initializing 2D Occupancy Grid (XZ plane): "
        << gridWidth << "x" << gridHeight << " cells." << endl;

    // 4. מילוי הגריד הדו-ממדי על בסיס נתוני ה-Octree (במישור XZ)
    vector<PointT, Eigen::aligned_allocator<PointT>> occupiedVoxelCenters;
    octree.getOccupiedVoxelCenters(occupiedVoxelCenters);

    cout << "Mapping occupied voxels to 2D grid ("
        << occupiedVoxelCenters.size() << " occupied voxels)..." << endl;
    for (const auto& voxelCenter : occupiedVoxelCenters) {
        //  אם הווקסל נמצא בטווח הגובה (Y) הרלוונטי עבור המכשולים
        if (voxelCenter.y >= minYObstacle_ && voxelCenter.y <= maxYObstacle_) {
            //  קואורדינטות עולם (X, Z) לקואורדינטות גריד 2D
            int gx = static_cast<int>((voxelCenter.x - mapBounds_.minX) / grid2DResolution_);
            int gz = static_cast<int>((voxelCenter.z - mapBounds_.minZ) / grid2DResolution_);

            // מוודא שהתא בתוך גבולות הגריד
            if (gx >= 0 && gx < gridWidth && gz >= 0 && gz < gridHeight) {
                if (occupancyGrid2D[gx][gz] == 0) {
                    occupancyGrid2D[gx][gz] = 1;
                }
            }
        }
    }
    cout << "2D Occupancy Grid populated." << endl;

    // 5. ניפוח מכשולים על הגריד הדו-ממדי
    cout << "Inflating obstacles by " << inflationRadiusCells_ << " cells..." << endl;
    inflateObstacles(occupancyGrid2D, inflationRadiusCells_);
    cout << "Obstacles inflated." << endl;

    return occupancyGrid2D;
}