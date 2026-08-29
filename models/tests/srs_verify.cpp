#include "Board.h"
#include "Tetromino.h"
#include "SRS.h"
#include <iostream>
#include <algorithm>

static const std::array<std::array<std::array<Point,5>,4>,4>&
kickTable(PieceType t){ return (t==PieceType::I)?KICK_I:KICK_JLSTZ; }

bool valid(const Board& b, const Tetromino& p){
    for(auto& blk: p.getBlocks()) if(!b.isEmpty(blk.x,blk.y)) return false;
    return true;
}
bool rotateKick(const Board& b, Tetromino& piece, Direction dir, int& used){
    int from=(int)piece.getRotationState();
    Tetromino rotated=piece; rotated.rotate(dir);
    int to=(int)rotated.getRotationState();
    const auto& tests=kickTable(piece.getPieceType())[from][to];
    for(int i=0;i<5;i++){ Tetromino t=rotated; t.move(tests[i].x,tests[i].y);
        if(valid(b,t)){ piece=t; used=i; return true; } }
    return false;
}
const char* pn(PieceType p){const char*n[]={"?","I","O","T","S","Z","J","L"};return n[(int)p];}
const char* sn(RotationState s){const char*n[]={"0","R","2","L"};return n[(int)s];}

int main(){
    Board empty;
    const PieceType pieces[]={PieceType::I,PieceType::T,PieceType::S,
                              PieceType::Z,PieceType::J,PieceType::L};
    int found=0;
    for(PieceType pt: pieces){
      for(int startState=0; startState<4; startState++){
        for(int wall=-1; wall<=1; wall+=2){ // -1 ซ้าย, +1 ขวา
          for(Direction dir : {Direction::CW, Direction::CCW}){
            // เตรียมชิ้น: หมุนไป startState ก่อน แล้วดันชนกำแพง
            Tetromino p(pt);
            for(int s=0;s<startState;s++) p.rotate(Direction::CW);
            while(true){ Tetromino c=p; c.move(wall,0); if(valid(empty,c)) p=c; else break; }
            if(!valid(empty,p)) continue;
            // หมุนไม่เตะ
            Tetromino plain=p; plain.rotate(dir);
            bool plainOk=valid(empty,plain);
            // หมุนเตะ
            int used=-1; Tetromino kicked=p;
            bool kickOk=rotateKick(empty,kicked,dir,used);
            if(!plainOk && kickOk && used>0 && found<6){
                std::cout << "ชิ้น " << pn(pt) << " state " << sn(p.getRotationState())
                          << " ชิดกำแพง" << (wall<0?"ซ้าย":"ขวา")
                          << " หมุน" << (dir==Direction::CW?"CW":"CCW")
                          << ": ไม่เตะ=ชน, เตะ=ผ่าน (test " << used << ") ,ชิ้นหลังเตะ valid="
                          << valid(empty,kicked) << "\n";
                found++;
            }
          }
        }
      }
    }
    std::cout << "\nเจอเคสที่ wall kick ช่วยได้ " << found << " เคส\n";
    if(found>0) std::cout << "[OK] kick table ทำงานจริง — หมุนติดกำแพงได้ด้วยการเตะ\n";
    return 0;
}