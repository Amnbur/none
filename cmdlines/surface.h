#ifndef RUBIKS_CUBE_H
#define RUBIKS_CUBE_H
//  魔方阶数，用模板实在是太复杂了，就直接用宏吧
/*
    to define the rubik's cube
    这个系统只是被用与可视化，预计不会进行性能上的优化
*/

#ifndef DIMENTION
#define DIMENTION 3
#endif
#define SURFACE_SIZE DIMENTION*DIMENTION

#include "D:/programs/eigen/Eigen/Eigen"
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

typedef enum Color{YELLOW=0,ORANGE=1,BLUE=2,RED=4,WHITE=5,GREEN=3,BLACK=7}Color;
typedef enum Operation{X0=0,X1,X2,Y0,Y1,Y2,Z0,Z1,Z2,_X0=9,_X1,_X2,_Y0,_Y1,_Y2,_Z0,_Z1,_Z2,UP=18,DOWN,LEFT,RIGHT}Operation;
typedef Eigen::Matrix<enum Color,1,DIMENTION> ROW;
typedef Eigen::Matrix<enum Color,DIMENTION,1> COL;
extern enum Color testCube[54];
extern enum Color rawCube[54];
extern enum Color someCube[54];

class CubeSurface{
private:
    Eigen::Matrix<enum Color,-1,-1> pieces;
public:
    CubeSurface(){
        pieces.resize(DIMENTION,DIMENTION);
    }
    CubeSurface(enum Color* piece):CubeSurface(){
        // CubeSurface();   居然是这一句的问题
        init(piece);
    }
    ~CubeSurface(){}
    void init(enum Color*piece){
        for (int index=0;index<DIMENTION;++index){
            for (auto it = pieces.block<1,DIMENTION>(index,0).begin();it!=pieces.block<1,DIMENTION>(index,0).end();++it){
                *it = *piece;
                ++piece;
            }
        }
    }
    void replaceCol(COL v,int index){
        pieces.block<DIMENTION,1>(0,index) = v;
    }
    void replaceRow(ROW v,int index){    //   参数是行向量和一个行索引
        pieces.block<1,DIMENTION>(index,0) = v;
    }
    void clockwiseRotating();
    void counterClockwiseRotating();
    void rotating(bool direction){
        if (direction)  return counterClockwiseRotating();
        else return clockwiseRotating();
    }
    void display(std::ostream&o = std::cout){
        for (int index=0;index<DIMENTION;++index){
            for (auto it = pieces.block<1,DIMENTION>(index,0).begin();it!=pieces.block<1,DIMENTION>(index,0).end();++it){
                o << *it << ' ';
            }
            o << "\b\n" ;
        }
    }
    friend class RubiksCube;    //  直接一个友元类，免去一些繁琐操作
};

class RubiksCube{   //   现在开始制作魔方类型了，应该把可视化添加进去
private:
    std::vector<CubeSurface* > surfaces;

    void turnY(bool direction);  //  true means rotating along the y-axis direction  (right-hand rule)
    void turnX(bool direction);
    void turnZ(bool direction);

    void x0 (bool direction);
    void _x0(bool direction);   //  表示最后一个
    void x(int index,bool direction);

    void y(int index,bool direction);
    void y0 (bool direction);
    void _y0(bool direction);

    void z(int index,bool direction);
    void z0 (bool direction);
    void _z0(bool direction);
public:
    RubiksCube(){
        surfaces.resize(6);
        for (auto it = surfaces.begin();   it<surfaces.end();  ++it){
            *it = new CubeSurface;
        }
    }
    ~RubiksCube(){}
    RubiksCube(enum Color* surface):RubiksCube(){
        init(surface);
    }
    void init(enum Color* surface){
        for (std::vector<CubeSurface*>::iterator surfaceIte = this->surfaces.begin();   surfaceIte != this->surfaces.end(); surfaceIte++){
            (*surfaceIte)->init(surface);
            surface += SURFACE_SIZE;
        }
    }
    void layoutDisplay(std::ostream&o = std::cout){
        char blanks[] = "                              | ";   //  是30个空格和竖线以及一个空格
        char *p = blanks+(30-DIMENTION*2);
        o << p;
    }
    void combineLayoutAndDisplay(std::stringstream&source,std::ostream&out = std::cout);
    void display(std::ostream&out = std::cout);
    void turnToUp(){turnY(true);}
    void turnToDown(){turnY(false);}
    void turnToRight(){turnX(true);}
    void turnToLeft(){turnX(false);}
    void turnCounterClockwise(){turnZ(true);}
    void turnClockwise(){turnZ(false);}
    //  下面这些是为  <三阶魔方>  定义的函数
        //  辛马斯特标记
        //  x
    void U(){x0(true);}
    void _U(){x0(false);}
    void D(){_x0(false);}
    void _D(){_x0(true);}
        //  y
    void L(){y0(true);} 
    void _L(){y0(false);}
    void R(){_y0(false);}
    void _R(){_y0(true);}
        //  z
    void B(){z0(true);}
    void _B(){z0(false);}
    void F(){_z0(false);}
    void _F(){_z0(true);}
    #if DIMENTION%2
        void MU(){x(DIMENTION/2,true);}
        void _MU(){x(DIMENTION/2,false);}
        void ML(){y(DIMENTION/2,true);}       //  这个不一样诶
        void _ML(){y(DIMENTION/2,false);}
        void MF(){z(DIMENTION/2,false);}
        void _MF(){z(DIMENTION/2,true);}
    #endif
};
#endif