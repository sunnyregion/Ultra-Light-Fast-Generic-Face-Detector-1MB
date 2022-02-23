//
//  main.cpp
//  UltraFaceTest
//
//  Created by vealocia on 2019/10/17.
//  Copyright © 2019 vealocia. All rights reserved.
//

#include "UltraFace.hpp"
#include "GenderAge.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv) {
    if (argc <= 3) {
        fprintf(stderr, "Usage: %s <ncnn bin> <ncnn param> [image files...]\n", argv[0]);
        return 1;
    }

    std::string bin_path = argv[1];
    std::string param_path = argv[2];
    UltraFace ultraface(bin_path, param_path, 320, 240, 1, 0.7); // config model input

    auto bin= "../data/genderage.bin";
    auto param = "../data/genderage.param";
    // std::string bin = argv[3];
    // std::string param = argv[4];
    GenderAge genderface(bin,param);
    
    for (int i = 3; i < argc; i++) {
        std::string image_file = argv[i];
        std::cout << "Processing " << image_file << std::endl;

        cv::Mat frame = cv::imread(image_file);
        ncnn::Mat inmat = ncnn::Mat::from_pixels(frame.data, ncnn::Mat::PIXEL_BGR2RGB, frame.cols, frame.rows);

        std::vector<FaceInfo> face_info;
        ultraface.detect(inmat, face_info);

        std::vector<GenderAgeInfo> gender_age_info;
        for (int i = 0; i < face_info.size(); i++) {
            auto face = face_info[i];
            cv::Point pt1(face.x1, face.y1);
            cv::Point pt2(face.x2, face.y2);
            cv::rectangle(frame, pt1, pt2, cv::Scalar(0, 255, 0), 2);
            cv::Rect roi(face.x1, face.y1, face.x2-face.x1, face.y2-face.y1);
            cv::Mat image_roi = frame(roi);
            // image_roi
            face_box box;
            box.x0 = face.x1;
            box.y0 =  face.y1;
            box.x1 = face.x2;
            box.y1 = face.y2;
            // box.landmark=face.landmarks[0];
            // for (int i = 0; i < 5; i++)
            // {
            //     box.landmark.x[i] = face.landmarks
            //     box.landmark.y[i] = face_info[0].pts.y[i];
            // }
            genderface.detect(frame,gender_age_info);
            std::cout << "--------------性别："<<gender_age_info[i].gender<<"\t 年龄："<<gender_age_info[i].age<<"------------"<<std::endl;
            //  genderface.detect(image_roi,gender_age_info);
            
            cv::imwrite("./hello.jpg",image_roi);
        }

        cv::imshow("UltraFace", frame);
        cv::waitKey();
        cv::imwrite("./result.jpg", frame);
    }
    return 0;
}
