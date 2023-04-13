#include "homework1.h"


using namespace cv;
using namespace std;

void My_DFT(Mat,Mat&,Mat&);
QImage MatToImage(Mat&);

homework1::homework1(QWidget *parent)
    : QWidget(parent)
{
    
    ui->setupUi(this);
    setWindowTitle(tr("第一次作业"));
    resize(1080, 720);
    //closebutton
    //closeButton = new QPushButton(tr("close"));
    //closeButton->setParent(this);
    //closeButton->setFixedSize(100, 30);
    //closeButton->move(980, 0);

    //closeButton->setStyleSheet("QPushButton { background-color : white;"\
        "color : blue;"\
        "border-radius : 2px;"\
        "border : 2px solid green;"\
        "font : 15px;}");

    //connect(closeButton, &QPushButton::clicked, this, &homework1::close);

    //chosebutton
    choseButton = new QPushButton(tr("选择图片"));
    choseButton->setParent(this);
    choseButton->setFixedSize(200, 50);
    choseButton->move(35, 620);

    choseButton->setStyleSheet("QPushButton { background-color : white;"\
        "color : black;"\
        "border-radius : 2px;"\
        "border : 1px solid black;"\
        "font : 15px;}");
    
    connect(choseButton, &QPushButton::clicked, this, &homework1::setsourcepic);

    //bilateralFilterButton
    bilateralFilterButton = new QPushButton(tr("自适应双边滤波"));
    bilateralFilterButton->setParent(this);
    bilateralFilterButton->setFixedSize(200, 50);
    bilateralFilterButton->move(305, 620);

    bilateralFilterButton->setStyleSheet("QPushButton { background-color : white;"\
        "color : black;"\
        "border-radius : 2px;"\
        "border : 1px solid black;"\
        "font : 15px;}");

    connect(bilateralFilterButton, &QPushButton::clicked, this, &homework1::bilateralFilter);

    //fourierbutton
    fourierButton = new QPushButton(tr("傅里叶变换"));
    fourierButton->setParent(this);
    fourierButton->setFixedSize(200, 50);
    fourierButton->move(575, 620);

    fourierButton->setStyleSheet("QPushButton { background-color : white;"\
        "color : black;"\
        "border-radius : 2px;"\
        "border : 1px solid black;"\
        "font : 15px;}");

    connect(fourierButton, &QPushButton::clicked, this, &homework1::fourier);

    //myfourierbutton
    myFourierButton = new QPushButton(tr("自定义傅里叶变换"));
    myFourierButton->setParent(this);
    myFourierButton->setFixedSize(200, 50);
    myFourierButton->move(845, 620);

    myFourierButton->setStyleSheet("QPushButton { background-color : white;"\
        "color : black;"\
        "border-radius : 2px;"\
        "border : 1px solid black;"\
        "font : 15px;}");

    connect(myFourierButton, &QPushButton::clicked, this, &homework1::myfourier);

    //sourcepic label
    sourcepic = new QLabel;
    sourcepic->setParent(this);
    sourcepic->setFixedSize(1024,576);
    sourcepic->move(28, 28);
    // 设置框框
    sourcepic->setFrameShape(QFrame::Box);
    // 设置线条宽度
    sourcepic->setLineWidth(1);


}

