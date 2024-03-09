#include <cstddef>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstring>
#include <toml.hpp>

#include <Eigen/Core>

#define N 510
#define enter putchar('\n')

const double dis_lim = 10;

std::vector<cv::Point2d> my_dot_array;

bool edge[N][N];

bool choose_right[N];

void mouseCallback(int event, int x, int y, int flags, void* userdata) {
    static int id,flag=0;
    double dis = -1;
    if (event == cv::EVENT_LBUTTONDOWN) {
        my_dot_array.push_back(cv::Point2d(x,y));
    }
    else if (event == cv::EVENT_RBUTTONDOWN) {
        if(flag){
            int choose_id = -1;
            for(int i=0,lim = my_dot_array.size();i<lim;++i){
                double now_dis = sqrt((my_dot_array[i].x - x)*(my_dot_array[i].x - x) + (my_dot_array[i].y - y)*(my_dot_array[i].y - y));
                std::cout <<i << " " << now_dis << std::endl;
                if(now_dis<dis_lim)
                if(dis<0 || now_dis < dis)
                dis = now_dis,
                choose_id = i;
            }
            if(!~choose_id) return;
            edge[choose_id][id] ^= 1;
            edge[id][choose_id] ^= 1;

            choose_right[id] = 0;

            flag = 0;
        }
        else{
            int choose_id = -1;
            for(int i=0,lim = my_dot_array.size();i<lim;++i){
                double now_dis = sqrt((my_dot_array[i].x - x)*(my_dot_array[i].x - x) + (my_dot_array[i].y - y)*(my_dot_array[i].y - y));
                if(now_dis<dis_lim)
                if(dis<0 || now_dis < dis)
                dis = now_dis,
                choose_id = i;
            }
            if(!~choose_id) return;
            id = choose_id;
            flag = 1;

            choose_right[id] = 1;
        }
    }
    return;
}

void mouseCallback2(int event,int x,int y,int flags,void* userdata){
    if(event == cv::EVENT_LBUTTONDOWN){

    }
    if(event == cv::EVENT_RBUTTONDOWN){
        
    }
    return;
}

std::string windowName = "field_tag_test";

cv::Mat tar;

void paint_the_graph(cv::Mat img){
    int lim = my_dot_array.size();
    for(int i=0;i<lim;++i)
        for(int j=0;j<i;++j){
            if(edge[i][j])
            cv::line(img,my_dot_array[i],my_dot_array[j],cv::Scalar(0,255,0),2);
        }

    double radius = 10;
    for(int i=0;i<lim;++i)
    cv::circle(img, my_dot_array[i], radius, choose_right[i]?cv::Scalar(255,0,0):cv::Scalar(0,0,255), -1),
    cv::putText(img, std::to_string(i), my_dot_array[i], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,255,0), 2);

    cv::imshow(windowName, img);
    tar = img.clone();
    return;
}


int main(){
    const auto toml_file = toml::parse(ROOT "src/config.toml");
    double map_width = toml::find<double>(toml_file,"map_width");
    double map_height = toml::find<double>(toml_file,"map_height");
    std::string str = "static/now_map.png";
    auto img = cv::imread(ROOT + str);

    cv::resize(img,img,cv::Size(800,800*img.rows/img.cols));

    FILE* file = fopen(ROOT "src/config_save.toml","w");

    std::cout << ROOT << std::endl;

    cv::namedWindow(windowName);
    cv::setMouseCallback(windowName, mouseCallback);

    while(1){
        int ret = cv::waitKey(10);
        if(ret==0) continue;
        switch(ret){
            case 'd':if(!my_dot_array.empty()) my_dot_array.pop_back();break;
            case 'q':goto S;
        }
        paint_the_graph(img.clone());
    }
    S:;

    cv::imwrite(ROOT "src/src_img/img_save.png",tar);

    fprintf(file,"node_tot_num = %ld\n",my_dot_array.size());
        fprintf(file,"\n");

    for(int i=0,lim1 = my_dot_array.size();i<lim1;++i){
        edge[i][i]=1;
    }

    fprintf(file,"edge_able = [\n");
    for(int i=0,lim1 = my_dot_array.size();i<lim1;++i){
        fprintf(file,"[");
        for(int j=0,lim2 = my_dot_array.size();j<lim2;++j){
            fprintf(file,"%d",edge[i][j]);
            if(j!=lim2-1)
            fprintf(file,",");
        }
        fprintf(file,"]");
        if(i!=lim1-1)
        fprintf(file,",");
        fprintf(file,"\n");
    }
    
    fprintf(file,"]");
    fprintf(file,"\n");
    fprintf(file,"\n");

    
    fprintf(file,"point_position = [");
    for(int i=0,lim = my_dot_array.size();i<lim;++i){
        double X = my_dot_array[i].x,Y = my_dot_array[i].y;
        fprintf(file,"[%lf,%lf]",X/img.cols*map_width,Y/img.rows*map_height);
        if(i!=lim-1)
        fprintf(file,",");
    }
    fprintf(file,"]");
    
    return 0;
}