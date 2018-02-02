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