//选择图片按钮
void homework1::setsourcepic() {
    fileurl= QFileDialog::getOpenFileName(this, tr("Open Image"), "/home/jana", tr("Image Files (*.png *.jpg *.bmp)"));
    QPixmap pix = QPixmap();
    pix.load(fileurl);
    sourcepic->setPixmap(pix.scaled(QSize(1024,576), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    sourcepic->setAutoFillBackground(true);
}
//自适应双边滤波
void homework1::bilateralFilter()
{
    QString newfile = fileurl;
    Mat image, image_gray, image_output, image_transform;

    image = imread(fileurl.toStdString());
    cvtColor(image, image_gray, COLOR_BGR2GRAY);
    cv::bilateralFilter(image, image_output, 9, 50, 25 / 2);

    imshow("自适应双边滤波", image_output);
    
}
//傅里叶
void homework1::fourier()
{
    QString newfile = fileurl;
    Mat image, image_gray, image_output, image_transform;

    image = imread(fileurl.toStdString());
    cvtColor(image, image_gray, COLOR_BGR2GRAY);
    My_DFT(image_gray, image_output, image_transform);

    imshow("傅里叶变换", image_output);
}
//自定义傅里叶
void homework1::myfourier()
{
    QString newfile = fileurl;
    Mat image, image_gray, image_output, image_transform;

    image = imread(fileurl.toStdString());
    cvtColor(image, image_gray, COLOR_BGR2GRAY);
    My_DFT(image_gray, image_output, image_transform);

    imshow("傅里叶变换", image_output);
}


homework1::~homework1()
{
    delete ui;
}

void My_DFT(Mat input_image, Mat& output_image, Mat& transform_image)
{
    //1.扩展图像矩阵，为2，3，5的倍数时运算速度快
    int m = getOptimalDFTSize(input_image.rows);
    int n = getOptimalDFTSize(input_image.cols);
    copyMakeBorder(input_image, input_image, 0, m - input_image.rows, 0, n - input_image.cols, BORDER_CONSTANT, Scalar::all(0));

    //2.创建一个双通道矩阵planes，用来储存复数的实部与虚部
    Mat planes[] = { Mat_<float>(input_image), Mat::zeros(input_image.size(), CV_32F) };

    //3.从多个单通道数组中创建一个多通道数组:transform_image。函数Merge将几个数组合并为一个多通道阵列，即输出数组的每个元素将是输入数组元素的级联
    merge(planes, 2, transform_image);

    //4.进行傅立叶变换
    dft(transform_image, transform_image);

    //5.计算复数的幅值，保存在output_image（频谱图）
    split(transform_image, planes); // 将双通道分为两个单通道，一个表示实部，一个表示虚部
    magnitude(planes[0], planes[1], output_image); //计算复数的幅值，保存在output_image（频谱图）

    //6.前面得到的频谱图数级过大，不好显示，因此转换
    output_image += Scalar(1);   // 取对数前将所有的像素都加1，防止log0
    log(output_image, output_image);   // 取对数
    normalize(output_image, output_image, 0, 1, NORM_MINMAX); //归一化

    //7.剪切和重分布幅度图像限
    output_image = output_image(Rect(0, 0, output_image.cols & -2, output_image.rows & -2));

    // 重新排列傅里叶图像中的象限，使原点位于图像中心
    int cx = output_image.cols / 2;
    int cy = output_image.rows / 2;
    Mat q0(output_image, Rect(0, 0, cx, cy));   // 左上区域
    Mat q1(output_image, Rect(cx, 0, cx, cy));  // 右上区域
    Mat q2(output_image, Rect(0, cy, cx, cy));  // 左下区域
    Mat q3(output_image, Rect(cx, cy, cx, cy)); // 右下区域

    //交换象限中心化
    Mat tmp;
    q0.copyTo(tmp); q3.copyTo(q0); tmp.copyTo(q3);//左上与右下进行交换
    q1.copyTo(tmp); q2.copyTo(q1); tmp.copyTo(q2);//右上与左下进行交换
}


QImage MatToImage(Mat& m)
{
    //判断m的类型，可能是CV_8UC1  CV_8UC2  CV_8UC3  CV_8UC4
    switch (m.type())
    { //QIamge 构造函数, ((const uchar *data, 宽(列),高(行), 一行共多少个（字节）通道，宽度*字节数，宏参数)
    case CV_8UC1:
    {
        QImage img((uchar*)m.data, m.cols, m.rows, m.cols * 1, QImage::Format_Grayscale8);
        return img;
    }
    break;
    case CV_8UC3:   //一个像素点由三个字节组成
    {
        //cvtColor(m,m,COLOR_BGR2RGB); BGR转RGB
        QImage img((uchar*)m.data, m.cols, m.rows, m.cols * 3, QImage::Format_RGB888);
        return img.rgbSwapped(); //opencv是BGR  Qt默认是RGB  所以RGB顺序转换
    }
    break;
    case CV_8UC4:
    {
        QImage img((uchar*)m.data, m.cols, m.rows, m.cols * 4, QImage::Format_RGBA8888);
        return img;
    }
    break;
    default:
    {
        QImage img; //如果遇到一个图片均不属于这三种，返回一个空的图片
        return img;
    }
    }
}

