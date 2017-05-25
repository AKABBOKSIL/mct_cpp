Mat floatConvert(Mat img)
{
	// �̹����� float ��ķ� ��ȯ
	img.convertTo(img, CV_32FC3, (1.0 / 255));

	float maxValue = 1.0, minValue = 0.0;

	// �ּ� �� �ִ��� ���
	for (int r = 0; r < img.rows; r++)
	{
		float* imgPtr = img.ptr<float>(r);
		for (int c = 0; c < img.cols; c++)
		{
			if (imgPtr[c] > maxValue)
				maxValue = imgPtr[c];
			if (imgPtr[c] < minValue)
				minValue = imgPtr[c];
		}
	}

	//�߸��� �� ���� ��� ���� ���
	if (minValue < 0.0 && maxValue > 1.0)
	{
		cout << "���� �ʴ� �������� �� [0,1]!" << endl;

		// �̹����� 0���� �����ǰ�
		img = Mat::zeros(img.rows, img.cols, CV_32FC3);
		return img;

	}
	else
	{
		return img;
	}

}
void CAppleDetection::HogFeatureDetector(unsigned char *pSrcImage, long width, long height, unsigned char *pHogImage)
{
	//uchar[]�� openCV matrix ������ ���� ��ȯ
	Mat m1 = Mat(height, width, CV_8U, pSrcImage);
	Mat hog, extracted_hog;


	//In float Convert
	hog = floatConvert(m1);


	extracted_hog = extractHOG(hog, 9); // HOG Feature ���� �� ���� resize(288x384)


	// ����ȯ ����

	Mat img2, img22;
	extracted_hog.convertTo(img2, CV_8U, 255); //CV_32FC1 �� float ���� min-max = [0,1], CV_8U �� int ���� min-max = [0,255]



	unsigned char *pdata = pHogImage;
	int count = width*height;
	for (int i = 0; i<count; i++)
		*pdata++ = img2.data[i];
	if (count3++ == 0) imwrite("HOG123.bmp", Mat(height, width, CV_8U, pHogImage));
	return;
}

Mat extractHOG(Mat rootImg, vl_size orientations)		 //���� ���ã�� : HogFeatureDetector
{
	Mat img = rootImg;

	//���� ��� ���� ������
	float* imgPtr = img.ptr<float>(0);

	// ������ ��
	vl_size numOrientations = orientations;
	// HOG ���� ũ��
	vl_size cellSize = 4;
	float* hogArray;

	//HOG ����� ���� 
	//VlHog * hog = vl_hog_new(VlHogVariantUoctti, numOrientations, VL_TRUE);			// Uoctti, DalalTriggs (VL_TRUE,VL_FALSE)�� ���� �ٲ�
	VlHog * hog = vl_hog_new(VlHogVariantDalalTriggs, numOrientations, VL_FALSE);


	//vl_hog_set_use_bilinear_orientation_assignments (hog, VL_TRUE) ;//�߰���


	vl_hog_put_image(hog, imgPtr, img.cols, img.rows, img.channels(), cellSize);

	vl_size hogWidth = vl_hog_get_width(hog);
	vl_size hogHeight = vl_hog_get_height(hog);
	vl_size hogDimension = vl_hog_get_dimension(hog);
	hogArray = (float*)vl_malloc(hogWidth*hogHeight*hogDimension*sizeof(float));
	//printf("%d %d %d",hogWidth,hogHeight,hogDimension);
	vl_hog_extract(hog, (float*)hogArray);

	vl_size glyphSize = vl_hog_get_glyph_size(hog);
	vl_size imageHeight = glyphSize * hogHeight;
	vl_size imageWidth = glyphSize * hogWidth;
	float* hogPtr = (float*)vl_malloc(sizeof(float)*imageWidth*imageHeight);
	vl_hog_render(hog, hogPtr, hogArray, hogWidth, hogHeight);




	// �� ���� �ٽ� ����
	Mat hogImg = Mat::zeros((int)imageHeight, (int)imageWidth, CV_32FC1);
	int i = 0;
	for (int r = 0; r < hogImg.rows; r++)
	{
		imgPtr = hogImg.ptr<float>(r);
		for (int c = 0; c < hogImg.cols; c++)
		{
			imgPtr[c] = hogPtr[i];
			// printf("%f ", hogPtr[i]);
			i++;
		}
	}

	// ũ�� ����
	resize(hogImg, hogImg, Size(img.cols, img.rows), 3, 3, CV_INTER_AREA); // Bi-Linear(288x384 ũ��� ����) CV_INTER_LINEAR, CV_INTER_CUBIC, CV_INTER_AREA
	//resize(hogImg,hogImg,Size(252,336),0.5,0.5); // Bi-Linear
	//resize(hogImg,hogImg,Size(378,504),INTER_CUBIC);


	//printf("%d %d ",img.cols,img.rows );

	vl_hog_delete(hog);

	return hogImg;
}
