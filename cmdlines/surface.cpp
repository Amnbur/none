#include "surface.h"

enum Color testCube[54]={YELLOW,YELLOW,YELLOW,YELLOW,BLACK,YELLOW,YELLOW,BLACK,YELLOW,
                            ORANGE,ORANGE,ORANGE,ORANGE,BLACK,BLACK,ORANGE,ORANGE,ORANGE,
                            BLUE,BLUE,BLACK,BLUE,BLACK,BLUE,BLUE,BLUE,BLUE,
                            GREEN,GREEN,BLACK,GREEN,BLACK,GREEN,GREEN,GREEN,GREEN,
                            RED,RED,RED,BLACK,BLACK,RED,RED,RED,RED,
                            WHITE,BLACK,WHITE,WHITE,BLACK,WHITE,WHITE,WHITE,WHITE};
enum Color rawCube[54]={YELLOW,YELLOW,YELLOW,YELLOW,BLACK,YELLOW,YELLOW,YELLOW,YELLOW,
                            ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,
                            BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,
                            GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,
                            RED,RED,RED,RED,RED,RED,RED,RED,RED,
                            WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE};
enum Color someCube[54] = {YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,ORANGE,BLUE,GREEN,
                            ORANGE,ORANGE,BLUE,ORANGE,ORANGE,WHITE,ORANGE,ORANGE,GREEN,
                            BLUE,BLUE,BLACK,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,
                            GREEN,GREEN,BLACK,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,
                            YELLOW,RED,RED,BLUE,RED,RED,ORANGE,RED,RED,
                            RED,BLUE,ORANGE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE};

void CubeSurface::clockwiseRotating(){
        ROW rows[DIMENTION];
        for (int i =0;i<DIMENTION;++i){
            rows[i] = pieces.block<1,DIMENTION>(i,0);
        }
        for (int i=0;i<DIMENTION;++i){
            replaceCol(rows[DIMENTION-i-1].transpose(),i);
        }
    }
void CubeSurface::counterClockwiseRotating(){
    COL cols[DIMENTION];
    for (int i =0;i<DIMENTION;++i){
        cols[i] = pieces.block<DIMENTION,1>(0,i);
    }
    for (int i=0;i<DIMENTION;++i){
        replaceRow(cols[DIMENTION-i-1].transpose(),i);
    }
}

//__RubiksCube________________________________________________________

void RubiksCube::turnY(bool direction){  //  true means rotating along the y-axis direction  (right-hand rule)
    int up[]={0,3,5,2,1,4};
    int down[]={0,2,5,3,4,1};
    int *p = direction?up:down;

    CubeSurface* tmp = surfaces[p[0]];
    surfaces[p[0]] = surfaces[p[1]];
    surfaces[p[1]] = surfaces[p[2]];
    surfaces[p[2]] = surfaces[p[3]];
    surfaces[p[3]] = tmp;
    surfaces[p[4]]->clockwiseRotating();
    surfaces[p[5]]->counterClockwiseRotating();        
}
void RubiksCube::turnX(bool direction){
    int right[] = {2,4,3,1,5,0};
    int left[]  = {2,1,3,4,0,5};
    int *p=direction?right:left;

    surfaces[p[3]]->clockwiseRotating();
    surfaces[p[3]]->clockwiseRotating();
    surfaces[3]->clockwiseRotating();
    surfaces[3]->clockwiseRotating();

    CubeSurface* tmp = surfaces[p[0]];
    surfaces[p[0]] = surfaces[p[1]];
    surfaces[p[1]] = surfaces[p[2]];
    surfaces[p[2]] = surfaces[p[3]];
    surfaces[p[3]] = tmp;
    surfaces[p[4]]->counterClockwiseRotating();
    surfaces[p[5]]->clockwiseRotating();
}
void RubiksCube::turnZ(bool direction){
    int counterClockwise[] = {0,4,5,1};
    int clockwise[] = {0,1,5,4};
    int *p = direction?counterClockwise:clockwise;

    surfaces[2]->rotating(direction);
    surfaces[3]->rotating(! direction);
    surfaces[0]->rotating(direction);
    surfaces[1]->rotating(direction);
    surfaces[5]->rotating(direction);
    surfaces[4]->rotating(direction);
    
    CubeSurface* tmp = surfaces[0];
    surfaces[p[0]] = surfaces[p[1]];
    surfaces[p[1]] = surfaces[p[2]];
    surfaces[p[2]] = surfaces[p[3]];
    surfaces[p[3]] = tmp;
}

