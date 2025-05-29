#pragma once
#ifndef OCCUPANCY_GRID_BUILDER_H
#define OCCUPANCY_GRID_BUILDER_H
#include <vector>
#include <string>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/octree/octree_pointcloud_occupancy.h>
    // הגדרות גלובליות וקונסטנטים
    const double DEFAULT_OCTREE_RESOLUTION = 0.05; // 5 ס"מ
    const double DEFAULT_GRID_2D_RESOLUTION = 0.1;  // 10 ס"מ לתא בגריד 2D

    // Y הוא הגובה.
    const double DEFAULT_MIN_Y_OBSTACLE = 0.0; // התקרה עצמה או קצת מתחתיה
    const double DEFAULT_MAX_Y_OBSTACLE = 0.5; // גובה מקסימלי מתחת לתקרה שבו נחשיב מכשולים (כלפי מטה)
    // רדיוס הרובוט לצורך ניפוח מכשולים (במטרים)
    const double DEFAULT_ROBOT_RADIUS_M = 0.3; // לדוגמה: רדיוס של 30 ס"מ
    // הגדרת סוג הנקודה
    typedef pcl::PointXYZ PointT;
    class OccupancyGridBuilder {
    public:
        // ייצוג הגריד הדו-ממדי: 0=פנוי, 1=מכשול, 2=מנופח (חסום).
       using OccupancyGrid = std::vector<std::vector<int>>;

        // מבנה נתונים לשמירת גבולות המפה.
        struct MapBounds {
            double minX, maxX;
            double minY, maxY; // Y הוא גובה
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

