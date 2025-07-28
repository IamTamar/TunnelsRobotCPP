#include "OccupancyGridBuilder.h"
#include <iostream>
#include <limits>   // ���� numeric_limits
#include <queue>    // ���� BFS �-inflateObstacles
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
    // ����� ����� ������ ������� �� ��� ���� �� ����� ������ ���� ��������� �� �����
    inflationRadiusCells_ = static_cast<int>(robotRadiusM / grid2DResolution_) + 1;

    // ����� ������ ���� ������ ����� ������ (�� �������/�������)
    mapBounds_ = {
        numeric_limits<double>::max(), numeric_limits<double>::lowest(),
        numeric_limits<double>::max(), numeric_limits<double>::lowest(),
        numeric_limits<double>::max(), numeric_limits<double>::lowest()
    };
}

// ������� ������ ������ ��� �������
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

// ������� ������ ������� �� ��� ����� ���-���� BFS)
void OccupancyGridBuilder::inflateObstacles(OccupancyGrid& grid, int inflationRadius) {
    int gridWidth = grid.size();
    if (gridWidth == 0) return;
    int gridHeight = grid[0].size();

    OccupancyGrid inflatedGrid = grid; // ����� �� ����� ������
    std::queue<std::pair<int, int>> q;

    // ����� ���� �� �� ��� ������ ��������
    for (int i = 0; i < gridWidth; ++i) {
        for (int j = 0; j < gridHeight; ++j) {
            if (grid[i][j] == 1) { // �� ��� ����� �����
                q.push({ i, j });
                // �� ��� ���� ������ �� �����, �� ����� �����
                if (inflatedGrid[i][j] != 1 && inflatedGrid[i][j] != 2) {
                    inflatedGrid[i][j] = 1; // ���� ���� ����� ������
                }
            }
        }
    }

    // BFS ������
    // ����� �-BFS ����: ��� ����� "������" �� ����� ������
    // ����� ��� ��� "������" ���������
    vector<vector<int>> dist(gridWidth, vector<int>(gridHeight, -1));

    while (!q.empty()) {
        std::pair<int, int> current = q.front();
        q.pop();
        int cx = current.first;
        int cz = current.second;

        for (int dx = -1; dx <= 1; ++dx) {
            for (int dz = -1; dz <= 1; ++dz) {
                if (dx == 0 && dz == 0) continue; // �� ����� �� ��� ������

                int nx = cx + dx;
                int nz = cz + dz;

                // ���� ����� ���� ������ �����
                if (nx >= 0 && nx < gridWidth && nz >= 0 && nz < gridHeight) {
                    // �� ����� �� ������ ��� �� (dist[nx][nz] == -1)
                    if (dist[nx][nz] == -1) {
                        dist[nx][nz] = dist[cx][cz] + 1;
                        if (dist[nx][nz] <= inflationRadius) {
                            inflatedGrid[nx][nz] = 2; // ���� ��� �����/�����
                            q.push({ nx, nz }); 
                        }
                    }
                }
            }
        }
    }
    grid = inflatedGrid; // ����� �� ����� ������ �� ������
}

// ������� ������ ��� ������
OccupancyGridBuilder::OccupancyGrid OccupancyGridBuilder::buildOccupancyGrid(const pcl::PointCloud<PointT>::Ptr& cloud) {
    if (!cloud || cloud->empty()) {
        cerr << "Input cloud is empty or null." << std::endl;
        return {}; // ����� ���� ���
    }

    // 1. ����� ������ ��� �������
    mapBounds_ = findPointCloudBounds(cloud);
    cout << "Cloud bounds: X[" << mapBounds_.minX << ", " << mapBounds_.maxX
        << "] Y[" << mapBounds_.minY << ", " << mapBounds_.maxY
        << "] Z[" << mapBounds_.minZ << ", " << mapBounds_.maxZ << "]" << std::endl;

    // 2. ����� Octree ������ ������
    pcl::octree::OctreePointCloudOccupancy<PointT> octree(octreeResolution_);
    octree.setInputCloud(cloud);
    octree.addPointsFromInputCloud();
    cout << "Octree created with resolution: " << octreeResolution_ << "m" << std::endl;

    // 3. ����� ��� ����� ��-����� (����� XZ)
    int gridWidth = static_cast<int>((mapBounds_.maxX - mapBounds_.minX) / grid2DResolution_) + 1;
    int gridHeight = static_cast<int>((mapBounds_.maxZ - mapBounds_.minZ) / grid2DResolution_) + 1;

    OccupancyGrid occupancyGrid2D(gridWidth, std::vector<int>(gridHeight, 0)); // 0 = ����

    cout << "Initializing 2D Occupancy Grid (XZ plane): "
        << gridWidth << "x" << gridHeight << " cells." << endl;

    // 4. ����� ����� ���-���� �� ���� ����� �-Octree (������ XZ)
    vector<PointT, Eigen::aligned_allocator<PointT>> occupiedVoxelCenters;
    octree.getOccupiedVoxelCenters(occupiedVoxelCenters);

    cout << "Mapping occupied voxels to 2D grid ("
        << occupiedVoxelCenters.size() << " occupied voxels)..." << endl;
    for (const auto& voxelCenter : occupiedVoxelCenters) {
        //  �� ������ ���� ����� ����� (Y) �������� ���� ��������
        if (voxelCenter.y >= minYObstacle_ && voxelCenter.y <= maxYObstacle_) {
            //  ����������� ���� (X, Z) ������������ ���� 2D
            int gx = static_cast<int>((voxelCenter.x - mapBounds_.minX) / grid2DResolution_);
            int gz = static_cast<int>((voxelCenter.z - mapBounds_.minZ) / grid2DResolution_);

            // ����� ���� ���� ������ �����
            if (gx >= 0 && gx < gridWidth && gz >= 0 && gz < gridHeight) {
                if (occupancyGrid2D[gx][gz] == 0) {
                    occupancyGrid2D[gx][gz] = 1;
                }
            }
        }
    }
    cout << "2D Occupancy Grid populated." << endl;

    // 5. ����� ������� �� ����� ���-����
    cout << "Inflating obstacles by " << inflationRadiusCells_ << " cells..." << endl;
    inflateObstacles(occupancyGrid2D, inflationRadiusCells_);
    cout << "Obstacles inflated." << endl;

    return occupancyGrid2D;
}