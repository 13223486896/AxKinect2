#include "AxPCTransform.h"

AxPCTransform::AxPCTransform(QWidget *parent)
	: QMainWindow(parent)
	, isrgb(true)
{
	ui.setupUi(this);
	QObject::connect(ui.action_transformPc, SIGNAL(triggered()), this, SLOT(TransformPointClouds()));
}

void AxPCTransform::TransformPointClouds()
{
	std::string keyFramePath = "E:/PointCloudData/520KinectOrbSLAM/";
	std::string keyFrameFilename="KeyFrameTrajectory.txt";
	std::string Trajactry = keyFramePath + keyFrameFilename;
	//��ȡ�ؼ�֡λ���ļ�
	FILE *file = fopen(Trajactry.c_str(), "r");
	if (file == NULL)
	{
		return;
	}
	double frameId;
	float x, y, z, a, b, c,d;
	//x,y,z,��Ԫ��
	fscanf(file, "%lf %f %f %f %f %f %f %f", &frameId, &x, &y, &z, &a, &b, &c, &d);
	while (feof(file) == 0) /*�ж��Ƿ��ļ�β��������ѭ��*/
	{
		fscanf(file, "%lf %f %f %f %f %f %f %f", &frameId, &x, &y, &z, &a, &b, &c, &d);
		char astr[20];
		sprintf(astr, "%8.6lf", frameId);
		std::string aas = astr;
		std::string fileNameFrame = aas + ".txt";
		Quaternionf Q1(a, b, c, d);
		Vector3f transf(x, y, z);
		//������Ҫת���ĵ��ƣ�����Ԫ��, x, y, z, a, b, c, d
		ReadPointCloud(keyFramePath,fileNameFrame, transf, Q1);
	}
	fclose(file);
	//
}

void AxPCTransform::ReadPointCloud(std::string filePath,std::string fileName, Vector3f& transform, Quaternionf &Q1)
{
	std::string fileNameSave = filePath+"fusion_" + fileName;
	std::string fileNameOpen = filePath +  fileName;
	FILE *file = fopen(fileNameOpen.c_str(), "r");
	if (file == NULL)
	{
		return;
	}
	FILE *fileSave = fopen(fileNameSave.c_str(), "w");
	if (fileSave == NULL)
	{
		return;
	}
	AngleAxisf V5;
	Vector3f coord;
	V5 = Q1;
	float x, y, z;
	int  a, b, c;
	if (!isrgb)
	{
		fscanf(file, "%f %f %f", &x, &y, &z);
	}
	else
	{
		fscanf(file, "%f %f %f %d %d %d", &x, &y, &z, &a, &b, &c);
		float cameraX = static_cast<float>(x);
		float cameraY = -static_cast<float>(z);
		float cameraZ = static_cast<float>(y);
		coord << cameraX, cameraY, cameraZ;   //Ĭ�ϵ�����Ϊ������
		coord = V5.matrix()*coord + transform;
		fprintf(fileSave, "%f %f %f %d %d %d\n", coord.x(), coord.y(), coord.z(), a, b, c);
	}

	while (feof(file) == 0) /*�ж��Ƿ��ļ�β��������ѭ��*/
	{

		if (!isrgb)
		{
			fscanf(file, "%f %f %f", &x, &y, &z);
		}
		else
		{
			fscanf(file, "%f %f %f %d %d %d", &x, &y, &z, &a, &b, &c);
			float cameraX = static_cast<float>(x);
			float cameraY = -static_cast<float>(z);
			float cameraZ = static_cast<float>(y);
			coord << cameraX, cameraY, cameraZ;   //Ĭ�ϵ�����Ϊ������
			coord = V5.matrix()*coord + transform;
			fprintf(fileSave, "%f %f %f %d %d %d\n", coord.x(), coord.y(), coord.z(), a, b, c);
		}
	}
	fclose(file);
	fclose(fileSave);
}
