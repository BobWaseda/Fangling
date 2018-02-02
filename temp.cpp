void ConnectionNester::InitTry()
{
  int image_size = 600;
  int radius = 200;

  image::Array<int> status(image_size, image_size, 1);
  
  cv::Mat status_mat;
  ImageTool::ArrayIntToMat(status, status_mat);

  cv::circle(status_mat, cv::Point(image_size/2, image_size/2),
             radius, cv::Scalar(125), -1);
             
cv::threshold(status_mat, status_mat, 140, 255, cv::THRESH_BINARY_INV);

  cv::imwrite("haha.png", status_mat);

  std::vector< std::vector<cv::Point> > contours;
  
  std::vector<cv::Vec4i> hierarchy;
  cv::findContours(status_mat, contours, hierarchy,
                   cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
cv::Rect bounding_rect =  cv::boundingRect(contours[0]);

  cv::Point reference_point = bounding_rect.tl();
  std::vector< std::vector< cv::Point > > new_contours;
  
  std::vector< cv::Point > temp_new_contours;

  for ( int i = 0; i < (int)contours[0].size(); i++ ) {
if ( contours[0][i].x != bounding_rect.tl().x
      && contours[0][i].y != bounding_rect.tl().y
      && contours[0][i].x != bounding_rect.br().x - 1
      && contours[0][i].y != bounding_rect.br().y - 1) {

      temp_new_contours.push_back(cv::Point(contours[0][i].x - reference_point.x,
                            contours[0][i].y - reference_point.y) );
    }
    
    }

  new_contours.push_back(temp_new_contours);
  
  cv::Mat new_status_mat;
  image::Array<int> new_status( bounding_rect.height,
                                bounding_rect.width, 0);
                                
  ImageAssistTool::ArrayIntToMat(new_status, new_status_mat);

  cv::drawContours(new_status_mat, new_contours, 0, cv::Scalar(255), -1);

  
  cv::imwrite("he1.png", new_status_mat);

  cv::drawContours(new_status_mat, new_contours,
                   0, cv::Scalar(0), 1);
  
  cv::imwrite("he2.png", new_status_mat);

  std::vector< std::vector<cv::Point> >().swap(contours);
  std::vector<cv::Vec4i>().swap(hierarchy);
  
  cv::findContours(new_status_mat, contours, hierarchy,
                   cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

  new_contours[0].insert(new_contours[0].end(),contours[0].begin(), contours[0].end());

  cv::Mat outlineMat = cv::Mat::zeros(bounding_rect.height,
                                      bounding_rect.width,CV_32FC1);

  cv::drawContours(outlineMat, new_contours,
                   0, cv::Scalar(255), 1);
  
}

void ConnectionNester::SpeedupPaintWithoutCheck(int reference_column,
                                                int reference_row,image::Array<int> &status) {

  for (size_t i = 0; i < connection_nester_space_img_.outline_points_.size();i++) {

    int bi = connection_nester_space_img_.outline_points_[i].y_;
    int bj = connection_nester_space_img_.outline_points_[i].x_;
    
    if (bi + reference_row < (int)status.RowSize()
     && bj + reference_column < (int)status.ColSize()
     && bi + reference_row >=0
     && bj + reference_column >=0 ) {
      
      status[bi + reference_row][bj + reference_column] = 2;
    }
  }
}


void ConnectionNester::SpeedupRemoveFromStatusWithoutCheck(int reference_column,
                                                           int reference_row,image::Array<int> &status) {

  for (size_t i = 0; i < connection_nester_space_img_.outline_points_.size();
       i++) {

    int bi = connection_nester_space_img_.outline_points_[i].y_;
    int bj = connection_nester_space_img_.outline_points_[i].x_;
if (bi + reference_row < (int)status.RowSize()
     && bj + reference_column < (int)status.ColSize()
     && bi + reference_row >=0
     && bj + reference_column >=0 ) {status[bi + reference_row][bj + reference_column] = 1;
    }
  }
}

void ImageAssistTool::ArrayIntToMat(const image::Array<int> &src, cv::Mat &dst) {
  size_t height =  src.RowSize();
  size_t width = src.ColSize();
  
  dst = cv::Mat(height, width, CV_8U);

  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      dst.at<uchar>(i, j) = src[i][j] == 1 ? 255 : 0;
    }
  }
}

void ImageAssistTool::MatToArrayInt(const cv::Mat &src, image::Array<int> &dst) {
  int width = src.cols;
  int height = src.rows;
  cv::Mat img = src.clone();
  if (src.channels() == 3) {
    cv::cvtColor(img, img, CV_BGR2GRAY);
  }
  
  cv::threshold(img, img, 200, 255, CV_THRESH_BINARY);
  dst = image::Array<int>(height, width, 0);
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      dst[i][j] = src.at<uchar>(i, j) > 0 ? 1 : 0;
    }
  }
}