//  rotation:x
void RubiksCube::x0(bool direction){
    surfaces[0]->rotating(! direction);
    x(0,direction);
}
void RubiksCube::_x0(bool direction){
    surfaces[5]->rotating(direction);
    x(DIMENTION-1,direction);
}
void RubiksCube::x(int index,bool direction){
    if (index >= DIMENTION || index<0){
        throw "index out of range";
        return;
    }
    int negative[] = {2,1,3,4};
    int positive[] = {2,4,3,1};
    int *order = direction?positive:negative;

    Eigen::Matrix<Color,1,DIMENTION> tmpRow;
    tmpRow = surfaces[order[0]]->pieces.block<1,DIMENTION>(index,0);
    surfaces[order[0]]->pieces.block<1,DIMENTION>(index,0) = surfaces[order[1]]->pieces.block<1,DIMENTION>(index,0);
    surfaces[order[1]]->pieces.block<1,DIMENTION>(index,0) = surfaces[order[2]]->pieces.block<1,DIMENTION>(DIMENTION-1-index,0).reverse();
    surfaces[order[2]]->pieces.block<1,DIMENTION>(DIMENTION-index-1,0) = surfaces[order[3]]->pieces.block<1,DIMENTION>(index,0).reverse();
    surfaces[order[3]]->pieces.block<1,DIMENTION>(index,0) = tmpRow;
}

//  rotationg:y
void RubiksCube::y(int index,bool direction){
    if (index >= DIMENTION || index<0){throw "index out of range";return;}
        //  这个是最容易的
    int positive[] = {0,2,5,3};
    int negative[] = {0,3,5,2};
    int *order = direction?positive:negative;

    Eigen::Matrix<Color,DIMENTION,1> tmpCol = surfaces[order[0]]->pieces.block<DIMENTION,1>(0,index);
    surfaces[order[0]]->pieces.block<DIMENTION,1>(0,index) = surfaces[order[1]]->pieces.block<DIMENTION,1>(0,index);
    surfaces[order[1]]->pieces.block<DIMENTION,1>(0,index) = surfaces[order[2]]->pieces.block<DIMENTION,1>(0,index);
    surfaces[order[2]]->pieces.block<DIMENTION,1>(0,index) = surfaces[order[3]]->pieces.block<DIMENTION,1>(0,index);
    surfaces[order[3]]->pieces.block<DIMENTION,1>(0,index) = tmpCol;
}
void RubiksCube::y0(bool direction){
    surfaces[1]->rotating(! direction);
    y(0,direction);
};
void RubiksCube::_y0(bool direction){
    surfaces[4]->rotating(direction);
    y(DIMENTION-1,direction);
}

    //  rotating:z
