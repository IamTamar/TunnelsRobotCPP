#pragma once
#include "GasSensors.h"

//קוד מgpt שאמור לקרוא קובץ csv של רמות חישני גז
/*סיכום: מה להגדיר בתור סף קריטי לרובוט שלך ?
? CO? > 2000 PPM ? כנראה שיש אנשים.
? CO > 100 PPM ? ייתכן מקור חיצוני של שריפה, חומרים מסוכנים או אדם.
? NH? > 10 PPM ? ייתכן אדם או חומרי דלק.
? NO? > 3 PPM ? אפשרות לדליפת חומרי נפץ.
? אם צד שמאל גבוה ב - 50 % מצד ימין ? כנראה שהמקור משמאל.
? אם יש עלייה פתאומית תוך שניות / דקות ? משהו חריג מתרחש.*/
//צרי קובץ בשם sensors.csv עם התוכן הבא :
//CO, CO2, H2S, CH4
//120, 800, 20, 4000
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <vector>
//
//std::vector<float> readCSV(const std::string& filename) {
//    std::vector<float> values;
//    std::ifstream file(filename);
//    std::string line;
//
//    if (!file) {
//        std::cerr << "שגיאה: לא ניתן לפתוח את הקובץ " << filename << std::endl;
//        return values;
//    }
//
//    std::getline(file, line); // דילוג על כותרת
//    std::getline(file, line); // קריאת הנתונים
//
//    std::stringstream ss(line);
//    std::string value;
//
//    while (std::getline(ss, value, ',')) {
//        values.push_back(std::stof(value)); // המרה למספרים
//    }
//
//    file.close();
//    return values;
//}
//
//int main() {
//    std::string filename = "sensors.csv";
//    std::vector<float> data = readCSV(filename);
//
//    if (!data.empty()) {
//        std::cout << "קריאת נתונים מהקובץ CSV:\n";
//        std::cout << "CO: " << data[0] << " PPM\n";
//        std::cout << "CO2: " << data[1] << " PPM\n";
//        std::cout << "H2S: " << data[2] << " PPM\n";
//        std::cout << "CH4: " << data[3] << " PPM\n";
//    }
//
//    return 0;
//}

