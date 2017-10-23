// AxFeatureExtract.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "vector"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	cv::Mat rgb1 = cv::imread( "data\\2017- 4-11-15-52-47-rgb.png");
	cv::Mat rgb2 = cv::imread( "data\\2017- 4-11-15-53- 0-rgb.png");
	/*cv::Mat rgb1 = cv::imread( "G:\\SLAMDatasets\\һ����RGB-DSLAM����\\rgb_png\\1.png");
	cv::Mat rgb2 = cv::imread( "G:\\SLAMDatasets\\һ����RGB-DSLAM����\\rgb_png\\2.png");*/
	//cv::Mat depth1 = cv::imread( "G:\\SLAMDatasets\\һ����RGB-DSLAM����\\depth_png\\1.png", -1);
	//cv::Mat depth2 = cv::imread( "G:\\SLAMDatasets\\һ����RGB-DSLAM����\\depth_png\\2.png", -1);
	 
	// ����������ȡ������������ȡ��
	cv::Ptr<cv::FeatureDetector> _detector;
	cv::Ptr<cv::DescriptorExtractor> _descriptor;
	
	// ������ȡ����Ĭ�����߶�Ϊsift
	// ����sift, surf֮ǰҪ��ʼ��nonfreeģ��
	cv::initModule_nonfree();
	_detector = cv::FeatureDetector::create( "GridSIFT" );
	_descriptor = cv::DescriptorExtractor::create( "SIFT" );
	 
	vector< cv::KeyPoint > kp1, kp2; //�ؼ���
	_detector->detect( rgb1, kp1 );  //��ȡ�ؼ���
	_detector->detect( rgb2, kp2 );
	
	cout<<"Key points of two images: "<<kp1.size()<<", "<<kp2.size()<<endl;
	    
	// ���ӻ��� ��ʾ�ؼ���
	cv::Mat imgShow;
	cv::drawKeypoints( rgb1, kp1, imgShow, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
	cv::imshow( "keypoints", imgShow );
	cv::imwrite( "data\\keypoints.png", imgShow );
	cv::waitKey(0); //��ͣ�ȴ�һ������
	   
	// ����������
	cv::Mat desp1, desp2;
	_descriptor->compute( rgb1, kp1, desp1 );
	_descriptor->compute( rgb2, kp2, desp2 );
	
	// ƥ��������
	vector< cv::DMatch > matches; 
	cv::FlannBasedMatcher matcher;
	matcher.match( desp1, desp2, matches );
	cout<<"Find total "<<matches.size()<<" matches."<<endl;
	
	// ���ӻ�����ʾƥ�������
	cv::Mat imgMatches;
	cv::drawMatches( rgb1, kp1, rgb2, kp2, matches, imgMatches );
	cv::imshow( "matches", imgMatches );
	cv::imwrite( "data\\matches.png", imgMatches );
	cv::waitKey( 0 );
	
	// ɸѡƥ�䣬�Ѿ���̫���ȥ��
	// ����ʹ�õ�׼����ȥ�������ı���С�����ƥ��
	vector< cv::DMatch > goodMatches;
	double minDis = 9999;
	for ( size_t i=0; i<matches.size(); i++ )
	{
			if ( matches[i].distance < minDis )
				minDis = matches[i].distance;
	}
	
	for ( size_t i=0; i<matches.size(); i++ )
	{
		if (matches[i].distance < 4*minDis)
			goodMatches.push_back( matches[i] );
	}

		// ��ʾ good matches
	cout<<"good matches="<<goodMatches.size()<<endl;
	cv::drawMatches( rgb1, kp1, rgb2, kp2, goodMatches, imgMatches );
	cv::imshow( "good matches", imgMatches );
	cv::imwrite( "data\\good_matches.png", imgMatches );
	system("pause");
	return 0;
}