void RubiksCube::z(int index,bool direction){
    if (index >= DIMENTION || index<0){throw "index out of range";return;}
    //  这个似乎会比较麻烦
    int positive[] = {5,1,0,4};
    int negative[] = {5,4,0,1};
    int *order = direction?positive:negative;

    Eigen::Matrix<Color,1,DIMENTION> tmpRow = surfaces[order[0]]->pieces.row(DIMENTION-1-index);   //  Z轴正方向是向前，所以第0层是back
    surfaces[order[0]]->pieces.row(DIMENTION-1-index) = surfaces[order[1]]->pieces.col(direction?index:(DIMENTION-1-index)).transpose();  //  底等于左
    // cout << "********************\n";this->display();
        //  原始的debug方式，但有效
    surfaces[order[1]]->pieces.col(direction?index:(DIMENTION-1-index)) = surfaces[order[2]]->pieces.row(index).transpose();  //  左等于顶
    // cout << "********************\n";this->display();
    surfaces[order[2]]->pieces.row(index) = surfaces[order[3]]->pieces.col(direction?(DIMENTION-1-index):index).transpose();  //  顶等于右
    surfaces[order[3]]->pieces.col(direction?(DIMENTION-1-index):index) = tmpRow.transpose();                                                  //  右等于底
        //  我可能需要一些快捷方式来缩短这些表达式
    if (direction){
        // surfaces[1]->pieces.col(index) = surfaces[1]->pieces.col(index).reverse();  //  是这里出错了
        //  后面3行同样的错误删去了。
        surfaces[1]->pieces.col(index).reverseInPlace();
        surfaces[4]->pieces.col(DIMENTION-1-index).reverseInPlace();    //  有专门的函数用来reverse
    }else{
        surfaces[5]->pieces.row(DIMENTION-1-index).reverseInPlace();
        surfaces[0]->pieces.row(index).reverseInPlace();

    }
}
void RubiksCube::z0(bool direction){
    surfaces[3]->rotating(! direction); //  因为正方向是逆时针
    z(0,direction);
}
void RubiksCube::_z0(bool direction){
    surfaces[2]->rotating(direction);
    z(DIMENTION-1,direction);
}
//  endof rotation

void RubiksCube::combineLayoutAndDisplay(std::stringstream&source,std::ostream&out){
    char buf[1024];
    for (int i = 0;i<DIMENTION;++i){
        layoutDisplay(out);
        source.getline(buf,1024);
        out << buf << " |\n";
    }
}
void RubiksCube::display(std::ostream&out){
    std::stringstream buf;
    std::stringstream buff;
    std::stringstream bufff;
    char bufc[1024];
    surfaces[0]->display(buf);
    combineLayoutAndDisplay(buf,out);   //  以上显示顶面

    //  第二行稍显复杂
    surfaces[1]->display(buf);
    surfaces[2]->display(buff);
    surfaces[4]->display(bufff);    //  ...索引写成了2,3,5,于是出错了
    out << '\n';
    for (int i =0;i<DIMENTION;++i){
        buf.getline(bufc,1024);
        out << bufc << " | ";
        buff.getline(bufc,1024);
        out << bufc << " | ";
        bufff.getline(bufc,1024);
        out << bufc << std::endl;
    }
    out << '\n';
    surfaces[5]->display(buf);
    combineLayoutAndDisplay(buf,out);   //  以上显示底面
    out << '\n';
    surfaces[3]->display(buf);
    combineLayoutAndDisplay(buf,out);   //  以上显示后面
}


//class CubeSurfacePiece{
//private:

//public:
//    char color;
//    CubeSurfacePiece(){}
//    ~CubeSurfacePiece(){}

//};



	// Eigen::Matrix<int,-1,-1>m;
	// m.resize(3,3);
	// m << 1,2,3,4,5,6,7,8,9;
	// std::cout << m;


     // ROW tmp;
            // for (auto it = tmp.begin(); it!=tmp.end() ;++it){
            //     *it = *piece;
            //     piece++;
            // }
            // replaceRow(tmp,index);

/*
enum Color* p=c;
    while (*p!=0){
        cout << *p++ <<' ';
    }
*/

