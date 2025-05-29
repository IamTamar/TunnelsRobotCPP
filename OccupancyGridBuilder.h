#pragma once
#ifndef OCCUPANCY_GRID_BUILDER_H
#define OCCUPANCY_GRID_BUILDER_H
#include <vector>
#include <string>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/octree/octree_pointcloud_occupancy.h>
    // ������ �������� ����������
    const double DEFAULT_OCTREE_RESOLUTION = 0.05; // 5 �"�
    const double DEFAULT_GRID_2D_RESOLUTION = 0.1;  // 10 �"� ��� ����� 2D

    // Y ��� �����.
    const double DEFAULT_MIN_Y_OBSTACLE = 0.0; // ����� ���� �� ��� ������
    const double DEFAULT_MAX_Y_OBSTACLE = 0.5; // ���� ������� ���� ����� ��� ����� ������� (���� ���)
    // ����� ������ ����� ����� ������� (������)
    const double DEFAULT_ROBOT_RADIUS_M = 0.3; // ������: ����� �� 30 �"�
    // ����� ��� ������
    typedef pcl::PointXYZ PointT;
    class OccupancyGridBuilder {
    public:
        // ����� ����� ���-����: 0=����, 1=�����, 2=����� (����).
       using OccupancyGrid = std::vector<std::vector<int>>;

        // ���� ������ ������ ������ ����.
        struct MapBounds {
            double minX, maxX;
            double minY, maxY; // Y ��� ����
            double minZ, maxZ;
        };
        OccupancyGridBuilder(double octreeResolution = DEFAULT_OCTREE_RESOLUTION,
            double grid2DResolution = DEFAULT_GRID_2D_RESOLUTION,
            double minYObstacle = DEFAULT_MIN_Y_OBSTACLE,
            double maxYObstacle = DEFAULT_MAX_Y_OBSTACLE,
            double robotRadiusM = DEFAULT_ROBOT_RADIUS_M);
        OccupancyGrid buildOccupancyGrid(const pcl::PointCloud<PointT>::Ptr& cloud);
        MapBounds getMapBounds() const { return mapBounds_; }

    private:
        double octreeResolution_;
        double grid2DResolution_;
        double minYObstacle_;
        double maxYObstacle_;
        int inflationRadiusCells_;
        MapBounds mapBounds_;
        void inflateObstacles(OccupancyGrid& grid, int inflationRadius);
        MapBounds findPointCloudBounds(const pcl::PointCloud<PointT>::Ptr& cloud);
    };
#endif // OCCUPANCY_GRID_BUILDER_H

