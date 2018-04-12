// AxGeneratePointCloudDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <iostream>
#include <string>
using namespace std;

// OpenCV ��
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
// ����ڲ�
const double camera_factor = 1000;
const double camera_cx = 325.5;
const double camera_cy = 253.5;
const double camera_fx = 518.0;
const double camera_fy = 519.0;

int _tmain(int argc, _TCHAR* argv[])
{
	// ͼ�����
	cv::Mat rgb, depth;
	// ʹ��cv::imread()����ȡͼ��
	rgb = cv::imread("./data/rgb.png");
	// rgb ͼ����8UC3�Ĳ�ɫͼ��
	// depth ��16UC1�ĵ�ͨ��ͼ��ע��flags����-1,��ʾ��ȡԭʼ���ݲ����κ��޸�
	depth = cv::imread("./data/depth.png", -1);

	std::string fileNameSave = "pointcloud.txt";
	FILE *fileSave = fopen(fileNameSave.c_str(), "w");
	if (fileSave == NULL)
	{
		return 0;
	}
	// �������ͼ
	for (int m = 0; m < depth.rows; m++)
		for (int n = 0; n < depth.cols; n++)
		{
			// ��ȡ���ͼ��(m,n)����ֵ
			ushort d = depth.ptr<ushort>(m)[n];
			// d ����û��ֵ������ˣ������˵�
			if (d == 0)
				continue;
			// d ����ֵ�������������һ����
			float x, y, z;
			int r, g, b;
			// ���������Ŀռ�����
			z = double(d) / camera_factor;
			x = (n - camera_cx) * z / camera_fx;
			y = (m - camera_cy) * z / camera_fy;

			// ��rgbͼ���л�ȡ������ɫ
			// rgb����ͨ����BGR��ʽͼ�����԰������˳���ȡ��ɫ
			b = rgb.ptr<uchar>(m)[n * 3];
			g = rgb.ptr<uchar>(m)[n * 3 + 1];
			r = rgb.ptr<uchar>(m)[n * 3 + 2];

			fprintf(fileSave, "%f %f %f %d %d %d\n", x, y, z, r, g, b);
		}
	fclose(fileSave);
	system("pause");
	return 0;
}