/*
RubiksCube::x0
    Eigen::Matrix<Color,1,DIMENTION> tmpRow;
    tmpRow = surfaces[order[0]]->pieces.block<1,DIMENTION>(0,0);
    surfaces[order[0]]->pieces.block<1,DIMENTION>(0,0) = surfaces[order[1]]->pieces.block<1,DIMENTION>(0,0);
    // std::cout << "testing\n";this->display();    //  这玩意儿要怎么调试啊！！！
    surfaces[order[1]]->pieces.block<1,DIMENTION>(0,0) = surfaces[order[2]]->pieces.block<1,DIMENTION>(DIMENTION-1,0).reverse();
    surfaces[order[2]]->pieces.block<1,DIMENTION>(DIMENTION-1,0) = surfaces[order[3]]->pieces.block<1,DIMENTION>(0,0).reverse();
    surfaces[order[3]]->pieces.block<1,DIMENTION>(0,0) = tmpRow;
*/
/*
RubiksCube::_x0
    Eigen::Matrix<Color,1,DIMENTION> tmpRow;
    tmpRow = surfaces[order[0]]->pieces.block<1,DIMENTION>(DIMENTION-1,0);
    surfaces[order[0]]->pieces.block<1,DIMENTION>(DIMENTION-1,0) = surfaces[order[1]]->pieces.block<1,DIMENTION>(DIMENTION-1,0);
    surfaces[order[1]]->pieces.block<1,DIMENTION>(DIMENTION-1,0) = surfaces[order[2]]->pieces.block<1,DIMENTION>(0,0).reverse();
    surfaces[order[2]]->pieces.block<1,DIMENTION>(0,0) = surfaces[order[3]]->pieces.block<1,DIMENTION>(DIMENTION-1,0).reverse();
    surfaces[order[3]]->pieces.block<1,DIMENTION>(DIMENTION-1,0) = tmpRow;
*/

/*
void RubiksCube::z(int index,bool direction){
    if (index >= DIMENTION || index<0){throw "index out of range";return;}
    //  这个似乎会比较麻烦
    int positive[] = {5,1,0,4};
    int negative[] = {5,4,0,1};
    int *order = direction?positive:negative;

    Eigen::Matrix<Color,1,DIMENTION> tmpRow = surfaces[order[0]]->pieces.block<1,3>(DIMENTION-1-index,0);   //  Z轴正方向是向前，所以第0层是back
    surfaces[order[0]]->pieces.block<1,DIMENTION>(DIMENTION-1-index,0) = surfaces[order[1]]->pieces.block<DIMENTION,1>(0,direction?index:(DIMENTION-1-index)).transpose();  //  底等于左
    // cout << "********************\n";this->display();    //  原始的debug方式，但有效
    surfaces[order[1]]->pieces.block<DIMENTION,1>(0,direction?index:(DIMENTION-1-index)) = surfaces[order[2]]->pieces.block<1,DIMENTION>(index,0).transpose();  //  左等于顶
    surfaces[order[2]]->pieces.block<1,DIMENTION>(index,0) = surfaces[order[3]]->pieces.block<DIMENTION,1>(0,direction?(DIMENTION-1-index):index).transpose();  //  顶等于右
    surfaces[order[3]]->pieces.block<DIMENTION,1>(0,direction?(DIMENTION-1-index):index) = tmpRow.transpose();                                                  //  右等于底
        //  我可能需要一些快捷方式来缩短这些表达式
    if (direction){
        surfaces[1]->pieces.block<DIMENTION,1>(0,index) = surfaces[1]->pieces.block<DIMENTION,1>(0,index).reverse();
        surfaces[4]->pieces.block<DIMENTION,1>(0,DIMENTION-1-index) = surfaces[4]->pieces.block<DIMENTION,1>(0,DIMENTION-1-index).reverse();
    }else{
        surfaces[5]->pieces.block<1,DIMENTION>(DIMENTION-1-index,0) = surfaces[5]->pieces.block<1,DIMENTION>(DIMENTION-1-index,0).reverse();
        surfaces[0]->pieces.block<1,DIMENTION>(index,0) = surfaces[0]->pieces.block<1,DIMENTION>(index,0).reverse();
    }
}
*/